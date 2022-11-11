#pragma once
#include <memory>
#include <list>
#include "Object.hpp"

class Field
{
	using Value = Object;

	static inline std::list<std::shared_ptr<Value>> list;

public:
	static std::weak_ptr<Value> get(int pos, int range, bool force = false);
	static std::list<std::shared_ptr<Value>>::iterator getIterator(int pos, int range, bool force = false);
	static std::list<std::shared_ptr<Value>>::iterator getIterator(int pos);
	//static std::list<std::shared_ptr<Value>>::iterator getIterator(const std::shared_ptr<Value>& t);
	static std::shared_ptr<Value> set(Value* t);
	static void sort() { list.sort([](const std::shared_ptr<Value>& l, const std::shared_ptr<Value>& r) {return l->pos < r->pos; }); }
	static void reset() { list.clear(); }
	static std::list<std::shared_ptr<Value>>::iterator erase(std::list<std::shared_ptr<Value>>::iterator& t) { return list.erase(t); }
	static std::list<std::shared_ptr<Value>>::iterator begin() { return list.begin(); }
	static std::list<std::shared_ptr<Value>>::const_iterator cend() { return list.cend(); }
};

