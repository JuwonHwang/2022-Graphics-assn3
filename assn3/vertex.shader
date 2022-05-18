#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;
out vec4 color; // vertex shade

uniform vec4 AP, DP, SP; // Ambient product, Diffuse product, Specular product
uniform vec4 LP; // Light position
uniform mat4 MV;
uniform mat4 P;
uniform float Shininess;
uniform bool is_gouraud; 
uniform int pointLightNum;
uniform vec4 pointLights[10];

out vec3 fN;
out vec3 fL;
out vec3 fE;

vec4 calcPointLight(vec4 pointLight, vec3 mvPos);

void main(void)
{
	//gl_Position = P * MV * vec4(vPosition, 1.0f);
	if (is_gouraud) {
		vec3 pos = (MV * vPosition).xyz;

		vec3 L = normalize(LP.xyz - pos);
		vec3 E = normalize(-pos);
		vec3 H = normalize(L + E);
		vec3 N = normalize(MV * vec4(vNormal, 0.0)).xyz;

		vec4 ambient = AP;
		float Kd = max(dot(L, N), 0.0);
		vec4 diffuse = Kd * DP;
		float Ks = pow(max(dot(N, H), 0.0), Shininess);
		vec4 specular = Ks * SP;

		if (dot(L, N) < 0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);
		gl_Position = P * MV * vPosition;
		color = ambient +diffuse + specular;

		vec4 result = vec4(0.0, 0.0, 0.0, 1.0);
		for (int i = 0; i < pointLightNum; i++)
			result += calcPointLight(pointLights[i], pos);
		color += result;
		color.a = 1.0;
	}
	else {
		fN = normalize(MV * vec4(vNormal, 0.0)).xyz;
		fE = -vPosition.xyz;
		fL = LP.xyz;

		if (LP.w != 0.0) {
			fL = LP.xyz - vPosition.xyz;
		}
		gl_Position = P * MV * vPosition;
	}

}

vec4 calcPointLight(vec4 pointLight, vec3 mvPos) {

	vec3 L = normalize(pointLight.xyz - mvPos);
	vec3 E = normalize(-mvPos);
	vec3 H = normalize(L + E);
	vec3 N = normalize(MV * vec4(vNormal, 0.0)).xyz;

	vec4 ambient = 0.2 * AP;
	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = 0.2 * Kd * DP;
	float Ks = pow(max(dot(N, H), 0.0), Shininess);
	vec4 specular = Ks * SP;

	float d = length(L);
	float att = min(1.0 / (0.1 + d * 1.0 + d * d * 2.0), 1);

	if (dot(L, N) < 0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 _color = att * (ambient + diffuse + specular);

	return _color;
}