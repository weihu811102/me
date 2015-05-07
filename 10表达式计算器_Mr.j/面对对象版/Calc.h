#ifndef _CALC_H_
#define _CALC_H

#include <string>
#include "SymbolTable.h"
#include "scanner.h"
#include "Storage.h"
#include "FunctionTable.h"


class Parser;

class Calc
{
	friend class Parser;
public:
	Calc() : storage_(symTbl_), funTbl_(symTbl_)
	{

	}
private:
	Storage& GetStorage()
	{
		return storage_;
	}
	PtrFun GetFunction(unsigned int id)const
	{
		return funTbl_.GetFunction(id);
	}
	bool IsFunction(unsigned int id)const
	{
		return id < funTbl_.Size();
	}
	unsigned int FindSymbol(const std::string& str)const;
	unsigned int AddSymbol(const std::string& str);
	SymbolTable symTbl_;//不用引用& 声明周期由本类来管理 symTbl对象先构建
	FunctionTable funTbl_;//FunctionTable的构造依赖于SymbolTable 所以它需要在SymbolTable之后声明 先构造符号表
	Storage storage_;
};

#endif//_CALC_H_