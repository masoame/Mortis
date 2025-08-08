#include<PipeExecutor.hpp>
#include<thread>
#include<iostream>

using namespace std::chrono_literals;
namespace Mortis::Pipe
{	
	PipeExecutor::PipeExecutor()
	{
		_send_pipe_handle = CreateNamedPipeW(LogPipeName, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE, 1, 0, 0, 0, nullptr);
		_recv_pipe_handle = CreateNamedPipeW(CtrlPipeName, PIPE_ACCESS_INBOUND, PIPE_READMODE_BYTE, 1, 0, 0, 0, nullptr);
		_send_thr = std::jthread([this](std::stop_token st)
		{
			while (st.stop_requested() == false)
			{
				if (ConnectNamedPipe(_send_pipe_handle, nullptr)){
					std::cout << "LogPipe Client link success!!!" << std::endl;
				}else if (GetLastError() == ERROR_NO_DATA){
					DisconnectNamedPipe(_send_pipe_handle);
					std::cout << "LogPipe Client Disconnect" << std::endl;
					continue;
				}
				while (st.stop_requested() == false) {
					const auto sendDataOptional = _send_queue.pop_for(2s);
					if (sendDataOptional.has_value() == false) {
						continue;
					}
					const auto& sendData = sendDataOptional.value();
					DWORD sendRealLen = 0;
					if (WriteFile(_send_pipe_handle, sendData.data(), static_cast<DWORD>(sendData.size()), &sendRealLen, nullptr) == FALSE) {
						throw std::runtime_error("LogPipe WriteFile failed");
					}
					if (sendRealLen!= sendData.size()) {
						throw std::runtime_error("LogPipe WriteFile failed");
					}
				}
			}
		});

		_recv_thr = std::jthread([this](std::stop_token st)
		{
			while (st.stop_requested() == false)
			{
				if (ConnectNamedPipe(_recv_pipe_handle, nullptr)) {
					std::cout << "LogPipe Client link success!!!" << std::endl;
				}else if (GetLastError() == ERROR_NO_DATA) {
					DisconnectNamedPipe(_recv_pipe_handle);
					std::cout << "LogPipe Client Disconnect" << std::endl;
					continue;
				}
			}
		});
	}

	PipeExecutor::~PipeExecutor()
	{

	}
}