#ifndef _FUNCTIONTABLE_H_
#define _FUNCTIONTABLE_H_

class SymbolTable;

typedef double (*PtrFun)(double);

class FunctionTable
{
public:
	FunctionTable(SymbolTable& tbl);
	~FunctionTable(void);

	void Init(SymbolTable& tbl);

	unsigned int Size()const
	{
		return size_;
	}
	PtrFun GetFunction(unsigned int id)const
	{
		return pFuns_[id];
	}
private:
	PtrFun* pFuns_;
	unsigned int size_;
};

#endif//_FUNCTIONTABLE_H_