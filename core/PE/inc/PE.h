#pragma once
#include<sstream>
#include <utils.hpp>
#include <asm_types.hpp>
#include <PE_template.hpp>

namespace Mortis::PE
{

	template<typename HasIdType>
		requires BC::HasType<HasIdType, PROCESSENTRY32* , PROCESSENTRY32W*, std::unique_ptr<PROCESSENTRY32>, std::unique_ptr<PROCESSENTRY32W>, DWORD>
	auto OpenProcessHandle(const HasIdType& hasIdType, DWORD dwDesiredAccess = PROCESS_ALL_ACCESS, BOOL bInheritHandle = FALSE)
		-> ScopeHandle<>;

	//搜索进程
	template<typename ProcessNameType, typename UseWrapper = BT::SearchProcessWrapper<ProcessNameType>, typename PROCESSENTRY32Wrapper = UseWrapper::TYPE::PROCESSENTRY32Wrapper>
		requires BC::SearchProcessConcept<ProcessNameType, PROCESSENTRY32Wrapper>
	auto SearchProcess(const ProcessNameType& processName) 
		-> std::unique_ptr<PROCESSENTRY32Wrapper>;


	//搜索模块
	template<typename ModuleNameType, typename UseWrapper = BT::SearchModuleWrapper<ModuleNameType>, typename MODULEENTRY32Wrapper = UseWrapper::TYPE::MODULEENTRY32Wrapper>
		requires BC::SearchModuleConcept<ModuleNameType, MODULEENTRY32Wrapper>
	auto SearchModule(DWORD th32ProcessID, const ModuleNameType& ModuleName)
		->std::unique_ptr<MODULEENTRY32Wrapper>;

	//进程信息
	template<typename T, typename UseWrapper = BT::ProcessInfoWrapper<T>, typename PROCESSENTRY32Wrapper = UseWrapper::TYPE::PROCESSENTRY32Wrapper>
	auto ProcessInfo()
		-> std::vector<PROCESSENTRY32Wrapper>;

	//模块信息
	template<typename T, typename UseWrapper = BT::ModuleInfoWrapper<T>, typename MODULEENTRY32Wrapper = UseWrapper::MODULEENTRY32Wrapper>
	auto ModuleInfo(DWORD th32ProcessID)
		-> std::vector<MODULEENTRY32Wrapper>;

	//获得DOS头和NT头
	auto GetFileHeader(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::shared_ptr<std::pair<IMAGE_DOS_HEADER, IMAGE_NT_HEADERS>>;

}

#include<PE.hpp>
#include<ExportTable.hpp>
#include<ImportTable.hpp>