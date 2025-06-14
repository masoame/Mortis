#include<ScopeHandle.hpp>
#include <bounded_queue.hpp>
namespace Mortis::Pipe
{
	constexpr const auto& LogPipeName = L"\\\\.\\pipe\\LogPipe";
	constexpr const auto& CtrlPipeName = L"\\\\.\\pipe\\CtrlPipe";

	class PipeExecutor 
	{
		PipeExecutor(PipeExecutor&) = delete;
		PipeExecutor(PipeExecutor&&) = delete;
		PipeExecutor& operator = (PipeExecutor&) = delete;
		PipeExecutor& operator = (PipeExecutor&&) = delete;
	protected:
		ScopeHandle<> _recv_pipe_handle;
		ScopeHandle<> _send_pipe_handle;

		bounded_queue<std::vector<char>> _recv_queue;
		bounded_queue<std::vector<char>> _send_queue;

		std::jthread _recvThr;
		std::jthread _sendThr;

		PipeExecutor();
		~PipeExecutor();
	public:

		static PipeExecutor& Instance() {
			static PipeExecutor instance;
			return instance;
		}
	};
}