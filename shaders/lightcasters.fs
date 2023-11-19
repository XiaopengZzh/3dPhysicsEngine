#version 330

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    //float shininess;
};

struct Light
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
//uniform Light light;

void main()
{
    Light light;
    light.direction = vec3(-0.2f, -1.0f, 1.0f);
    light.ambient = vec3(0.8f, 0.8f, 0.8f);
    light.diffuse = vec3(0.5f, 0.5f, 0.5f);
    light.specular = vec3(1.0f, 1.0f, 1.0f);
    //material.shininess = 32.0f;
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max( dot(norm, lightDir ), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), 32.0f ); // 32.0f is the material shininess
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

}