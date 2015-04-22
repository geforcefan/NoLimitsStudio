
#version 150 core

// Color Fragment´n stuff
in vec4 fragmentColor;
out vec4 color;
vec4 fColor;

// Texture Stuff
uniform sampler2D firstTextureMap;
uniform sampler2D secondTextureMap;

uniform int useTexSampler;
uniform int useSecondTexSampler;

in vec2 firstTexcoord;
in vec2 secondTexcoord;

////////// SCATTERING ////////////////////////////////////////////////////////////////
in vec3 scattering;
in vec3 extinction;
in vec3 normal;
in vec3 light;
uniform int useScattering = 0;

// CUSTOM COLOR
uniform int useCustomColor = 0;
uniform vec4 customColor = vec4(1.0);

// Heat Colors
uniform int useHeatColor = 0;
uniform int useColorHeatByR = 0;
uniform float heatMinValue = -5;
uniform float heatMaxValue = 5;
uniform float heatValue = 10;
float heatMinVisibleWaveLength = 450.0;
float heatMaxVisibleWaveLength = 700.0;
float heatGamma = 0.8;
float heatIntensityMax = 1.0;

float heatAdjust(float Colour, float Factor) {
    if(Colour == 0) return 0.0;
    else return heatIntensityMax * pow(Colour * Factor, heatGamma);
}

void doHeatColor(float value, float MinValue, float MaxValues) {
    float wavelength, Factor, scaled;
    vec3 oColor;

    scaled = (value - MinValue) / (MaxValues - MinValue);
    wavelength = scaled * (heatMaxVisibleWaveLength - heatMinVisibleWaveLength) + heatMinVisibleWaveLength;

    float wl = floor(wavelength);

    if(wl > 379 && wl < 440) {
        oColor.r = -(wavelength - 440) / (440 - 380);
        oColor.g = 0.0;
        oColor.b = 1.0;
    }
    else if(wl > 439 && wl < 490) {
        oColor.r = 0.0;
        oColor.g = (wavelength - 440) / (490 - 440);
        oColor.b = 1.0;
    }
    else if(wl > 489 && wl < 510) {
        oColor.r = 0.0;
        oColor.g = 1.0;
        oColor.b = -(wavelength - 510) / (510 - 490);
    }
    else if(wl > 509 && wl < 580) {
        oColor.r = (wavelength - 510) / (580 - 510);
        oColor.g = 1.0;
        oColor.b = 0.0;
    }
    else if(wl > 579 && wl < 645) {
        oColor.r = 1.0;
        oColor.g = -(wavelength - 645) / (645 - 580);
        oColor.b = 0.0;
    }
    else if(wl > 644 && wl < 781) {
        oColor.r = 1.0;
        oColor.g = 0.0;
        oColor.b = 0.0;
    }
    else {
        oColor.r = 0;
        oColor.g = 0;
        oColor.b = 0;
    }

    if(wl > 379 && wl < 412) Factor = 0.3 + 0.7 * (wavelength - 380) / (420 - 380);
    else if(wl > 419 && wl < 701) Factor = 1;
    else if(wl > 700 && wl < 781) Factor = 0.3 + 0.7 * (780 - wavelength) / (780 - 700);
    else Factor = 0;

    oColor.r = heatAdjust(oColor.r, Factor);
    oColor.g = heatAdjust(oColor.g, Factor);
    oColor.b = heatAdjust(oColor.b, Factor);

    fColor = vec4(oColor, 1);
}

vec4 doScattering() {
    float Idiff = dot(normalize(normal), normalize(light));
    vec4 surfaceColour = vec4(0.0, 0.0, 0.0, 1.0); //colourless atmosphere

    vec4 final = (surfaceColour*vec4(extinction, 1.0)) + vec4(scattering, 1.0);
    return final;
}

////////// LIGHT ///////////////////////////////////////////////////////////
uniform vec3 lightPosition;
uniform vec4 lightColor;
uniform vec4 lightAmbient;

in vec4 Vertex;

vec4 lightNum0 () {
    vec4 final_color =
            (vec4(0.2)) +
    (lightAmbient * (0.4));

    vec3 N = normalize(normal);

    vec3 L = normalize(vec3(lightPosition.xyz));
    float lambertTerm = dot(N,L);

    if(lambertTerm > 0.0)
    {
            final_color += lightColor *
                           fColor *
                                       lambertTerm;

            vec3 E = normalize(Vertex.xyz);
            vec3 R = reflect(-L, N);
            float specular = pow( max(dot(R, E), 0.0),
                             100.0 );
            final_color += lightColor *
                           (0.8) *
                                       specular;
    }
    return final_color;
}
////////////////////////////////////////////////////////////////////////////////

vec4 applyTextures() {
    vec4 outColor;

    if(useSecondTexSampler == 1) {
        vec4 firstColor = fColor * texture(firstTextureMap, firstTexcoord.xy);
        vec4 blendColor = fColor * texture(secondTextureMap, secondTexcoord.xy);
        outColor = (firstColor * blendColor) / 1.0;
    } else if(useTexSampler == 1) {
        outColor = fColor * texture(firstTextureMap, firstTexcoord.xy);
    } else outColor = fColor;

    return outColor;
}

vec3 LightDir;

vec4 CelShading ( vec4 col )
{
    LightDir = normalize(cross(lightPosition.xyz, vec3(0,1,0)));
float Intensity = dot( LightDir , normalize(normal) );
float factor = 1.0;
if ( Intensity < 0.5 ) factor = 0.5;
col *= vec4 ( factor, factor, factor, 1.0 );

return col;
}

void main() {
    fColor = fragmentColor;
    color = vec4(1.0);

    if(useHeatColor == 1) doHeatColor(heatValue, heatMinValue, heatMaxValue);
    if(useCustomColor == 1) fColor = customColor;

    if(useColorHeatByR == 1) doHeatColor(fColor.r, heatMinValue, heatMaxValue);

    if(useScattering == 1) {
        color = doScattering();
    } else {
        color = lightNum0() * applyTextures();
    }

    //color = CelShading(color);
}
