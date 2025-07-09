#pragma once
#include<type_traits>
#include<string>
#include<string_view>
namespace Mortis 
{
	enum class CodePage : unsigned int
	{
		UTF_8 = CP_UTF8,
		UTF_16 = 1200,
		UTF_16BE = 1201,
		UTF_32 = 12000,

		GB2312 = 936,
		SHIFT_JIS = 932,
	};
}
