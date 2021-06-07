#version 460 core

in vec4 outColor;
in vec4 fragmentShaderPosition;

out vec4 FragColor;

void main() {
	FragColor = outColor;
}
