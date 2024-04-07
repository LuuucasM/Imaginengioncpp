// Renderer2D Circle Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in uint a_EntityID;

layout(std140, binding = 0) uniform Camera{
	mat4 u_ViewProjection;
};

struct VertexOutput{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat uint v_EntityID;

void main()
{
	Output.Color = a_Color;
	Output.LocalPosition = a_LocalPosition;
	Output.Thickness = a_Thickness;
	Output.Fade = a_Fade;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out uint o_EntityID;

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat uint v_EntityID;

void main()
{
    
    float distance = 1.0 - length(Input.LocalPosition);
    float colorAlpha = smoothstep(0.0, Input.Fade, distance);
    colorAlpha *= smoothstep(Input.Thickness + Input.Fade, Input.Thickness, distance);

	if (colorAlpha == 0.0){
		discard;
	}

	o_color = Input.Color;
	o_color.a *= colorAlpha;
	o_EntityID = v_EntityID;
}