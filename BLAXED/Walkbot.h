#pragma once

#include "SDK.h"

namespace Walkbot
{
	class CWayPoint
	{
	private:
		Vector m_vecPosition;
		CUserCmd m_Cmd;
		bool m_bFinal;

	public:
		CWayPoint(CUserCmd* pCmd, const Vector& position)
		{
			m_vecPosition = position;
			memcpy(&m_Cmd, pCmd, sizeof(CUserCmd));
			m_bFinal = true;
		}

		CUserCmd* GetCmd()
		{
			return &m_Cmd;
		}

		const Vector& GetPosition()
		{
			return m_vecPosition;
		}

		bool IsFinal()
		{
			return m_bFinal;
		}

		void SetFinal(bool final)
		{
			this->m_bFinal = final;
		}
	};

	class CPath
	{
	private:
		std::vector<CWayPoint> m_Nodes;
		std::string m_szName;

	public:
		CPath(const std::string& name)
		{
			this->m_szName = name;
			m_Nodes.clear();
		}

		const std::string& GetName()
		{
			return m_szName;
		}

		const CWayPoint& GetNode(int index)
		{
			return m_Nodes[index];
		}

		const std::vector<CWayPoint>& GetNodes()
		{
			return m_Nodes;
		}

		void ClearNodes()
		{
			m_Nodes.clear();
		}

		void InsertNode(CWayPoint node)
		{
			m_Nodes.end()->SetFinal(false);
			m_Nodes.push_back(node);
			m_Nodes.end()->SetFinal(true);
		}

		int GetClosestNode(int actual)
		{
			float bestDistance = FLT_MAX;
			int index = -1;

			for (auto i = 0; i < m_Nodes.size(); i++)
			{
				float distance = (m_Nodes[actual].GetPosition() - m_Nodes[i].GetPosition()).Length();

				if (distance < bestDistance)
				{
					bestDistance = distance;
					index = (int)actual;
				}
			}
		}
	};

	class CWalkbot
	{
	private:
		int m_iLastError;
		std::vector<CPath> m_Paths;

		enum ERRORS : int
		{
			SUCESS,
			PATH_ALREADY_EXISTS,
		};
		
	public:
		CWalkbot()
		{
			m_iLastError = SUCESS;
			m_Paths.clear();
		}

		const std::vector<CPath>& GetPaths()
		{
			return m_Paths;
		}

		int GetLastError()
		{
			return m_iLastError;
		}

		bool CreatePath(const std::string& name)
		{
			for (auto i = 0; i < m_Paths.size(); i++)
			{
				if (m_Paths[i].GetName() == name)
				{
					m_iLastError = PATH_ALREADY_EXISTS;
					return false;
				}
			}
		}
	};

	class CRecorder
	{
	private:
		int m_iLastRecordedTick;
		int m_iResolution;
		CPath* m_Path;
		bool m_bStopped;

	public:
		CRecorder(CPath* path, int resolution)
		{
			m_iResolution = resolution;
			m_iLastRecordedTick = 0;
			m_Path = path;
			m_bStopped = true;
		}

		void Tick(CUserCmd *pCmd, CBaseEntity *entity)
		{
			if (m_bStopped)
				return;

			if (m_iLastRecordedTick - pCmd->tick_count >= m_iResolution)
			{
				CWayPoint wayPoint = CWayPoint(pCmd, entity->GetOrigin());				
				m_Path->InsertNode(wayPoint);
				m_iLastRecordedTick = pCmd->tick_count;
			}
		}

		void Stop()
		{
			m_bStopped = true;
		}

		void Start()
		{
			m_bStopped = false;
			m_iLastRecordedTick = 0;
		}
	};
};
