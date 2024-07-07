#version 460
in vec3 fragPos;
in vec2 texCoord;
in vec3 scaledNormal;
//Propriedades do material do objeto
uniform vec3 ka;
uniform float kd;
uniform vec3 ks;
uniform float q;
//Posicao da camera
uniform vec3 cameraPos;
//Propriedades da fonte de luz
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform sampler2D colorBuffer;
out vec4 color;
void main()
{
    //Ambient
    vec3 ambient =  lightColor * ka;
    //Diffuse
    vec3 N = normalize(scaledNormal);
    vec3 L = normalize(lightPos - fragPos);
    float diff = max(dot(N, L),0.0);
    vec3 diffuse = diff * lightColor * kd;
    //Specular
    vec3 R = reflect(-L,N);
    vec3 V = normalize(cameraPos - fragPos);
    float spec = pow(max(dot(R,V),0.0),q);
    vec3 specular = spec * ks * lightColor;
    vec4 texColor = texture(colorBuffer,texCoord);
    vec3 result = (ambient + diffuse) * vec3(texColor) + specular;
    color = vec4(result, 1.0f);
}