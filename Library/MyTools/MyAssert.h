#pragma once
#include <string>
#include <exception>


// use TOGGLE_ALL_DEBUG_MESSAGE to turn on all the debug output

class  SimpleException: std::exception
{
public:
	const char* expr;
	const char* file;
	const int line;

public:
	SimpleException(const char *e, const char* f, const int line)
		:expr(e), file(f), line(line)
	{
	}

	inline std::string SimpleException::ToString()
	{
		char lineString[256];
		std::string returnval;

		sprintf_s(lineString, "\nFile:\t\t%s\nLine:\t\t%d\nExperssion:\t%s", file, line, expr);

		returnval += lineString;
		return returnval;
	}
};


#ifdef TOGGLE_ALL_DEBUG_MESSAGE
#define ASSERTIONS_ENABLED
#endif
// remaind that if the ASSERTIONS_ENABLED is turned off, 
// the expr in the ASSERT() will no exist.
// please not put any key operation in the ASSERT(For that you 
// may consider using ThrowIfFalse(bool).).
#ifdef ASSERTIONS_ENABLED
#define ASSERT(expr) \
	if(expr){}\
	else\
	{\
		throw SimpleException( #expr, __FILE__, __LINE__);\
	}
#else
#define ASSERT(expr)//����ֵ
#endif


#ifdef TOGGLE_ALL_DEBUG_MESSAGE
#define CHECK_THROW_IF_ENABLED
#endif
// Simulate to the ThrowIfFailed(HRESULT).
// Here we just use a boolean to toggle the exception.
#ifdef CHECK_THROW_IF_ENABLED
#define ThrowIfFalse(expr)\
	if (expr){}\
	else\
	{\
		throw SimpleException(#expr, __FILE__, __LINE__); \
	}
#else
#define ThrowIfFalse(expr) expr
#endif

#ifdef TOGGLE_ALL_DEBUG_MESSAGE
#define SHOW_DEBUG_MESSAGE
#endif
// print some message only in the debug mode.
#ifdef SHOW_DEBUG_MESSAGE
#define DEBUG_MESSAGE(format, ...) fprintf(stderr, format, ##__VA_ARGS__)
#else
#define DEBUG_MESSAGE(...)
#endif

//�����������һЩ����û��ʵ�ֵ�ʱ���׳�����쳣����ֹ����һЩ�����ʵ��
#define THROW_UNIMPLEMENT_EXCEPTION(pChar)\
	{\
		throw SimpleException(pChar, __FILE__, __LINE__);\
	}
