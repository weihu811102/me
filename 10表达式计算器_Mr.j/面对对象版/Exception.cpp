#include "Exception.h"

const char* Exception::what()const throw()//no exception
{
	return message_.c_str();
}
const char* Exception::StackTrace() const throw()
{
	return stackTrace_.c_str();
}
void Exception::FillStackTrace()
{
	//适用于linux的栈回溯
	//void* buffer[len];
	//int nptrs = ::backtrace_symbols(buffer, nptrs);
	//char** strings = ::backtrace_symbols(buffer, nptrs);
	//if(strings)
	//{
	//	for(int i=0; i<nptrs; ++i)
	//	{
	//		stackTrace_.append(strings[i]);
	//		stackTrace_.push_back('\n');
	//	}
	//	free(strings);
	//}
}
