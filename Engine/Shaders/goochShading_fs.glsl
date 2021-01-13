#version 330 core
// Shadows
in vec4 exShadowVertex;
in vec3 exPosition;
in vec3 exNormal;

uniform vec3 LightPosition;
uniform sampler2D u_ShadowMap;

// Shading 
//-- Calculae the specular component
in vec3 exEyeVector; 
in vec3 exReflectionVector; // To compute the specular tern
in float exIntensity;       // Difusse intensity 

uniform vec3 SurfaceColor;
uniform vec3 AmbientWarm;
uniform vec3 AmbientCool;
uniform float DiffuseCool;  //How much diffuse will be transfer for the object
uniform float DiffuseWarm;
uniform float SpecularPower;

// Output color
out vec4 FragColor;

/**/
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(exNormal);
    vec3 lightDir = normalize(LightPosition - exPosition);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.0005);
    // PCF

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}
/**/
void main(void)
{
  // Diffuse color
  vec3 kcool = min(AmbientCool + DiffuseCool * SurfaceColor, 1.0).rgb;
  vec3 kwarn = min(AmbientWarm + DiffuseWarm * SurfaceColor, 1.0).rgb;
  vec3 diffuse = mix(kcool, kwarn, exIntensity);

  // Specular color
  vec3 R = normalize(exReflectionVector);
  vec3 E = normalize(exEyeVector);
  float RdotE = max(dot(R,E),0.0);
  vec3 specular = vec3(pow(RdotE, SpecularPower));
  vec4 goochColor =  vec4(vec3(diffuse + specular), 1);
  //FragColor =  vec4(exNormal, 1);

  // Shadow calculation and color
  float shadow = ShadowCalculation(exShadowVertex);
  vec4 shadowDiffuseColor = vec4(diffuse, 1).rgba * 0.6;
  shadowDiffuseColor.a = 0.6;

  FragColor = mix(shadowDiffuseColor, goochColor,(1.0 - shadow));
}