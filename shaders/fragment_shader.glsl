#version 410 core

#define	AMBIENT 0.1
#define LIGHT_INTENSE 0.64

in	DATA {
	/* object data */
	vec4	position;
	vec4	normal;
	vec4	color;

	/* area data */
	vec4	camera;
	vec4	light;
}	input_data;

out vec4 fragColor;

void main()
{
	vec4	toCameraVec = normalize(input_data.camera - input_data.position);

	/* phong lighting */
	vec4	normal = input_data.normal * sign(dot(input_data.normal, toCameraVec));
	vec4	lightVec = normalize(input_data.light - input_data.position);
	float	diffuseRatio = clamp(0., 1., dot(normal, lightVec)) * LIGHT_INTENSE + AMBIENT;
	vec4	reflectLightVec = 2 * normal * dot(normal, lightVec) - lightVec;
	float	specularRatio = pow(clamp(0., 1., dot(toCameraVec, reflectLightVec)), 64);
	/*  */

	vec4	objectColor = input_data.color * (1. - specularRatio) * diffuseRatio;
	vec4	specularColor = vec4(1, 1, 1, 1) * specularRatio * LIGHT_INTENSE;
    fragColor = clamp(vec4(0, 0, 0, 1), vec4(1, 1, 1, 1), objectColor + specularRatio);
}
