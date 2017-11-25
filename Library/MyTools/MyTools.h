#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "MyAssert.h"
#include "Cleaner.h"

#define Not(x) (!x)

//删除类的复制构造函数和等于号重载函数。
#define DELETE_COPY_CONSTRUCTOR(CLASS) \
	CLASS(const CLASS&) = delete;\
	CLASS& operator = (const CLASS&) = delete;
