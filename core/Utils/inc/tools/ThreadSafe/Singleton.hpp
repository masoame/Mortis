#pragma once

#include<utils.hpp>
namespace Mortis
{
	template<typename Object>
	class Singleton 
	{
	private:
		Singleton() = default;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator=(Singleton&&) = delete;
	public:
		static Object& Instance() {
			static Object instance{};
			return instance;
		}
	};
}