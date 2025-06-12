#include"PipeExecutor.hpp"
#include <iostream>
#include<chrono>
#include<thread>
namespace Mortis::Pipe
{	
	using namespace std::chrono_literals;

	PipeExecutor::PipeExecutor()
	{
		_sendPipeH = CreateNamedPipeW(LogPipeName, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE, 1, 0, 0, 0, nullptr);
		_recvPipeH = CreateNamedPipeW(CtrlPipeName, PIPE_ACCESS_INBOUND, PIPE_READMODE_BYTE, 1, 0, 0, 0, nullptr);
		_sendThr = std::jthread([this](std::stop_token st)
		{
			while (st.stop_requested() == false)
			{
				if (ConnectNamedPipe(_sendPipeH, nullptr)){
					std::cout << "LogPipe Client link success!!!" << std::endl;
				}else if (GetLastError() == ERROR_NO_DATA){
					DisconnectNamedPipe(_sendPipeH);
					std::cout << "LogPipe Client Disconnect" << std::endl;
					continue;
				}
				while (st.stop_requested() == false) {
					const auto sendDataOptional = _sendQueue.pop_for(2s);
					if (sendDataOptional.has_value() == false) {
						continue;
					}
					const auto& sendData = sendDataOptional.value();
					DWORD sendRealLen = 0;
					if (WriteFile(_sendPipeH, sendData.data(), static_cast<DWORD>(sendData.size()), &sendRealLen, nullptr) == FALSE) {
						throw std::runtime_error("LogPipe WriteFile failed");
					}
					if (sendRealLen!= sendData.size()) {
						throw std::runtime_error("LogPipe WriteFile failed");
					}
				}
			}
		});

		_recvThr = std::jthread([this](std::stop_token st)
		{
			while (st.stop_requested() == false)
			{
				if (ConnectNamedPipe(_recvPipeH, nullptr)) {
					std::cout << "LogPipe Client link success!!!" << std::endl;
				}else if (GetLastError() == ERROR_NO_DATA) {
					DisconnectNamedPipe(_recvPipeH);
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