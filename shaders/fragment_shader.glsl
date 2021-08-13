#version 410 core

#define	AMBIENT 0.08
#define LIGHT_INTENSE 0.72

in	DATA {
	/* object data */
	vec4	position;
	vec4	normal;
	vec4	color;

	/* area data */
	vec4	camera;
	vec4	light;

	float	mode;
	vec2	texture_coord;
}	input_data;

uniform sampler2D texture0;

out vec4 fragColor;

void main()
{
	vec4	toCameraVec = normalize(input_data.camera - input_data.position);

	vec4	color = input_data.color * (1. - input_data.mode)
				+ texture(texture0, input_data.texture_coord) * input_data.mode;

	/* phong lighting */
	vec4	normal = normalize(input_data.normal * sign(dot(input_data.normal, toCameraVec)));
	vec4	lightVec = normalize(input_data.light - input_data.position);
	float	diffuseRatio = clamp(dot(normal, lightVec), 0., 1.);
	vec4	reflectLightVec = reflect(-lightVec, normal);
	float	specularRatio = pow(clamp(dot(toCameraVec, reflectLightVec), 0., 1.), 1024);
	/*  */

    fragColor = clamp(color * (diffuseRatio * LIGHT_INTENSE + AMBIENT) + vec4(1, 1, 1, 1) * specularRatio * LIGHT_INTENSE,
	vec4(0, 0, 0, 1), vec4(1, 1, 1, 1));
}
