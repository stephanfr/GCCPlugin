#define _GLIBCXX_GTHREAD_USE_WEAK 0

#include <string>
#include <iostream>


namespace TestNamespace
{
	void			FunctionReturningVoid();

	int				FunctionReturningInt();

	std::string		FunctionReturningString();


	void*			FunctionReturningVoidPointer();
	void*			(*PointerToFunctionReturningVoidPointer)();
}
