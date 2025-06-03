#include"Pipe.hpp"
#include <iostream>
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

	//std::queue<std::string> PipeIO::OutQueue;
	//std::queue<CtrlContext> PipeIO::InQueue;
	//std::mutex PipeIO::OutQueuemtx;
	//std::mutex PipeIO::InQueuemtx;

	//AutoHandle<> PipeIO::LogPipeH{};
	//AutoHandle<> PipeIO::CtrlPipeH{};
	//std::stringstream PipeIO::ss{};
	//std::mutex PipeIO::ssmtx;

	//const PipeIO pout, pin, io;
	//const std::string pendl("\n");
	//std::thread PipeIO::pipeInit([]
	//{
	//	LogPipeH = CreateNamedPipeW(LogPipeName, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE, 1, 0, 0, 0, nullptr);
	//	CtrlPipeH = CreateNamedPipeW(CtrlPipeName, PIPE_ACCESS_INBOUND, PIPE_READMODE_BYTE, 1, 0, 0, 0, nullptr);

	//	if (LogPipeH) {
	//		std::jthread([](std::stop_token st)
	//		{
	//			if (LogPipeH) {
	//				std::cout << "LogPipe open success" << std::endl;
	//			}
	//			std::unique_lock lock(PipeIO::OutQueuemtx, std::defer_lock);
	//			while (st.stop_requested() == false)
	//			{
	//				if (ConnectNamedPipe(LogPipeH, nullptr)) {
	//					std::cout << "LogPipe Client link success!!!" << std::endl;
	//				}
	//				else if (GetLastError() == ERROR_NO_DATA)
	//				{
	//					DisconnectNamedPipe(LogPipeH);
	//					std::cout << "LogPipe Client Disconnect" << std::endl;
	//					continue;
	//				}
	//				size_t len = OutQueue.size();
	//				for (int i = 0; i != len; i++)
	//				{
	//					lock.lock();
	//					std::string& str = OutQueue.front();
	//					lock.unlock();
	//					DWORD temp;
	//					if (WriteFile(LogPipeH, str.c_str(), (DWORD)str.size() + 1, &temp, nullptr))
	//					{
	//						lock.lock();
	//						OutQueue.pop();
	//						lock.unlock();
	//					}
	//					else if (GetLastError() == ERROR_NO_DATA)  break;
	//				}
	//				Sleep(10);
	//			}
	//		}).detach();
	//	}
	//	if (CtrlPipeH)
	//		std::jthread([](std::stop_token st)
	//			{
	//				if (CtrlPipeH)std::cout << "CtrlPipe open success" << std::endl;
	//				std::unique_lock lock(PipeIO::InQueuemtx, std::defer_lock);
	//				while (st.stop_requested() == false)
	//				{
	//					if (ConnectNamedPipe(CtrlPipeH, nullptr))std::cout << "CtrlPipe Client link success!!!" << std::endl;
	//					else if (GetLastError() == ERROR_NO_DATA)
	//					{
	//						DisconnectNamedPipe(CtrlPipeH);
	//						std::cout << "CtrlPipe Client Disconnect" << std::endl;
	//						continue;
	//					}
	//					DWORD framelen, temp;
	//					while (ReadFile(CtrlPipeH, &framelen, sizeof(framelen), &temp, nullptr))
	//					{
	//						if (framelen == 0)continue;
	//						std::unique_ptr<char[]> buf(new char[framelen]);
	//						if (ReadFile(CtrlPipeH, buf.get(), framelen, &temp, nullptr) && framelen == temp)
	//						{
	//							std::cout << buf.get() << std::endl;
	//							lock.lock();
	//							InQueue.emplace(CtrlContext{ framelen,std::move(buf) });
	//							lock.unlock();
	//						}
	//					}
	//					if (GetLastError() == ERROR_NO_DATA)continue;
	//				}
	//			}).detach();
	//	pipeInit.detach();
	//});


	//const PipeIO& PipeIO::operator>>(CtrlContext& cf)const
	//{
	//	std::unique_lock lock(InQueuemtx, std::try_to_lock);
	//	cf = std::move(InQueue.front());
	//	InQueue.pop();
	//	return *this;
	//}
	//{
	//	std::unique_lock lock(InQueuemtx, std::try_to_lock);
	//	cf = std::move(InQueue.front());
	//	InQueue.pop();
	//	return *this;
	//}
}