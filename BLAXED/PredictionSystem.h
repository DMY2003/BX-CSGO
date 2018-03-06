
class CPredictionSystem 
{
public:
	static void Init();
	void StartPrediction(CUserCmd *pCmd, CBaseEntity *entity);
	void Predict();
	void EndPrediction(CBaseEntity *entity);

private:
	float m_flOldCurTime;
	float m_flOldFrametime;
	CMoveData moveData;
};
extern int* m_pPredictionRandomSeed;
extern CBaseEntity** m_pPredictionPlayer;
extern CPredictionSystem *predictionSystem;
