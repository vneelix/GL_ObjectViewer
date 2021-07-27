#version 460 core

in vec4 outColor;
in vec4 fragmentShaderPosition;
in vec4 outNormal;

out vec4 FragColor;

void main() {

	float d = 0.08;

	vec4 lPosition = vec4(0, 0, 0, 1);
	vec4 lVec = normalize(lPosition - fragmentShaderPosition);

	vec4 n = outNormal;

	vec4 pos = vec4(0, 0, 0, 1);

	vec4 fromPoint = normalize(pos - fragmentShaderPosition);

	n *= sign(dot(fromPoint, n));
	d += clamp(0., 1., dot(lVec, n)) * 0.64;

	FragColor = outColor * d;
	FragColor.a = 1.;
}
