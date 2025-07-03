#pragma once
#include<utils.hpp>

namespace Mortis
{
	template<typename ValueType, typename FirstKey, typename... Keys>
	class MutualMap : public MutualMap<ValueType, Keys...>
	{
		std::map<FirstKey, std::weak_ptr<ValueType>> _map;

		template<typename ValueType, typename FirstKey>
		bool insert(std::shared_ptr<ValueType>&& value, FirstKey&& firstKey) {
			return _map.insert(std::make_pair( std::forward<FirstKey>(firstKey) , std::forward<ValueType>(value) )) != _map.cend();
		} 

	public:
		template<typename KeyType>
		auto find(KeyType&& key) {
			return _map.find(std::forward<KeyType>(key));
		}

		template<typename ValueType, typename FirstKey, typename... Keys>
		bool insert(ValueType&& value, FirstKey&& firstKey, Keys&& ...keys) {
			auto value_ptr = std::make_shared(value);
			return insert(value_ptr, firstKey) && (insert(value_ptr, keys) && ...) ;
		}
	};

	template<typename ValueType,typename FirstKey>
	class MutualMap<ValueType, FirstKey>
	{
		std::set<std::shared_ptr<ValueType>> _values;
		std::map<FirstKey, std::weak_ptr<ValueType>> _map;
	public:

		template<typename KeyType>
		auto find(KeyType&& key) {
			return _map.find(std::forward<KeyType>(key));
		}


		template<typename ValueType, typename FirstKey>
		bool insert(std::shared_ptr<ValueType>&& value, FirstKey&& firstKey) {
			return (_map.insert({ firstKey , value }) != _map.cend()) && (_values.insert(std::make_shared(value)) != _values.cend());
		}

	};
}