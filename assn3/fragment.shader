#version 330

out vec4 FragColor;

uniform vec4 ourColor; 

in vec2 fUV;
in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec4 color;
in vec3 fPL[10];

uniform vec4 AP, DP, SP;
uniform vec4 LP; // Light position
uniform mat4 MV;
uniform mat4 P;
uniform float Shininess;
uniform bool is_gouraud;
uniform int pointLightNum;
uniform vec4 pointLights[10];

uniform int tex_mapping;
uniform sampler2D ourTexture;

vec4 FcalcPointLight(vec3 PL, vec3 N, vec3 E);

void main()
{
	if (is_gouraud) {
        FragColor = color * ourColor;
        if (tex_mapping == 0) {
            FragColor = texture(ourTexture, fUV) * color;
        }
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

        vec4 result = vec4(0.0, 0.0, 0.0, 1.0);
        for (int i = 0; i < pointLightNum; i++)
            result += FcalcPointLight(fPL[i], N, E);

        vec4 lighting_color = ambient + diffuse + specular + result;
        lighting_color.a = 1.0;
        FragColor = lighting_color * ourColor;
        if (tex_mapping == 0) {
            FragColor = texture(ourTexture, fUV) * lighting_color;
        }
	}
}


vec4 FcalcPointLight(vec3 PL, vec3 N, vec3 E) {
    vec3 fplight = normalize(PL);
    vec3 H = normalize(fplight + E);
    vec4 ambient = AP;
    float Kd = max(dot(fplight, N), 0.0);
    vec4 diffuse = Kd * DP;
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks * SP;

    if (dot(fplight, N) < 0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);

    float d = length(PL);
    float att = 0.5 * min(1.0 / (0.01 + d * 0.1 + d * d * 0.1), 1);

    vec4 lighting_color = att * (ambient + diffuse + specular);
    return lighting_color;
}