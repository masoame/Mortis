#include "common.hpp"
namespace Mortis::Pipe
{
	constexpr const auto& LogPipeName = L"\\\\.\\pipe\\LogPipe";
	constexpr const auto& CtrlPipeName = L"\\\\.\\pipe\\CtrlPipe";

	struct CtrlContext
	{
		size_t _len;
		std::unique_ptr<char[]> buf;
		CtrlContext(size_t ilen, std::unique_ptr<char[]>&& ibuf)noexcept :_len(ilen), buf(ibuf.release()) {};
		CtrlContext(CtrlContext&& cf)noexcept :_len(cf._len), buf(cf.buf.release()) {};
		void operator=(CtrlContext&& cf)noexcept { _len = cf._len; buf = std::move(cf.buf); };
	};

	struct PIPE_OVERLAPPED : public OVERLAPPED
	{

	};

	class PipeExecutor {
	protected:
		PipeExecutor(PipeExecutor&) = delete;
		PipeExecutor(PipeExecutor&& ) = delete;
		PipeExecutor& operator = (PipeExecutor&) = delete;
		PipeExecutor& operator = (PipeExecutor&&) = delete;

		static bounded_queue<std::vector<char>> recvQueue;
		static bounded_queue<std::vector<char>> sendQueue;

		static AutoHandle<> recvPipeHandle;
		static AutoHandle<> sendPipeHandle;

		static std::jthread recvPipeThr;
		static std::jthread sendPipeThr;

		PipeExecutor();
		~PipeExecutor();
	public:
		static PipeExecutor& PipeInstance() {
			static PipeExecutor instance;
			return instance;
		}
	};

}