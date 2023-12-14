#version 450
out vec4 FragColor;

in vec3 Normal;
in vec2 UV;
in vec3 Position;



uniform vec3 _CameraPos;
uniform samplerCube _Texture;
uniform sampler2D _normalMap;
uniform vec3 _LightDir;
uniform float _AmbientK = 0.3;
uniform float _DiffuseK = 0.7;
uniform float _Time;
uniform float xScroll;
uniform float yScroll;
uniform float _mixVal;

float calcLight(vec3 normal){
	return _AmbientK + _DiffuseK * max(dot(-_LightDir,normal),0.0);
}

void main(){
       vec2 newUV = vec2(.2 * UV.x + _Time/xScroll, .2 * UV.y + _Time / yScroll);
	    vec3 I = normalize(Position - _CameraPos);
		vec4 normalMapColor = texture(_normalMap, newUV);
		vec3 newNormal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
		newNormal = mix(Normal, newNormal, _mixVal);
		vec3 R = reflect(I, normalize(newNormal));
		
		FragColor = vec4(texture(_Texture, R).rgb, 1.0);
		
		//FragColor = vec4(Normal,1.0);
}