#pragma once
#include <functional>
class Cleaner
{
public:
	Cleaner(std::function<void(void)> cleanFunc);
	~Cleaner();

	Cleaner(const Cleaner&) = delete;
	Cleaner& operator = (const Cleaner&) = delete;

private:
	std::function<void(void)> cleanFunction;
};

