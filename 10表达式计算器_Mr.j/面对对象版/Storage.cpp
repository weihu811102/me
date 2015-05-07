#include <cmath>
#include <cassert>
#include <iostream>
#include "Storage.h"
#include "SymbolTable.h"

Storage::Storage(SymbolTable& tbl)
{
	AddConstants(tbl);
}
void Storage::Clear()
{
	cells_.clear();
	inits_.clear();
}
bool Storage::IsInit(unsigned int id)const
{
	return id < cells_.size() && inits_[id];//id 不能超过cells_的最大值 并且是否初始化
}
void Storage::AddConstants(SymbolTable& tbl)
{
	std::cout<<"variable list"<<std::endl;
	unsigned int id = tbl.Add("e");
	AddValue(id, std::exp(1.0));
	std::cout<<"e = "<<exp(1.0)<<std::endl;

	id = tbl.Add("pi");
	AddValue(id, 2.0*acos(0.0));//PI == 2*0的反余弦函数
	std::cout<<"pi = "<<2.0*acos(0.0)<<std::endl;
}
double Storage::GetValue(unsigned int id) const
{
	assert(id < cells_.size());
	return cells_[id];
}
void Storage::SetValue(unsigned int id, double val)
{
	assert(id <= cells_.size());

	if(id < cells_.size())//值已经存在了
	{
		cells_[id] = val;
		inits_[id] = true;
	}
	else if(id == cells_.size())//new id val
	{
		AddValue(id, val);
	}
}
void Storage::AddValue(unsigned int id, double val)
{
	cells_.resize(id+1);//重新设置大小 加1
	inits_.resize(id+1);
	cells_[id] = val;	//不能使用pushback来添加 因为使用它分配的空间大小不一定 可能会导致size时大小不定 
	inits_[id] = true;	//所以我们使用数组的方式来添加新元素
}
