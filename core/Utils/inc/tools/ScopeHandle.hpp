#pragma once
#include<utils.hpp>
#include<base_template.hpp>
#include<base_concept.hpp>

namespace Mortis
{
	template<typename HandleType, typename DeleteFunc>
	concept ScopeHandleConcept = requires(HandleType handle, DeleteFunc func) {
		requires BC::NotConst<HandleType>&& BC::NotRef<HandleType>&& BC::NotConst<HandleType>;
		requires BC::IsStaticFunctor<DeleteFunc>;
		requires BC::CanCall<typename DeleteFunc::value_type, 
		std::conditional_t<std::is_pointer_v<HandleType>, HandleType, HandleType*>, 
			std::conditional_t<std::is_pointer_v<HandleType>, HandleType*, HandleType**>>;
	};

	template<typename HandleType, typename DeleteFunc>
		requires ScopeHandleConcept<HandleType, DeleteFunc>
	struct ScopeHandleWrapper;

	template<class HandleType, class DeleteFunc>
	class ScopeHandle : public ScopeHandleWrapper<HandleType, DeleteFunc>
	{
		using unique_ptr = ScopeHandleWrapper<HandleType, DeleteFunc>::unique_ptr;
		using Ptr = ScopeHandleWrapper<HandleType, DeleteFunc>::Ptr;
		using SecPtr = ScopeHandleWrapper<HandleType, DeleteFunc>::SecPtr;

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

		template<typename ...Args>
		static ScopeHandle<HandleType, DeleteFunc> New(Args&& ... agrs) {
			return  ScopeHandle<HandleType, DeleteFunc>(new HandleType(std::forward<Args>(agrs)...));
		}
	};

	template<typename HandleType>
	class ScopeHandle<HandleType, void> : public std::unique_ptr<HandleType> {

	};

	template<class HandleType = HANDLE, class DeleteFunc = BT::StaticFunctorWrapper<CloseHandle>>
	class ScopeHandle;

	template<typename HandleType, typename DeleteFunc>
		requires ScopeHandleConcept<HandleType, DeleteFunc>
	struct ScopeHandleWrapper
	{
		constexpr static bool isPtr = std::is_pointer_v<HandleType>;
		using UniqueType = std::conditional_t<isPtr, std::remove_pointer_t<HandleType>, HandleType>;
		using Ptr = UniqueType*;
		using SecPtr = Ptr*;

		constexpr static bool isSecPtr = not std::is_invocable_v<typename DeleteFunc::value_type, Ptr>;

		struct DeletePrimaryPtr {
			void operator()(void* ptr) {
				DeleteFunc()(static_cast<Ptr>(ptr));
			}
		};
		struct DeleteSecPtr {
			void operator()(void* ptr) {
				DeleteFunc()(reinterpret_cast<SecPtr>(&ptr));
			}
		};

		using DeletePtr = std::conditional_t<isSecPtr, DeleteSecPtr, DeletePrimaryPtr>;
		using unique_ptr = std::unique_ptr<UniqueType, DeletePtr>;
	};
}