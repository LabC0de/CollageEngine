#include "Layer.h"
#include <GL/glew.h>
#include "Debug.h"

DynamicLayer::DynamicLayer()
{
	int size;
	GLC(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &size));
	shdr = Shader("def_vert.glsl", "def_frag.glsl", "def_geom.glsl", Shader::FORMAT_FILE, size);
	std::vector<int> nums(size);
	for (int i = 0; i < size; i++)
	{
		nums[i] = i;
	}
	shdr.use();
	shdr.set_n_ints("textures", size, &nums[0]);
	shdr.set_ubo("Camera", 0);
	batches.emplace_back();
}

void DynamicLayer::draw()
{
	shdr.use();
	for (DynamicBatch& bt : batches)
	{
		if (bt.visible())
		{
			if (!bt.up_to_date()) bt.update();
			bt.bind();
			GLC(glDrawArrays(GL_POINTS, 0, bt.amount()));
		}
	}
}