#pragma once
#include "vertex.h"
#include "Collected.h"
#include "Texture.h"

/**
* If performance gets bad here's some things to try without cangeing (or only extending) the interface
* 1.) seperate the data into multiple buffers vbos with a single vao (Static for texid dynamic for rest)
*		-> only the vertex attrib pointers count 
* 2.) Buffered updating (use 2 vbos) who switch per frame
* 
* If that 'don help
* 1.) implement optimized buffers
*		-> STDynamic (uses spritesheet : vert is 3 floats bigger texidx replaced by 4 float texcoords)
*			-> throws on tex change
*			-> St drawable
*			-> emergency override for identically functional spritesheets (think summer winter etc stardew)
*		-> Hybrid dynamic (uses multiple spritesheets : vert is 4 floats bigger duh)
*			-> throws on tex change
*			-> st drawable (because no tex changes)
*			-> emergency override for identically functional spritesheets (think summer winter etc stardew)
*/

class DynamicBatch : public Collected
{
public:
	DynamicBatch();
	bool up_to_date() const
	{
		return (max == 0) && (min == size);
	}
	bool full() const
	{
		return pos == size;
	}
	inline bool visible() const { return bool(pos); } // if empty the batch is not visible

	void update();
	void bind() const;
	void unbind() const;
	int amount() const
	{
		return pos;
	}
	void set(int idx, const vertex& vt, const Texture& tex)
	{
		verts.at(idx) = vt;
		textures.at(idx) = tex;
		update_range(idx);
	}
	void add(const vertex& vt, const Texture& tex)
	{
		verts.at(pos) = vt;
		textures.emplace_back(tex);
		update_range(pos);
		pos += 1;
	}
	void remove(int idx)
	{
		verts.at(idx) = verts.at(pos - 1);
		textures.at(idx) = textures.at(pos - 1);;
		update_range(idx);
		pos -= 1;
	}
	~DynamicBatch() { dtor(); }
private:
	virtual void clear() override;
	void update_range(int idx)
	{
		min = min > idx ? idx : min;
		max = max < idx ? idx : max;
	}
	int min, max, pos = 0, size = 0;

	unsigned char act_buf = 0;
	unsigned vao, vbo;

	std::vector<vertex> verts;
	std::vector<Texture> textures;
};
