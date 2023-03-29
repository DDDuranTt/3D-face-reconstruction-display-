#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float alpha;


out vec2 v_TexCoord;
out float v_Alpha;

uniform mat4 u_MVP;
uniform float leftEdge1;
uniform float leftEdge2;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;


	vec2 pos = position.yz;
	pos.y -= 550.0f;
	mat2 rotationMat = mat2(0, 1,
		1, 0);
	pos = pos * rotationMat;

	float angle = degrees(atan(pos.y / pos.x));

	v_Alpha = smoothstep(leftEdge1, leftEdge2, angle);
};

#shader fragment
#version 330 core

in vec2 v_TexCoord;
in float v_Alpha;

layout(location = 0) out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	texColor.w = v_Alpha;
	color = texColor;
};