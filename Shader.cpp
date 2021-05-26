#include "Shader.h"
#include <glm.hpp>
#include <GL/glew.h>
#include "Debug.h"

void Shader::use()
{
	GLC(glUseProgram(program));
}

void Shader::set_n_ints(const char* name, int n, int* val)
{
	GLC(glUniform1iv(glGetUniformLocation(program, name), n, val));
}

void Shader::set_ubo(const char* name, unsigned idx)
{
	unsigned pos = glGetUniformBlockIndex(program, name);
	GLC(glUniformBlockBinding(program, pos, idx));
}

void Shader::make_program(const char* source_vert, const char* source_frag, const char* source_geom)
{
	unsigned vert_shader = make_shader(source_vert, GL_VERTEX_SHADER);
	unsigned frag_shader = make_shader(source_frag, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	GLC(glAttachShader(program, vert_shader));
	GLC(glAttachShader(program, frag_shader));
	if (source_geom)
	{
		unsigned geom_shader = make_shader(source_geom, GL_GEOMETRY_SHADER);
		GLC(glAttachShader(program, geom_shader));
		GLC(glDeleteShader(geom_shader));
	}
	GLC(glLinkProgram(program));
	GLC(glDeleteShader(vert_shader));
	GLC(glDeleteShader(frag_shader));
}

unsigned Shader::make_shader(const char* src, GLenum type)
{
	unsigned tmp = glCreateShader(type);
	GLC(glShaderSource(tmp, 1, &src, NULL));
	GLC(glCompileShader(tmp));
	test_success(tmp);
	return tmp;
}

void Shader::test_success(unsigned shader)
{
	int success;
	char infoLog[1024];
	GLC(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLC(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
		printf("ERROR COMPILING SHADER:\n%s", infoLog);
	}
}
