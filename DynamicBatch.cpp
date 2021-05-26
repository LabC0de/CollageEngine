#include "Batch.h"
#include "Debug.h"


DynamicBatch::DynamicBatch() : max(0)
{
	GLC(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &size));

	verts.resize(size);
	textures.reserve(size);
	min = size;
	GLC(glGenVertexArrays(1, &vao));
	GLC(glBindVertexArray(vao));

	GLC(glGenBuffers(1, &vbo));
	GLC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GLC(glBufferData(GL_ARRAY_BUFFER, size * sizeof(vertex), verts.data(), GL_DYNAMIC_DRAW));

	GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), 0));
	GLC(glEnableVertexAttribArray(0));

	GLC(glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, rot)));
	GLC(glEnableVertexAttribArray(1));

	GLC(glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, op)));
	GLC(glEnableVertexAttribArray(2));

	GLC(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, scl)));
	GLC(glEnableVertexAttribArray(3));

	GLC(glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, rat)));
	GLC(glEnableVertexAttribArray(4));

	GLC(glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, txid)));
	GLC(glEnableVertexAttribArray(5));

	GLC(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLC(glBindVertexArray(0));
}

void DynamicBatch::update() 
{
	if (min <= max)
	{
		GLC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GLC(glBufferSubData(GL_ARRAY_BUFFER, min * sizeof(vertex), (1 + max - min) * sizeof(vertex), &verts[0]));
		GLC(glBindBuffer(GL_ARRAY_BUFFER, 0));
		min = size; max = 0;
	}
}

void DynamicBatch::bind() const
{
	GLC(glBindVertexArray(vao));
	for (int i = 0; i < pos; i++)
	{
		textures.at(i).use(i);
	}
}

void DynamicBatch::unbind() const
{
	GLC(glBindVertexArray(0));
}

void DynamicBatch::clear()
{
	GLC(glDeleteVertexArrays(1, &vao));
	GLC(glDeleteBuffers(1, &vbo));
}
