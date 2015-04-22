#version 150 core

// View Matrix
uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
mat4 MVP = projMatrix * viewMatrix * modelMatrix;

// Vertex ins
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexCoord;
in vec2 vertexSecondCoord;
in vec4 vertexColor;

out vec3 Normal;
out vec4 Vertex;

// Fragment Outs
out vec4 fragmentColor;

// Texture Stuff
out vec2 firstTexcoord;
out vec2 secondTexcoord;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

////////// SCATTERING ///////////////////////////////////////////////////////////////////
const vec3 lightDir = vec3(1, 0, 1);

const vec3 Br = vec3(6.95e-6, 1.18e-5, 2.44e-5);
const vec3 Bm = vec3(4e-7, 6e-7, 2.4e-6);
const vec3 Brm = Br+Bm;
const vec4 Esun = vec4(1.0, 1.0, 1.0, 18.0);
const float g = 0.0;

float log2_e = 1.0/log(2.0);
const float e = 2.71828;
const float PI = 3.141592654;

out vec3 scattering;
out vec3 extinction;
out vec3 normal;
out vec3 light;

vec3 Fex(float s)
{
    vec3 ex = exp(Brm * -s);
    return ex;
}

vec3 rayleigh(float theta)
{
    float pi316 = 3.0/(16.0*PI);

    float phase = 1.0 + (theta*theta);

    vec3 ret = pi316*phase*Br;
    return ret;
}

vec3 mie(float theta)
{
    float pi14 = 1.0/(4.0*PI);

    float g1 = (1.0-g)*(1.0-g);
    float g2 = 1.0+(g*g);
    float g3 = 2.0*g;

    float phase = g1 / pow(g2 - g3*theta, 1.5);

    vec3 ret = pi14*phase*Bm;
    return ret;
}

vec3 inScatter(float s, float theta)
{
    vec3 num = rayleigh(theta) + mie(theta);
    num *= Esun.w;
    num *= 1.0-Fex(s);

    vec3 ret = num/Brm;
    return ret;
}

void doScattering() {
    vec4 pos =  MVP * vec4(vertexPosition,1.0f);

    light = normalize(vec3(lightPosition.xyz));

    normal = normalize(vertexNormal.xyz);

    vec3 wDir = normalize(pos.xyz - cameraPosition);

    float f_dist = pow(normal.y, 0.2); //fifth root
    float dist = (1.05 - f_dist) * 190000;

    //float dist = pos.z;
    float cosTheta = dot(normal, light);

    scattering = inScatter(dist, cosTheta);
    extinction = Fex(dist);

    light = lightDir;
}
////////// END SCATTERING ////////////////////////////////////////////////////////////////

void main(){
    gl_Position =  MVP * vec4(vertexPosition,1.0f);
    fragmentColor = vertexColor;

    Vertex = gl_Position;
    vec3 normalEyeSpace = vec4(MVP * vec4(vertexNormal,0.0)).xyz;
    Normal = normalize(normalEyeSpace * vertexNormal);

    // Textures
    firstTexcoord = vertexCoord.st;
    secondTexcoord = vertexSecondCoord.st;

    // Scattering
    doScattering();
}
