#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>

// credits: AimTux
class VMT
{
private:
	uintptr_t* vmt;
public:
	// New virtual method table
	uintptr_t** object = nullptr;

	uintptr_t* original_vmt = nullptr;

	unsigned int methodCount = 0;

	VMT(void* object)
	{
		this->object = reinterpret_cast<uintptr_t**>(object);

		size_t method_count = 0;

		while (reinterpret_cast<uintptr_t*>(*this->object)[method_count])
			method_count++;

		original_vmt = *this->object;

		vmt = new uintptr_t[sizeof(uintptr_t) * method_count];

		memcpy(vmt, original_vmt, sizeof(uintptr_t) * method_count);
	}

	// Hook virtual method
	void HookVM(void* method, size_t methodIndex)
	{
		vmt[methodIndex] = reinterpret_cast<uintptr_t>(method);
	}

	template<typename Fn>
	Fn GetOriginalMethod(size_t methodIndex)
	{
		return reinterpret_cast<Fn>(original_vmt[methodIndex]);
	}

	void ApplyVMT()
	{
		*this->object = vmt;
	}

	void ReleaseVMT()
	{
		*this->object = original_vmt;
	}
};

