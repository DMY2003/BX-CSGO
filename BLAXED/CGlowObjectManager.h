#pragma once

class CGlowObjectManager
{
public:
	class GlowObjectDefinition_t
	{
	public:
		void Set(Color color)
		{
			m_vGlowColor = Vector(color.Base()[0], color.Base()[1], color.Base()[2]);
			m_flGlowAlpha = color.Base()[3];

			if (color.a() != 0)
			{
				m_bRenderWhenOccluded = true;
				m_bRenderWhenUnoccluded = false;
				m_flBloomAmount = 1.f;
			}
			else
			{
				m_bRenderWhenOccluded = false;
				m_bRenderWhenUnoccluded = false;
			}
		}

		CBaseEntity* GetEntity()
		{
			return m_hEntity;
		}

		bool IsEmpty() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

	public:
		CBaseEntity*		m_hEntity;
		Vector				m_vGlowColor;
		float				m_flGlowAlpha;

		char				unknown[4];
		float				flUnk;
		float				m_flBloomAmount;
		float				localplayeriszeropoint3;

		bool				m_bRenderWhenOccluded;
		bool				m_bRenderWhenUnoccluded;
		bool				m_bFullBloomRender;
		char				unknown1[1];

		int					m_nFullBloomStencilTestValue;
		int					iUnk;
		int					m_nSplitScreenSlot;
		int					m_nNextFreeSlot;

		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	GlowObjectDefinition_t* m_GlowObjectDefinitions;
	int		max_size;
	int		pad;
	int		size;
	GlowObjectDefinition_t* m_GlowObjectDefinitions2;
	int		currentObjects;
};
