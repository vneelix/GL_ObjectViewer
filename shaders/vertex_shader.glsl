#version 410 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

uniform mat4	rotation;
uniform mat4	projection;
uniform mat4	translation;

uniform mat4	rotation_axisx;
uniform mat4	rotation_axisy;
uniform mat4	rotation_axisz;

out	DATA {
	/* object data */
	vec4	position;
	vec4	normal;
	vec4	color;

	/* area data */
	vec4	camera;
	vec4	light;
}	output_data;

void main()
{
	/* write object data */
    gl_Position = projection * translation * position;
	output_data.position = translation * position;
	output_data.normal = normalize(normal);
	output_data.color = vec4(0, 0, 1, 1);

	/* write area data */
	output_data.camera = vec4(0, 0, 0, 1);
	output_data.light = vec4(0, 0, 0, 1);
}
