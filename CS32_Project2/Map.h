#ifndef MAP_H
#define MAP_H
#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
public:
	Map();
	~Map();
	Map(const Map &src);
	Map &operator=(const Map &src);
	bool empty() const;
	int size() const;
	bool insert(const KeyType& key, const ValueType& value);
	bool update(const KeyType& key, const ValueType& value);
	bool insertOrUpdate(const KeyType& key, const ValueType& value);
	bool erase(const KeyType& key);
	bool contains(const KeyType& key) const;
	bool get(const KeyType& key, ValueType& value) const;
	bool get(int i, KeyType& key, ValueType& value) const;
	void swap(Map& other);
	void dump() const;
private:
	struct Node {
		KeyType m_key;
		ValueType m_value;
		Node *next;
		Node *prev;
	};
	Node *head;
	Node *tail;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif