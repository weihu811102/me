#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <map>
#include <string>
#include <algorithm>

class SymbolTable
{
public:
	enum {IDNOTFOUND = 0xffffffff};

	SymbolTable() : curId_(0){}
	unsigned int Add(const std::string& str);
	unsigned int Find(const std::string& str) const;
	void Clear();
private:
	std::map<const std::string, unsigned int> dictionary_;
	unsigned int curId_;
	std::string GetSymbolName(unsigned int id) const;
};

#endif//_SYMBOL_TABLE_H_