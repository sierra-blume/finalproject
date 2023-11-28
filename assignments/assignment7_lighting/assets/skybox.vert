#version 450
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	texCoords = vec3(model * vec4(aPos, 1.0));
	vec4 pos = projection * view * model * vec4(aPos, 1.0f);
	gl_Position = pos.xyzw;
}