#pragma once

#include "SDK.h"

namespace GranadePrediction
{
	// https://www.unknowncheats.me/forum/counterstrike-global-offensive/223164-grenade-prediction.html
	// FIXME: https://www.unknowncheats.me/forum/counterstrike-global-offensive/127412-grenade-prediction.html

	const int MAX_POINTS = 500;
	 
	class ShowPoint
	{
	public:
		ShowPoint() { valid = false; }
		ShowPoint(Vector start_, Vector end_, bool plane_, bool valid_, Vector normal_, bool detonate_);

		Vector start;
		Vector end;
		bool valid;
		bool plane;
		Vector normal;
		bool detonate;
	};

	class Predict
	{
	private:		
		ShowPoint m_Points[MAX_POINTS];
		CBaseEntity* m_pEntity;
		bool m_bPredicted;

	public:

		Predict();
		ShowPoint *GetPoints();
		bool IsPredicted();
		void SetPredicted(bool value);
		void PredictNade(CBaseEntity* entity);
	};

	extern Predict *pInstance;
};
