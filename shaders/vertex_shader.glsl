#version 460 core

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Normal;

uniform mat4 RotationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 TranslationMatrix;

out vec4 outColor;
out vec4 fragmentShaderPosition;
out	vec4 outNormal;

void main() {
	outColor = vec4(1, 1, 1, 1);
	outNormal = normalize(RotationMatrix * Normal);
	fragmentShaderPosition = TranslationMatrix * RotationMatrix * Position;
	gl_Position = ProjectionMatrix * fragmentShaderPosition;
}
