#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in float rotation;
layout (location = 2) in float opacity;
layout (location = 3) in float scale;
layout (location = 4) in float ratio;
layout (location = 5) in float texid;

layout (std140) uniform Camera
{
	mat4 projection;
	mat4 view;
};

out VS_OUT {
	float Scale;
	float Ratio;
	float Texid;
	float Rotation;
	float Opacity;
} vs_out;

float sigmoid(float inx)
{
	return 1 / (exp(-inx) + 1);
}

void main()
{
	gl_Position = projection * view * vec4(pos.x, pos.y, sigmoid(pos.y), 1.0);
	vs_out.Rotation = rotation;
	vs_out.Opacity = opacity;
	vs_out.Scale = scale;
	vs_out.Ratio = ratio;
	vs_out.Texid = texid;
}