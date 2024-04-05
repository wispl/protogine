#version 460 core

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_uv;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct GlobalLight {
	vec3 direction;
	vec3 color;
};

#define MAX_LIGHTS 4
struct PointLight {
	vec4 position;
	vec4 color;
	float constant;
	float linear;
	float quadratic;

	float padding;
};

layout(std140, binding = 0) uniform Lights {
	PointLight lights[MAX_LIGHTS];
};

uniform GlobalLight global_light;
uniform Material material;
uniform vec3 view_pos;
uniform int light_idx;

out vec4 FragColor;

vec3 calculate_globallight(GlobalLight light, vec3 normal, vec3 view_dir);
vec3 calculate_pointlight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main()
{
	if (light_idx >= 0) {
		FragColor = vec4(lights[light_idx].color);
		return;
	}

	vec3 normal = normalize(frag_normal);
	vec3 view_dir = normalize(view_pos - frag_pos);

	// calculate Global Light
	vec3 result = calculate_globallight(global_light, normal, view_dir);
	// Calculate Point Lights
	for(int i = 0; i < MAX_LIGHTS; ++i) {
		result += calculate_pointlight(lights[i], normal, frag_pos, view_dir);
	}

	FragColor = vec4(result, 1.0);
}

vec3 calculate_globallight(GlobalLight light, vec3 normal, vec3 view_dir)
{
    vec3 light_dir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    // combine results
    vec3 ambient = vec3(0.2f) * vec3(texture(material.diffuse, frag_uv));
    vec3 diffuse = light.color * diff * vec3(texture(material.diffuse, frag_uv));
    vec3 specular = spec * vec3(texture(material.specular, frag_uv));
    return (ambient + diffuse + specular);
}

vec3 calculate_pointlight(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.position.xyz - frag_pos);
    // diffuse shading
    float diff = max(dot(normal, light_dir), 0.0);
    // specular shading
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position.xyz - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = vec3(0.2f) * vec3(texture(material.diffuse, frag_uv));
    vec3 diffuse = light.color.rgb * diff * vec3(texture(material.diffuse, frag_uv));
    vec3 specular = spec * vec3(texture(material.specular, frag_uv));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
