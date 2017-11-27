#include "Cleaner.h"


Cleaner::Cleaner(std::function<void(void)> cleanFunc)
	:cleanFunction(cleanFunc)
{
}

Cleaner::~Cleaner()
{
	cleanFunction();
}
