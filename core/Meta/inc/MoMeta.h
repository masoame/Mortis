#pragma once
#include "utils.hpp"

template<typename Object>
class MoBaseMate
{
	constexpr static std::string get_name(const Object& obj){
		return typeid(obj).name();
	}
}


class MoMeta
{
	template<typename T>
	constexpr static std::string get_name(const T& );
}