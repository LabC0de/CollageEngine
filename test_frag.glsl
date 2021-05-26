#version 330 core
out vec4 FragColor;

uniform sampler2D textures[%d];

in GS_OUT {
	float opacity;
	vec2 texCoords;
	float texid;
} fs_in;

void main()
{
	vec4 color = texture(textures[int(fs_in.texid)], fs_in.texCoords);
	if(color.a < 0.1) {
		discard;
	}
	FragColor = vec4(color.x, color.y, color.z, color.a * fs_in.opacity);
}