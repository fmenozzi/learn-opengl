#version 150 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vec3 norm       = normalize(Normal);
    vec3 lightDir   = normalize(lightPos - FragPos);
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float sp = 32.0;

    float ka = 0.1;
    float kd = max(dot(norm, lightDir), 0.0);
    float ks = 0.5 * pow(max(dot(viewDir, reflectDir), 0.0), sp);

    vec3 ambient  = ka * lightColor;
    vec3 diffuse  = kd * lightColor;
    vec3 specular = ks * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    color = vec4(result, 1.0);
}
