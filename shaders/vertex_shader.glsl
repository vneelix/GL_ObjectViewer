#version 410 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texture_coord;

uniform	float	mode;
uniform	vec4	color;
uniform	vec4	center;
uniform mat4	rotation;
uniform mat4	projection;
uniform mat4	translation;

out	DATA {
	/* object data */
	vec4	position;
	vec4	normal;
	vec4	color;

	/* area data */
	vec4	camera;
	vec4	light;

	float	mode;
	vec2	texture_coord;
}	output_data;

void main()
{
	mat4 r = transpose(mat4(
		1, 0, 0, center.x,
		0, 1, 0, center.y,
		0, 0, 1, center.z,
		0, 0, 0, 1
	)) * rotation *
	transpose(mat4(
		1, 0, 0, -center.x,
		0, 1, 0, -center.y,
		0, 0, 1, -center.z,
		0, 0, 0, 1
	));

	/* write object data */
    gl_Position = projection * translation * r * position;
	output_data.position = translation * r * position;
	output_data.normal = normalize(rotation * normal);
	output_data.color = color;

	/* write area data */
	output_data.camera = vec4(0, 0, 0, 1);
	output_data.light = vec4(0, 0, 0, 1);

	output_data.mode = mode;
	output_data.texture_coord = texture_coord;
}
