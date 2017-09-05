#pragma #version 410
#define M_PI 3.1415926535897932384626433832795

uniform sampler2D currentTexture; // Our render texture
uniform sampler2D distortionMapTexture; // Our heat distortion 

uniform float time; // Time used to scroll the distortion map
uniform float distortionFactor; // Factor used to control
uniform float riseFactor; // Factor used to control how fast 
uniform vec2 lvlsize;



void main()
{
    
    
    vec2 distortionMapCoordinate = gl_TexCoord[0].st;
    
    distortionMapCoordinate.t -= time * riseFactor;
    
    vec4 distortionMapValue = texture2D(distortionMapTexture, distortionMapCoordinate);

    vec2 distortionPositionOffset = distortionMapValue.xy;
    distortionPositionOffset -= vec2(0.5, 0.5);
    distortionPositionOffset *= 2.0;
    

    distortionPositionOffset *= distortionFactor;
    

    vec2 distortionUnused = distortionMapValue.zw;
    distortionPositionOffset *= (1.0 - gl_TexCoord[0].t);
    
    vec2 distortedTextureCoordinate = gl_TexCoord[0].st + distortionPositionOffset;

vec4 tempcurr = texture2D(currentTexture, distortedTextureCoordinate);

    float newR = 0.2 * sin(time * M_PI/5.0) + 1.0;
    float newG = 0.3 * sin(-time * M_PI/5.0) + 1.0;
    float newB = 0.1 * sin(time * M_PI/5.0) + 1.0;


vec4 acolor = texture2D(currentTexture,distortedTextureCoordinate);

    vec4 newColor = vec4(newR,newG,newB,acolor.a);
    
    gl_FragColor = newColor * texture2D(currentTexture, distortedTextureCoordinate);
}