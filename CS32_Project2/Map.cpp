#include "Map.h"
#include <iostream>

bool combine(const Map& m1, const Map& m2, Map& result) {
	result.~Map();
	bool returnValue = true;
	KeyType add_key;
	ValueType add_value, comp_value;
	for (int i = 0; i < m1.size(); i++) {
		m1.get(i, add_key, add_value);
		if (m2.contains(add_key)) {
			m2.get(add_key, comp_value);
			if (comp_value == add_value)
				result.insert(add_key, add_value);
			else
				returnValue = false;
		}
		else
			result.insert(add_key, add_value);
	}
	for (int i = 0; i < m1.size(); i++) {
		m2.get(i, add_key, add_value);
		if (m1.contains(add_key) && !result.contains(add_key)) {
			m1.get(add_key, comp_value);
			if (comp_value == add_value)
				result.insert(add_key, add_value);
			else
				returnValue = false;
		}
		else
			result.insert(add_key, add_value);
	}
	return returnValue;
}

void subtract(const Map& m1, const Map& m2, Map& result) {
	result.~Map();
	KeyType add_key;
	ValueType add_value;
	for (int i = 0; i < m1.size(); i++) {
		m1.get(i, add_key, add_value);
		if (!m2.contains(add_key))
			result.insert(add_key, add_value);
	}
}

Map::Map() {
	head = nullptr;
	tail = nullptr;
}

Map::~Map() {
	Node *p = head;
	while (p != nullptr) {
		Node *del = p;
		p = p->next;
		delete del;
	}
	head = nullptr;
	tail = nullptr;
}

Map::Map(const Map &src) {
	Node *p = src.head;
	while (p != nullptr) {
		insert(p->m_key, p->m_value);
		p = p->next;
	}
}

Map &Map::operator=(const Map &src) { //IFFY
	Node *target = head;
	Node *source = src.head;
	if (src.head == nullptr) {
		head = nullptr;
		tail = nullptr;
		return (*this);
	}
	while (source != nullptr) {
		if (target == nullptr) {
			Node *add = new Node;
			add->m_key = source->m_key;
			add->m_value = source->m_value;
			if (head == nullptr) {
				head = add;
				tail = add;
			}
			else {
				tail->next = add;
				add->next = nullptr;
				add->prev = tail;
				tail = add;
			}
		} 
		else {
			target->m_key = source->m_key;
			target->m_value = source->m_value;
			target = target->next;
		}
		source = source->next;
	} 
	if (target != nullptr) {
		tail = target->prev;
		tail->next = nullptr;
	}
	Node *temp; 
	while (target != nullptr) {
		temp = target->next;
		delete target;
		target = temp;
	}
	return (*this);
}

bool Map::empty() const {
	if (head == nullptr)
		return true;
	else
		return false;
}

int Map::size() const {
	Node *p = head;
	int count = 0;
	while (p != nullptr) {
		count++;
		p = p->next;
	}
	return count;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
	Node *p = head;
	if (head == nullptr) {
		Node *add = new Node;
		add->m_key = key;
		add->m_value = value;
		add->next = nullptr;
		add->prev = nullptr;
		head = add;
		tail = add;
		return true;
	}
	while (p != nullptr) {
		if (p->m_key == key)
			break;
		p = p->next;
	}
	if (p == nullptr) {
		Node *add = new Node;
		tail->next = add;
		add->m_key = key;
		add->m_value = value;
		add->next = nullptr;
		add->prev = tail;
		tail = add;
		return true;
	}
	return false;
}

bool Map::update(const KeyType& key, const ValueType& value) {
	Node *p = head;
	while (p != nullptr) {
		if (p->m_key == key)
			break;
		p = p->next;
	}
	if (p == nullptr)
		return false;
	else {
		p->m_value = value;
		return true;
	}
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
	Node *p = head;
	if (head == nullptr) {
		Node *add = new Node;
		add->m_key = key;
		add->m_value = value;
		add->next = nullptr;
		add->prev = nullptr;
		head = add;
		tail = add;
		return true;
	}
	while (p != nullptr) {
		if (p->m_key == key) {
			p->m_value = value;
			return true;
		}
		p = p->next;
	}
	if (p == nullptr) {
		Node *add = new Node;
		tail->next = add;
		add->m_key = key;
		add->m_value = value;
		add->next = nullptr;
		add->prev = tail;
		tail = add;
		return true;
	}
	return true;
}

bool Map::erase(const KeyType& key) {
	Node *p = head;
	if (head == nullptr)
		return false;
	else if (head->m_key == key) {
		Node *del = head;
		head = head->next;
		head->prev = nullptr;
		delete del;
		return true;
	}
	else {
		while (p->next != nullptr) {
			if (p->next->m_key == key)
				break;
			p = p->next;
		}
		if (p->next != nullptr) {
			Node *del = p->next;
			p->next = del->next;
			del->next->prev = p;
			delete del;
			return true;
		}
	}
	return false;
}

bool Map::contains(const KeyType& key) const {
	Node *p = head;
	while (p != nullptr) {
		if (p->m_key == key)
			return true;
		p = p->next;
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
	Node *p = head;
	while (p != nullptr) {
		if (p->m_key == key) {
			value = p->m_value;
			return true;
		}
		p = p->next;
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
	int count = 0;
	if (i < size()) {
		Node *p = head;
		while (p != nullptr) {
			if (count != i) {
				count++;
				p = p->next;
			}
			else {
				key = p->m_key;
				value = p->m_value;
				return true;
			}
		}
	}
	return false;
}

void Map::swap(Map& other) {
	Node *temp_head;
	Node *temp_tail;
	temp_head = head;
	temp_tail = tail;
	head = other.head;
	tail = other.tail;
	other.head = temp_head;
	other.tail = temp_tail;
}

void Map::dump() const {
	Node *p = head;
	while (p != nullptr) {
		std::cout << p->m_key << " " << p->m_value << std::endl;
		p = p->next;
	}
}