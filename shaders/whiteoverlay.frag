#pragma #version 130
#define M_PI 3.1415926535897932384626433832795

uniform sampler2D currentTexture; // Our render texture

void main()
{
    float newR = 0.9;
    float newG = 0.9;
    float newB = 0.9;
    vec2 distortedTextureCoordinate = gl_TexCoord[0].st;
    vec4 acolor = texture2D(currentTexture,distortedTextureCoordinate);

    vec4 newColor = vec4(newR,newG,newB,acolor.a);

   
    gl_FragColor = newColor;
}