#pragma once
#include<base_template.hpp>
#include<base_concept.hpp>

namespace Mortis
{

	template<class HandleType, class DeleteFunc, class Wrapper = BT::ScopeHandleWrapper<HandleType, DeleteFunc>>
		requires BC::CanCall<typename DeleteFunc::value_type, typename Wrapper::Ptr, typename Wrapper::SecPtr>
	class ScopeHandle
	{
		using unique_ptr = Wrapper::unique_ptr;
		using Ptr = Wrapper::Ptr;
		using SecPtr = Wrapper::SecPtr;

		unique_ptr _ptr;
	public:
		ScopeHandle() noexcept {}

		ScopeHandle(ScopeHandle& handle) = delete;

		ScopeHandle(ScopeHandle&& handle) noexcept
			: _ptr(handle.release()) {
		}

		ScopeHandle(Ptr ptr) noexcept
			: _ptr(ptr) {
		}

		ScopeHandle& operator=(Ptr ptr) noexcept {
			_ptr.reset(ptr); 
			return *this;
		}

		ScopeHandle& operator=(ScopeHandle&& handle) noexcept {
			_ptr.reset(handle.release());
			return *this;
		}

		operator const Ptr() const noexcept {
			return _ptr.get();
		}
		operator Ptr& () noexcept {
			return *reinterpret_cast<SecPtr>(this);
		}
		operator bool() const noexcept {
			return _ptr.get() != nullptr;
		}

		SecPtr operator&() {
			return reinterpret_cast<SecPtr>(this);
		}

		Ptr operator->() const noexcept {
			return _ptr.get();
		}

		void reset(Ptr ptr = nullptr) noexcept {
			_ptr.reset(ptr);
		}

		Ptr release() noexcept {
			return _ptr.release();
		}

		Ptr get() const noexcept {
			return _ptr.get();
		}
	};

	template<class HandleType = HANDLE, class DeleteFunc = BT::StaticFunctorWrapper<CloseHandle>, class Wrapper>
		requires BC::CanCall<typename DeleteFunc::value_type, typename Wrapper::Ptr, typename Wrapper::SecPtr>
	class ScopeHandle;
}