#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>
#include <string>
#include <vector>
#include <shared_mutex>

template <typename KeyType, typename DataType>
struct Entity {
	int id;
	DataType data;
	// 其他成员...
};

template <typename KeyType, typename DataType>
class EntityManager {
public:
	EntityManager();
	~EntityManager();

	DataType& getEntity(const KeyType& key);
	void cleanupExpired(const std::vector<KeyType>& validKeys);
	size_t size() const;

	class Iterator {
	public:
		Iterator(typename std::map<KeyType, Entity<KeyType, DataType>>::iterator it, std::shared_mutex& mutex)
			: iterator(it), mutex_(mutex) {
			std::shared_lock<std::shared_mutex> lock(mutex_); // 读锁
		}

		Iterator& operator++() {
			++iterator;
			return *this;
		}

		bool operator!=(const Iterator& other) const {
			return iterator != other.iterator;
		}

		DataType& operator*() const {
			return iterator->second.data;
		}

	private:
		typename std::map<KeyType, Entity<KeyType, DataType>>::iterator iterator;
		std::shared_mutex& mutex_; // 引用到外部的读写锁
	};

	Iterator begin();
	Iterator end();

private:
	std::map<KeyType, Entity<KeyType, DataType>> entityMap;
	mutable std::shared_mutex mutex_; // 读写锁，注意这里使用了 mutable 以允许在 const 成员函数中上锁
};

template <typename KeyType, typename DataType>
EntityManager<KeyType, DataType>::EntityManager() {
	// 构造函数
}

template <typename KeyType, typename DataType>
EntityManager<KeyType, DataType>::~EntityManager() {
	// 析构函数
}
template <typename KeyType, typename DataType>
DataType& EntityManager<KeyType, DataType>::getEntity(const KeyType& key) {
	auto it = entityMap.find(key);
	if (it == entityMap.end()) {
		// 如果 key 不存在于 map 中，创建新的 Entity 并插入
		Entity<KeyType, DataType> newEntity;
		newEntity.id = entityMap.size() + 1; // 可以根据实际情况设定 ID
		newEntity.data = DataType(); // 初始化默认值
		it = entityMap.insert({ key, newEntity }).first;
	}
	return it->second.data;
}

template <typename KeyType, typename DataType>
void EntityManager<KeyType, DataType>::cleanupExpired(const std::vector<KeyType>& validKeys) {
	std::unique_lock<std::shared_mutex> lock(mutex_);

	if (validKeys.empty()) {
		entityMap.clear();
		return;
	}

	std::vector<KeyType> keysToDelete;
	for (auto it = entityMap.begin(); it != entityMap.end(); ) {
		if (std::find(validKeys.begin(), validKeys.end(), it->first) == validKeys.end()) {
			keysToDelete.push_back(it->first);
			it = entityMap.erase(it);
		}
		else {
			++it;
		}
	}
}

template <typename KeyType, typename DataType>
size_t EntityManager<KeyType, DataType>::size() const {
	std::shared_lock<std::shared_mutex> lock(mutex_); // 读锁
	return entityMap.size();
}

template <typename KeyType, typename DataType>
typename EntityManager<KeyType, DataType>::Iterator EntityManager<KeyType, DataType>::begin() {
	return Iterator(entityMap.begin(), mutex_); // 将读写锁传递给迭代器
}

template <typename KeyType, typename DataType>
typename EntityManager<KeyType, DataType>::Iterator EntityManager<KeyType, DataType>::end() {
	return Iterator(entityMap.end(), mutex_); // 将读写锁传递给迭代器
}

#endif // ENTITYMANAGER_H
