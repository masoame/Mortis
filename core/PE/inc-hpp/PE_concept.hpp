#pragma once
#include <base_concept.hpp>

namespace Mortis::BC
{
	template<typename T1, typename T2>
	concept SearchProcessConcept = requires (T1 t, T2 p) {
		requires BC::IsString<T1>;
		requires BC::IsSameType<T2, PROCESSENTRY32, PROCESSENTRY32W>;
	{ p.szExeFile } -> BC::ArrayElementTypeIsSame<T1>;
	};

	template<typename T1, typename T2>
	concept SearchModuleConcept = requires (T1 t, T2 p) {
		requires BC::IsString<T1>;
		requires BC::IsSameType<T2, MODULEENTRY32, MODULEENTRY32W>;
	{ p.szExePath } -> BC::ArrayElementTypeIsSame<T1>;
	};
}