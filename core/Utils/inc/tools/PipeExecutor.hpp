#include <utils.hpp>
namespace Mortis::Pipe
{
	constexpr const auto& LogPipeName = L"\\\\.\\pipe\\LogPipe";
	constexpr const auto& CtrlPipeName = L"\\\\.\\pipe\\CtrlPipe";

	class PipeExecutor 
	{
		PipeExecutor(PipeExecutor&) = delete;
		PipeExecutor(PipeExecutor&& ) = delete;
		PipeExecutor& operator = (PipeExecutor&) = delete;
		PipeExecutor& operator = (PipeExecutor&&) = delete;
	protected:
		ScopeHandle<> _recvPipeH;
		ScopeHandle<> _sendPipeH;

		bounded_queue<std::vector<char>> _recvQueue;
		bounded_queue<std::vector<char>> _sendQueue;

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