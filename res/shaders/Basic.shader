#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec3 v_normalColor;

uniform mat4 u_MVP;
uniform vec3 u_centerView;
uniform vec3 u_leftView;
uniform vec3 u_rightView;
uniform float u_threshold;

void main()
{
	gl_Position = u_MVP * position;

	float d_center = dot(normal, u_centerView);
	float d_left = dot(normal, u_leftView);
	float d_right = dot(normal, u_rightView);

	d_center = smoothstep(u_threshold, 1, d_center);
	d_left = smoothstep(u_threshold, 1, d_left);
	d_right = smoothstep(u_threshold, 1, d_right);

	v_normalColor.x = d_center;
	v_normalColor.y = d_left;
	v_normalColor.z = d_right;

	v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_normalColor;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_colorTransparency;

void main()
{
	//vec4 texColor = texture(u_Texture, v_TexCoord);
	vec4 texColor;
	texColor.xyz = v_normalColor + (0.6, 0.6, 0.6);
	texColor.w = u_colorTransparency;
	color = texColor;
};