#include <algorithm>
#include "SymbolTable.h"
#include "Exception.h"
#include "DebugNew.h"

unsigned int SymbolTable::Add(const std::string& str)
{
	dictionary_[str] = curId_;
	return curId_++;//return curId_
}
unsigned int SymbolTable::Find(const std::string& str) const
{
	std::map<const std::string, unsigned int>::const_iterator it;
	it = dictionary_.find(str);
	if(it != dictionary_.end())
		return it->second;
	else
		return IDNOTFOUND;
}
void SymbolTable::Clear()
{
	dictionary_.clear();
	curId_ = 0;
}

//STL: function object,functor 函数对象 
//STL六大组件:1.容器 2.算法 3.迭代器(算法和容器的桥梁) 4.适配器 5.函数对象 6.内存分配器
//让一个类对象使用起来像一个函数 本质是重载了括号运算符
class IsEqualId
{
public:
	explicit IsEqualId(unsigned int id) : id_(id){}//添加explicit避免转换构造 只能显示构造
	bool operator()(const std::pair<const std::string, unsigned int>& it)const
	{
		return id_ == it.second;
	}
private:
	unsigned int id_;
};
std::string SymbolTable::GetSymbolName(unsigned int id) const
{
	std::map<const std::string, unsigned int>::const_iterator it;
	it = find_if(dictionary_.begin(), dictionary_.end(), IsEqualId(id));

	if(dictionary_.end() == it)
	{
		throw Exception("Internal error: missing entry in symbol table.");
	}

	return it->first;
}