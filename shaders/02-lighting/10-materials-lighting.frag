#version 150 core

struct Material {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float sp;
};

struct Light {
    // Intensities
    vec3 ai;
    vec3 di;
    vec3 si;

    vec3 pos;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 viewPos;

uniform Material mat;
uniform Light light;

void main() {
    vec3 norm       = normalize(Normal);
    vec3 lightDir   = normalize(light.pos - FragPos);
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    vec3 ambient  = light.ai * mat.ka;
    vec3 diffuse  = light.di * max(dot(norm, lightDir), 0.0) * mat.kd;
    vec3 specular = light.si * pow(max(dot(viewDir, reflectDir), 0.0), mat.sp) * mat.ks;

    vec3 result = ambient + diffuse + specular;

    color = vec4(result, 1.0);
}
