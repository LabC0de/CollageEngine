#pragma once

#include <GL/glew.h>
#include <cstdio>

#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

#ifdef DEBUG
#define DBG(x) x
static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		printf("[OpneGL Error] (%u)\t%s\t%s\t%i\n", error, function, file, line );
		return false;
	}
	return true;
}

#include <chrono>
#include <unordered_map>
struct ctr
{
	double duration, counter = 1;
};
static const std::unordered_map<std::string, ctr>& Profile(const char* funcall, double duration)
{
	static std::unordered_map<std::string, ctr> map;
	if (map.find(funcall) != map.end())
	{
		ctr tmp = map.at(funcall);
		map[funcall] = { (tmp.counter * duration + tmp.duration) / (tmp.counter + 1), tmp.counter + 1 };
	}
	map[funcall] = { duration, 1.0 };
	return map;
}
#ifdef _WIN64
#define ASSERT(x) if(!x) { __debugbreak(); }
#endif // _WIN64


#define PROFILE(x) do { auto start = std::chrono::steady_clock::now(); x; Profile(#x, std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - start).count()); } while(false)
#define GLC(x) do { GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__)) } while(false)
#else
#define GLC(x) x
#define DBG(x)
#define PROFILE(x) x
#endif