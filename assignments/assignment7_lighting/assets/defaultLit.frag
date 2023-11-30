#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
}fs_in;

uniform sampler2D _HeightMap;

void main(){
	//vec4 texColor = 
	FragColor = texture(_HeightMap,fs_in.UV);
}