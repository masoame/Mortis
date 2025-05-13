#include <common.hpp>

namespace DllHook
{

#ifndef _WIN64
	using HOOKWORD = DWORD32;
#else
	using HOOKWORD = DWORD64;
#endif
	std::unique_ptr<HOOKWORD[]> GetFuncArgs(const CONTEXT* ct, const HOOKWORD argc);

	struct DebugRegister
	{
		HOOKWORD _dr0;
		HOOKWORD _dr1;
		HOOKWORD _dr2;
		HOOKWORD _dr3;

		enum DebugRegisterType : HOOKWORD
		{
			LG0 = 0, 
			LG1 = 2, 
			LG2 = 4, 
			LG3 = 6,

			LEGE = 8, 
			GD = 13,

			LEN0 = 18, 
			LEN1 = 22, 
			LEN2 = 26, 
			LEN3 = 30,

			RW0 = 16, 
			RW1 = 20, 
			RW2 = 24, 
			RW3 = 28,

			B0 = 1 << 0,
			B1 = 1 << 1, 
			B2 = 1 << 2, 
			B3 = 1 << 3,

			BD = 1 << 13, 
			BS = 1 << 14, 
			BT = 1 << 15
		};

		struct
		{
			HOOKWORD __dr6;
			operator HOOKWORD () const { return __dr6; }
			operator HOOKWORD& () { return __dr6; }
			void operator=(const HOOKWORD& dr6) { __dr6 = dr6; }
		}_dr6;

		struct
		{
			HOOKWORD __dr7;
			operator HOOKWORD () const { return __dr7; }
			operator HOOKWORD& () { return __dr7; }
			void operator=(const HOOKWORD& dr7) { __dr7 = dr7; }
		}_dr7;

		operator CONTEXT() const {
			CONTEXT context{};
			context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
			context.Dr0 = _dr0;
			context.Dr1 = _dr1;
			context.Dr2 = _dr2;
			context.Dr3 = _dr3;
			context.Dr6 = _dr6;
			context.Dr7 = _dr7;
			return context;
		}

		PVECTORED_EXCEPTION_HANDLER _fc_exception_handlers[5];

		DebugRegister() 
			:_dr0(0), _dr1(0), _dr2(0), _dr3(0), _dr6{ 0 }, _dr7{ 0 },_fc_exception_handlers{}
		{};
		DebugRegister(const CONTEXT& context) :
			_dr0(context.Dr0), 
			_dr1(context.Dr1), 
			_dr2(context.Dr2), 
			_dr3(context.Dr3), 
			_dr6{ context.Dr6 }, 
			_dr7{ context.Dr7 }, 
			_fc_exception_handlers{}
		{};

		bool GetDr6Bits(DebugRegisterType local) const;
		void SetDr7Bits(const DebugRegisterType local, unsigned char bits);
	};
	std::unique_ptr<std::stringstream> GetImportDirectory(const HMODULE hModule);
	std::unique_ptr<std::stringstream> GetExportDirectory(const HMODULE hModule);

	//INT3断点
	struct INT3Hook
	{
		static std::thread INT3HookStartThread;
		static std::mutex mtx;
		static std::map<LPVOID, PVECTORED_EXCEPTION_HANDLER> AddressToVEH;
		static LPVOID HandleVEH;

		LPBYTE Address;
		BYTE Original;
		INT3Hook(LPVOID Address, const PVECTORED_EXCEPTION_HANDLER backcall = nullptr);
		virtual ~INT3Hook();

		BOOL Hook(LPVOID Address = nullptr, const PVECTORED_EXCEPTION_HANDLER backcall = nullptr);
		BOOL UnHook()const;
	};

	//寄存器断点
	namespace RegisterHook
	{
		extern std::thread RegisterHookStartThread;
		extern LPVOID HandleVEH;
		extern DebugRegister global_context;
		extern std::mutex mtx;
		extern std::map<DWORD, DebugRegister> ThrIdToRegister;

		extern void Flush_GlobalDebug() noexcept;
		extern BOOL Insert_ThreadDebug(DWORD threadId);
	};
}