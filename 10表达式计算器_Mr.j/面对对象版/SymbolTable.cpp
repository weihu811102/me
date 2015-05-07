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

//STL: function object,functor �������� 
//STL�������:1.���� 2.�㷨 3.������(�㷨������������) 4.������ 5.�������� 6.�ڴ������
//��һ�������ʹ��������һ������ ���������������������
class IsEqualId
{
public:
	explicit IsEqualId(unsigned int id) : id_(id){}//���explicit����ת������ ֻ����ʾ����
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