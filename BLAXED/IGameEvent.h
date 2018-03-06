#pragma once

class IGameEvent
{
public:
	virtual ~IGameEvent() {};
	virtual const char *GetName() const = 0;    // get event name

	virtual bool  IsReliable() const = 0; // if event handled reliable
	virtual bool  IsLocal() const = 0; // if event is never networked
	virtual bool  IsEmpty(const char *keyName = NULL) = 0; // check if data field exists

														   // Data access
	virtual bool  GetBool(const char *keyName = NULL, bool defaultValue = false) = 0;
	virtual int   GetInt(const char *keyName = NULL, int defaultValue = 0) = 0;
	virtual unsigned long long GetUint64(char const *keyName = NULL, unsigned long long defaultValue = 0) = 0;
	virtual float GetFloat(const char *keyName = NULL, float defaultValue = 0.0f) = 0;

	virtual const char *GetString(const char *keyName = NULL, const char *defaultValue = "") = 0;
	virtual const wchar_t *GetWString(char const *keyName = NULL, const wchar_t *defaultValue = L"") = 0;

	virtual void Unk() = 0;

	virtual void SetBool(const char *keyName, bool value) = 0;
	virtual void SetInt(const char *keyName, int value) = 0;
	virtual void SetUInt64(const char *keyName, unsigned long long value) = 0;
	virtual void SetFloat(const char *keyName, float value) = 0;
	virtual void SetString(const char *keyName, const char *value) = 0;
	virtual void SetWString(const char *keyName, const wchar_t *value) = 0;
};

class IGameEventListener2
{
public:
	virtual	~IGameEventListener2(void) {};

	/*
	//Event class handler from engine:
	55F275A0   FF50 08          CALL DWORD PTR DS:[EAX+8]	; Call IndicateEventHandling() method
	55F275A3   83F8 2A          CMP EAX,2A	; Compare result value in EAX with 0x2A
	55F275A6   74 1C            JE SHORT engine.55F275C4	; If equal jump to part where to proceed handling
	...
	55F275C4   8B03             MOV EAX,DWORD PTR DS:[EBX]	; Copy DWORD at [EBX] to EAX (address to VFT of this class instance)
	55F275C6   8BCB             MOV ECX,EBX	; Copy EBX to ECX	; Copy base address of this class instance to ECX as this-pointer
	55F275C8   57               PUSH EDI	; Push EDI onto stack as function argument (holds IGameEvent instance address)
	55F275C9   FF50 04          CALL DWORD PTR DS:[EAX+4]	; Call HandleGameEvent() method

	//Example HandleGameEvent() method
	55EC6390   55               PUSH EBP	; Backup old EBP value onto stack
	55EC6391   8BEC             MOV EBP,ESP	; Copy value from ESP to EBP, stack frame pointer setup done
	55EC6393   83EC 10          SUB ESP,10	; Allocate 0x10 bytes on stack
	55EC6396   57               PUSH EDI	; Backup EDI onto stack
	55EC6397   8BF9             MOV EDI,ECX	; Copy ECX to EDI (class instance this-pointer)
	55EC6399   807F 08 00       CMP BYTE PTR DS:[EDI+8],0	; Compare byte at [EDI+0x08] with 0
	55EC639D   0F84 6F030000    JE engine.55EC6712	; If equal then jump to address
	...
	55EC6712   5F               POP EDI	; Restore EDI from stack
	55EC6713   8BE5             MOV ESP,EBP	; Copy value of EBP to ESP to free the allocated 0x10 stack space
	55EC6715   5D               POP EBP	; Restore old frame pointer
	55EC6716   C2 0400          RETN 4	; Restore return address and free 4 bytes of argument stack space

	//Example IndicateEventHandling() method
	55D964E0   8B41 04          MOV EAX,DWORD PTR DS:[ECX+4] ; Should copy 0x2A to EAX if HandleGameEvent() shall get called
	55D964E3   C3               RETN	; Restore return address
	*/

	//Called when indicated that the event shall be handled
	virtual void HandleGameEvent(IGameEvent *pEvent) = 0;

	//Called to indicate whether 'HandleGameEvent' shall be called or not
	virtual int IndicateEventHandling(void) = 0;
};

class IGameEventManager2 
{
public:
	bool AddListener(IGameEventListener2* pListener, std::string strName, bool bServerSide)
	{
		typedef bool(__thiscall *OriginalFn)(void*, IGameEventListener2*, const char*, bool);
		const char* c = strName.c_str();
		return Mem::CallVirtualFunction<OriginalFn>(this, 3)(this, pListener, c, bServerSide);
	}

	void RemoveListener(IGameEventListener2* pListener)
	{
		typedef bool(__thiscall *OriginalFn)(void*, IGameEventListener2*);
		Mem::CallVirtualFunction< OriginalFn >(this, 5)(this, pListener);
	}

	bool FireEventClientSide(IGameEvent* pGameEvent)
	{
		typedef bool(__thiscall *OriginalFn)(void*, IGameEvent*);
		return Mem::CallVirtualFunction< OriginalFn >(this, 8)(this, pGameEvent);
	}
};
