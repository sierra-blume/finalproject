#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out Surface{
	vec2 UV;
	//Sierra: Added another output variable to pass information into the fragment shader
	vec3 newPos;
}vs_out;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

//Sierra: Using a uniform to reference the png
uniform sampler2D _HeightMap;

void main(){
	vs_out.UV = vUV;
	//Sierra: Using the texture function to get the color on the height map that corresponds with the UV coordinate of the vertex
	vec4 heightColor = texture(_HeightMap, vUV);
	//Sierra: Using the color value at the point to determine a height value (0-100)
	vs_out.newPos = vPos + vec3(0, heightColor.r * 100, 0);
	//Sierra: Passing the new position into the fragment shader for determining which texture to use
	gl_Position = _ViewProjection * _Model * vec4(vs_out.newPos, 1.0);
}