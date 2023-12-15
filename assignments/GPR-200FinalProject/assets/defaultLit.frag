#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	//Sierra: Added another input variable to pass information into the fragment shader
	vec3 newPos;
}fs_in;

//Sierra: Uniforms to reference the three textures
uniform sampler2D _GrassTexture;
uniform sampler2D _RockTexture;
uniform sampler2D _SnowTexture;

void main(){
	vec2 uv = fs_in.UV;

	//Sierra: Getting the color value for the uv coordinate on each of the textures
	vec4 grassColor = texture(_GrassTexture, uv);
	vec4 rockColor = texture(_RockTexture, uv);
	vec4 snowColor = texture(_SnowTexture, uv);
	//Sierra: Setting the grass texture as the default
	vec4 finalColor = grassColor;

	//Sierra: Creating a gradient between the grass and rock textures
	if (fs_in.newPos.y > 30.5 && fs_in.newPos.y <= 40)
	{
		finalColor = mix(grassColor, rockColor, (fs_in.newPos.y - 30.5) / 9.5); //divided by 9.5 because that is the range between the ends of the gradient
	}

	//Sierra: Setting all points between two values to the rock texture
	else if (fs_in.newPos.y > 40  && fs_in.newPos.y <= 80.5)
	{
		finalColor = rockColor;
	}

	//Sierra: Creating a gradient between the rock and snow textures
	else if (fs_in.newPos.y > 80.5 && fs_in.newPos.y <= 90)
	{
		finalColor = mix(rockColor, snowColor, (fs_in.newPos.y - 80.5) / 9.5); //divided by 9.5 because that is the range between the ends of the gradient
	}

	//Sierra: Setting all points between two values to the snow texture
	else if (fs_in.newPos.y > 90)
	{
		finalColor = snowColor;
	}
	FragColor = finalColor;
}