#version 330

out vec4 FragColor;

uniform vec4 ourColor; 

in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec4 color;

uniform vec4 AP, DP, SP;
uniform vec4 LP; // Light position
uniform mat4 MV;
uniform mat4 P;
uniform float Shininess;
uniform bool is_gouraud;

void main()
{
	if (is_gouraud) {
		FragColor = color * ourColor;
	}
	else {
        vec3 N = normalize(fN);
        vec3 E = normalize(fE);
        vec3 L = normalize(fL);

        vec3 H = normalize(L + E);
        vec4 ambient = AP;
        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd * DP;
        float Ks = pow(max(dot(N, H), 0.0), Shininess);
        vec4 specular = Ks * SP;

        if (dot(L, N) < 0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);

        vec4 lighting_color = ambient + diffuse + specular;
        lighting_color.a = 1.0;
        FragColor = lighting_color * ourColor;
	}
}