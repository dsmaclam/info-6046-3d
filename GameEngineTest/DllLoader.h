#pragma once

#include <string>
#include <Windows.h>

class DllLoader
{
	HMODULE dll_handle_;

public:
	DllLoader() : dll_handle_(nullptr) {}

	bool load(const std::string& path)
	{
		const auto handle = LoadLibraryA(path.c_str());

		if (handle)
		{
			dll_handle_ = handle;
			return true;
		}

		return false;
	}

	template<typename T> T get_proc_address(const std::string& proc_name)
	{
		return reinterpret_cast<T>(GetProcAddress(dll_handle_, proc_name.c_str()));
	}
};
