#include <iostream>
#include "Tracer.h"

#ifndef NDEBUG


bool Tracer::Ready = false;

Tracer::Tracer(void) : lockCount_(0)
{
	Ready = true;
}

Tracer::~Tracer(void)
{
	Ready = false;
	Dump();
}


void Tracer::Add(void* p, const char* file, long line)
{
	if(lockCount_ > 0)
		return;

	Tracer::Lock lock(*this);

	mapEntry_[p] = Entry(file, line);
}
void Tracer::Remove(void* p)
{
	if(lockCount_ > 0)
		return;

	Tracer::Lock lock(*this);
	std::map<void*, Entry>::iterator it;
	it = mapEntry_.find(p);
	if(it != mapEntry_.end())
	{
		mapEntry_.erase(it);
	}
}
void Tracer::Dump()
{
	if(mapEntry_.size() > 0)
	{
		std::cout<<"**Memory leak(s):"<<std::endl;
		std::map<void*, Entry>::iterator it;

		for(it=mapEntry_.begin(); it!=mapEntry_.end(); ++it)
		{
			const char* file = it->second.File();
			long line = it->second.Line();
			int addr = reinterpret_cast<int>(it->first);//reinterpret_cast 将它强制转换为int型

			std::cout<<"0x"<<std::hex<<addr<<":"<<file<<","<<std::dec<<line<<std::endl;
		}
		std::cout<<std::endl;
	}
}

Tracer NewTracer;
void* operator new(size_t size)
{
	void* p = malloc(size);
	if(Tracer::Ready)
	{
		NewTracer.Add(p, "?", 0);
	}
	return p;
}

void operator delete(void* p)
{
	if(Tracer::Ready)
	{
		NewTracer.Remove(p);
	}
	free(p);
}
void* operator new(size_t size, const char* file, long line)
{
	void* p = malloc(size);
	if(Tracer::Ready)
	{
		NewTracer.Add(p, file, line);
	}
	return p;
}
void operator delete(void* p, const char* file, long line)
{
	if(Tracer::Ready)
	{
		NewTracer.Remove(p);
	}
	free(p);
}

void* operator new[](size_t size)
{
	void* p = malloc(size);
	if(Tracer::Ready)
	{
		NewTracer.Add(p, "?", 0);
	}
	return p;
}

void operator delete[](void* p)
{
	if(Tracer::Ready)
	{
		NewTracer.Remove(p);
	}
	free(p);
}
void* operator new[](size_t size, const char* file, long line)
{
	void* p = malloc(size);
	if(Tracer::Ready)
	{
		NewTracer.Add(p, file, line);
	}
	return p;
}
void operator delete[](void* p, const char* file, long line)
{
	if(Tracer::Ready)
	{
		NewTracer.Remove(p);
	}
	free(p);
}
#endif //NDEBUG
