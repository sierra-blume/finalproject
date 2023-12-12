#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out Surface{
	vec2 UV;
	vec3 newPos;
}vs_out;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

uniform sampler2D _HeightMap;

void main(){
	vs_out.UV = vUV;
	vec4 heightColor = texture(_HeightMap, vUV);
	vs_out.newPos = vPos + vec3(0, heightColor.r * 10, 0);
	gl_Position = _ViewProjection * _Model * vec4(vs_out.newPos, 1.0);
}