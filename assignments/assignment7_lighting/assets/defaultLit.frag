#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 newPos;
}fs_in;

uniform sampler2D _GrassTexture;
uniform sampler2D _RockTexture;
uniform sampler2D _SnowTexture;

void main(){
	//vec4 grassColor = texture(_GrassTexture, fs_in.UV);
	//vec4 rockColor = texture(_RockTexture, fs_in.UV);
	//vec4 snowColor = texture(_SnowTexture, fs_in.UV);
	FragColor = texture(_GrassTexture,fs_in.UV);
}