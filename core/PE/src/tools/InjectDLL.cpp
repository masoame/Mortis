#include "InjectDLL.h"

namespace Mortis::PE {

	bool InjectDll(DWORD th32ProcessID, std::wstring_view dll)
	{
		//通过相应进程id打开对应进程
		const auto process_handle = OpenProcessHandle(th32ProcessID);

		if (process_handle == nullptr) {
			return false;
		}
		//文件路径
		std::wstring location = std::filesystem::path(os::GetModuleFileNameW()).replace_filename(dll);

		//为对应内存申请进程，返回申请内存的地址
		const auto allocated_mem = ::VirtualAllocEx(process_handle, nullptr, (location.size() + 1) * sizeof(wchar_t), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (allocated_mem == nullptr) {
			return false;
		}

		//写入对应内存dll路径
		const auto is_write_success = ::WriteProcessMemory(process_handle, allocated_mem, location.data(), (location.size() + 1) * sizeof(wchar_t), 0);
		if (is_write_success == FALSE) {
			return false;
		}

		//注入线程
		ScopeHandle<> target_thread = ::CreateRemoteThread(process_handle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryW, allocated_mem, 0, 0);
		if (target_thread == false) {
			return false;
		}
		
		const auto wait_result = ::WaitForSingleObject(target_thread, INFINITE);
		if (wait_result == WAIT_FAILED) {
			return false;
		}

		//释放申请的内存
		const auto is_memory_freed = ::VirtualFreeEx(process_handle, allocated_mem, 0, MEM_RELEASE);
		if (is_memory_freed == false) {
			return false;
		}

		return true;
	}

}