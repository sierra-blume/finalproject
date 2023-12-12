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
	vec2 uv = fs_in.UV;

	vec4 grassColor = texture(_GrassTexture, uv);
	vec4 rockColor = texture(_RockTexture, uv);
	vec4 snowColor = texture(_SnowTexture, uv);
	vec4 finalColor = grassColor;
	
	if (fs_in.newPos.y > 3.5 && fs_in.newPos.y <= 4)
	{
		finalColor = mix(grassColor, rockColor, (fs_in.newPos.y - 3.5) / 0.5);
	}

	else if (fs_in.newPos.y > 4  && fs_in.newPos.y <= 8.5)
	{
		finalColor = rockColor;
	}

	else if (fs_in.newPos.y > 8.5 && fs_in.newPos.y <= 9)
	{
		finalColor = mix(rockColor, snowColor, (fs_in.newPos.y - 8.5) / 0.5);
	}

	else if (fs_in.newPos.y > 9)
	{
		finalColor = snowColor;
	}
	FragColor = finalColor;
}