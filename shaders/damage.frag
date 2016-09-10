#pragma #version 130
#define M_PI 3.1415926535897932384626433832795

uniform sampler2D currentTexture; // Our render texture
uniform float damage;

void main()
{
    float newR = damage;
    float newG = 0.1;
    float newB = 0.1;
    vec4 newColor = vec4(newR,newG,newB,10);
    vec2 distortedTextureCoordinate = gl_TexCoord[0].st;
    
    gl_FragColor = newColor * texture2D(currentTexture, distortedTextureCoordinate);
}