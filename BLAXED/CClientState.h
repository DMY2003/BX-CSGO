#pragma once

class CClientState
{
public:
	char pad_0000[148]; //0x0000
	class INetChannel* m_pNetChannel; //0x0094
	char pad_0098[16]; //0x0098
	unsigned int m_nRetryNumber; //0x00A8
	char pad_00AC[84]; //0x00AC
	int m_nSignonState; //0x0100
	char pad_0104[8]; //0x0104
	float m_flNextCmdTime; //0x010C
	int m_nServerCount; //0x0110
	int m_nCurrentSequence; //0x0114
	char pad_0118[75]; //0x0118
	int m_nServerTick; //0x0163
	int m_nClientTick; //0x0167
	int m_nDeltaTick; //0x016B
	char pad_016F[4]; //0x016F
	int m_nViewEntity; //0x0173
	char pad_0177[8]; //0x0177
	char m_szLevelName[260]; //0x017F
	char m_szLevelNameShort[40]; //0x0283
	char pad_02AB[18940]; //0x02AB
	char pad_0x2DA0; //0x4CA7
	int lastoutgoingcommand; //0x4CA8
	int chokedcommands; //0x4CAC
	int last_command_ack; //0x4CB0
	int command_ack; //0x4CB4
	int m_nSoundSequence; //0x4CB8
	char pad_4CBC[8]; //0x4CBC
	bool ishltv; //0x4CC4
};
