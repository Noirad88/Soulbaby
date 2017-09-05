uniform sampler2D currentTexture; // Our render texture
uniform float r;
uniform float g;
uniform float b;


void main()
{
    vec4 newColor = vec4(r,g,b,10);
    vec2 distortedTextureCoordinate = gl_TexCoord[0].st;
    gl_FragColor = newColor * texture2D(currentTexture, distortedTextureCoordinate);

}