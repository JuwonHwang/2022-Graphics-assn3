#version 330

in vec4 vPosition;
in vec3 vNormal;
out vec4 color; // vertex shade

uniform vec4 AP, DP, SP; // Ambient product, Diffuse product, Specular product
uniform vec4 LP; // Light position
uniform mat4 MV;
uniform mat4 P;
uniform float Shininess;
uniform bool is_gouraud;

out vec3 fN;
out vec3 fL;
out vec3 fE;

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
		color = ambient + diffuse + specular;
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

