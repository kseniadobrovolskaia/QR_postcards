#version 330 core

in vec3 Normal;
in vec3 frag_pos;

out vec4 color;

uniform vec3 obj_color;
uniform vec3 light_color;
uniform vec3 light_pos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 light_ray = normalize(light_pos - frag_pos);

    float diff = max(dot(norm, light_ray), 0.0);
    vec3 diffuse = diff * light_color;

    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * light_color;

    vec3 result = (ambient + diffuse) * obj_color;
    color = vec4(result, 1.0f);
}