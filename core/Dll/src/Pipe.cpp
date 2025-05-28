#include"Pipe.hpp"


#include <iostream>
namespace Mortis::Pipe
{
	bounded_queue<std::vector<char>> PipeExecutor::recvQueue{};
	bounded_queue<std::vector<char>> PipeExecutor::sendQueue{};

	AutoHandle<> PipeExecutor::recvPipeHandle = nullptr;
	AutoHandle<> PipeExecutor::sendPipeHandle = nullptr;

	std::jthread PipeExecutor::recvPipeThr{};
	std::jthread PipeExecutor::sendPipeThr{};

	PipeExecutor::PipeExecutor() 
	{
		recvPipeHandle = CreateNamedPipeW(LogPipeName, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE, 1, 0, 0, 0, nullptr);
		sendPipeHandle = CreateNamedPipeW(CtrlPipeName, PIPE_ACCESS_INBOUND, PIPE_READMODE_BYTE, 1, 0, 0, 0, nullptr);
		if (recvPipeHandle && sendPipeHandle) {
			std::cout << "Pipe Init success" << std::endl;
		}

		recvPipeThr = std::jthread([](std::stop_token st)
		{
			while (st.stop_requested() == false)
			{
				if (ConnectNamedPipe(recvPipeHandle, nullptr)) {
					std::cout << "LogPipe Client link success!!!" << std::endl;
				}
				else if (GetLastError() == ERROR_NO_DATA)
				{
					DisconnectNamedPipe(recvPipeHandle);
					std::cout << "LogPipe Client Disconnect" << std::endl;
					continue;
				}
				while (!recvQueue.empty())
				{
					trid = GetCurrentProcess();
					std::cout << "success to link LogServer!!!!" << std::endl;
					char buf[1024];
					buf[1023] = 0;
					DWORD len;
					while (ReadFile(LogPipeH, buf, 1023, &len, nullptr))
					{
						if (len == 1023 && buf[1022] != 0)
						{
							temp += buf;
							continue;
						}
						temp += buf;
						std::cout << temp;
						lock.lock();
						LogQueue.emplace(temp);
						lock.unlock();
						temp = "";
					}
				}
			}
		});

		sendPipeThr = std::jthread([](std::stop_token st) 
		{
			while (st.stop_requested() == false)
			{
				if (ConnectNamedPipe(sendPipeHandle, nullptr)) {
					std::cout << "CtrlPipe Client link success!!!" << std::endl;
				}
				else if (GetLastError() == ERROR_NO_DATA)
				{
					DisconnectNamedPipe(sendPipeHandle);
					std::cout << "CtrlPipe Client Disconnect" << std::endl;
					continue;
				}
				DWORD framelen, temp;
				while (ReadFile(sendPipeHandle, &framelen, sizeof(framelen), &temp, nullptr))
				{
					if (framelen == 0)continue;
					std::unique_ptr<char[]> buf(new char[framelen]);
					if (ReadFile(sendPipeHandle, buf.get(), framelen, &temp, nullptr) && framelen == temp)
					{
						std::cout << buf.get() << std::endl;
						sendQueue.emplace(CtrlContext{ framelen,std::move(buf) });
					}
				}
				if (GetLastError() == ERROR_NO_DATA)continue;
			}
		});
	}
}