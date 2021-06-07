#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 inColor;

uniform mat4 RotationMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 TranslationMatrix;

out vec4 outColor;
out vec4 outNormal;
out vec4 fragmentShaderPosition;

void main() {
	outColor = inColor;
	fragmentShaderPosition = TranslationMatrix * RotationMatrix * vec4(Position, 1.0);
	gl_Position = ProjectionMatrix * fragmentShaderPosition;
}
