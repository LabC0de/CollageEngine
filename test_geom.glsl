#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (std140) uniform Camera
{
	mat4 projection;
	mat4 view;
};

vec2 rotate(vec2 ip, float deg)
{
	float sin_x = sin(radians(deg));
	float cos_x = cos(radians(deg));
	return vec2(ip.x * cos_x - ip.y * sin_x, ip.x * sin_x + ip.y * cos_x);
}

in VS_OUT {
	float Scale;
	float Ratio;
	float Texid;
	float Rotation;
	float Opacity;
} gs_in[];

out GS_OUT {
	float opacity;
	vec2 texCoords;
	float texid;
} gs_out;

void main()
{
	gs_out.opacity = gs_in[0].Opacity;
	gs_out.texid = gs_in[0].Texid;
	//gs_out.texid = 2;

	gs_out.texCoords = vec2(0.0, 0.0);
	vec2 rot = rotate(vec2(-gs_in[0].Scale *  gs_in[0].Ratio, 0.0), gs_in[0].Rotation);
	gl_Position = gl_in[0].gl_Position + (projection * vec4(rot.x, rot.y, 0.0, 0.0));
	EmitVertex();

	gs_out.texCoords = vec2(1.0, 0.0);
	rot = rotate(vec2(gs_in[0].Scale *  gs_in[0].Ratio, 0.0), gs_in[0].Rotation);
	gl_Position = gl_in[0].gl_Position + (projection * vec4(rot.x, rot.y, 0.0, 0.0));
	EmitVertex();

	gs_out.texCoords = vec2(0.0, 1.0);
	rot = rotate(vec2(-gs_in[0].Scale *  gs_in[0].Ratio, 2 * gs_in[0].Scale), gs_in[0].Rotation);
	gl_Position = gl_in[0].gl_Position + (projection * vec4(rot.x, rot.y, 0.0, 0.0));
	EmitVertex();

	gs_out.texCoords = vec2(1.0, 1.0);
	rot = rotate(vec2(gs_in[0].Scale *  gs_in[0].Ratio, 2 * gs_in[0].Scale), gs_in[0].Rotation);
	gl_Position = gl_in[0].gl_Position + (projection * vec4(rot.x, rot.y, 0.0, 0.0));
	EmitVertex();

	EndPrimitive();
}