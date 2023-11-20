#version 330

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform sampler2D diffuse;

void main()
{
    vec3 ambient = vec3(0.8f, 0.8f, 0.8f) * texture(diffuse, TexCoords).rgb;
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(-vec3( -0.2f, -1.0f, 1.0f ));
    float diff = max( dot(norm, lightDir) , 0.0f );
    vec3 diffuse = vec3(0.5f, 0.5f, 0.5f) * diff * texture(diffuse, TexCoords).rgb;


    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), 32.0f ); // 32.0f is the material shininess
    vec3 specular = vec3(1.0f, 1.0f, 1.0f) * spec;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

}