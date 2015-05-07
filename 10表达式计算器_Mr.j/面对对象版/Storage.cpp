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
	return id < cells_.size() && inits_[id];//id ���ܳ���cells_�����ֵ �����Ƿ��ʼ��
}
void Storage::AddConstants(SymbolTable& tbl)
{
	std::cout<<"variable list"<<std::endl;
	unsigned int id = tbl.Add("e");
	AddValue(id, std::exp(1.0));
	std::cout<<"e = "<<exp(1.0)<<std::endl;

	id = tbl.Add("pi");
	AddValue(id, 2.0*acos(0.0));//PI == 2*0�ķ����Һ���
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

	if(id < cells_.size())//ֵ�Ѿ�������
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
	cells_.resize(id+1);//�������ô�С ��1
	inits_.resize(id+1);
	cells_[id] = val;	//����ʹ��pushback����� ��Ϊʹ��������Ŀռ��С��һ�� ���ܻᵼ��sizeʱ��С���� 
	inits_[id] = true;	//��������ʹ������ķ�ʽ�������Ԫ��
}
