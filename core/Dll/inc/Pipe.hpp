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

	class PipeContext {
		PipeContext(PipeContext&) = delete;
		PipeContext(PipeContext&& ) = delete;
		PipeContext& operator = (PipeContext&) = delete;
		PipeContext& operator = (PipeContext&&) = delete;
	protected:
		static AutoHandle<> LogPipeH;
		static AutoHandle<> CtrlPipeH;
		PipeContext() {

		}
		~PipeContext() {

		}
	public:
		static PipeContext& PipeInstance() {
			static PipeContext instance;
			return instance;
		}
	};




	struct PipeIO
	{
		static std::thread pipeInit;

		static std::queue<std::string> OutQueue;
		static std::queue<CtrlContext> InQueue;
		static std::mutex OutQueuemtx;
		static std::mutex InQueuemtx;

		static AutoHandle<> LogPipeH;
		static AutoHandle<> CtrlPipeH;

		inline const PipeIO& operator<<(auto&& str)const {

			ss << std::forward<decltype(str)>(str);
			std::unique_lock lockqueue(OutQueuemtx);
			std::unique_lock lockss(ssmtx);
			OutQueue.emplace(ss.str());
			ss.str("");
			ss.clear();
			return *this;

		}

		inline const PipeIO& operator>>(CtrlContext& cf) const;
	private:
		static std::stringstream ss;
		static std::mutex ssmtx;
	};
	extern const PipeIO pout, pin, io;
	extern const std::string pendl;
}