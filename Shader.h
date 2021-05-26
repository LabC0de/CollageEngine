#pragma once
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdarg>

typedef unsigned int GLenum;

class Shader 
{
public:
	enum GenMethod
	{
		FORMAT = 0x01,
		FROM_FILE = 0x02,
		FORMAT_FILE = 0x03
	};
	Shader() : program(0) {}
	Shader(	const char* source_geom,
			const char* source_frag =	"version 330 core\n"\
										"out vec4 FragColor\n"\
										"void main() {\n"\
										"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"\
										"}\n\0",

			const char* source_vert =	"#version 330 core\n"\
										"layout (location = 0) in vec2 aPos\n"\
										"void main() {\n"\
										"	gl_Position(aPos.x, aPos.y, 1/(exp(-aPos.y) + 1), 1.0);"\
										"}\n\0")
	{
		make_program(source_vert, source_frag, source_geom);
	}
	Shader(const char* source_vert, const char* source_frag, const char* source_geom, int mthd, ...)
	{
		va_list args;
		va_start(args, mthd);
		std::string src_vert = read_to_string(source_vert, mthd, args);
		std::string src_frag = read_to_string(source_frag, mthd, args);
		std::string src_geom;
		if (source_geom)
		{
			src_geom = read_to_string(source_geom, mthd, args);
			make_program(src_vert.c_str(), src_frag.c_str(), src_geom.c_str());
		}
		else
		{
			make_program(src_vert.c_str(), src_frag.c_str(), nullptr);
		}
		va_end(args);
	}

	void use();
	void set_n_ints(const char* name, int n, int* val);
	void set_ubo(const char* name, unsigned idx);
private:
	void make_program(const char* source_vert, const char* source_frag, const char* source_geom);
	std::string read_to_string(const char* source, int method, va_list args)
	{
		std::string src;
		if (method & FROM_FILE)
		{
			std::ifstream f(source);
			if (!f.good()) throw -1;

			f.seekg(0, std::ios::end);
			src.reserve(f.tellg());
			f.seekg(0, std::ios::beg);

			src.assign((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			f.close();
		}
		else
		{
			src.reserve(strlen(source) + 1);
			src.assign(source);
		}
		if (method & FORMAT)
		{
			std::string format;
			size_t sz = std::vsnprintf(NULL, 0, src.c_str(), args);
			format.reserve(sz + 1);
			std::vsnprintf(&format[0], sz + 1, src.c_str(), args);
			return format;
		}
		return src;
	}
	unsigned make_shader(const char* src, GLenum type);
	void test_success(unsigned shader);
	unsigned program;
};