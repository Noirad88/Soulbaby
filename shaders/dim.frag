uniform sampler2D currentTexture; // Our render texture

void main()
{
    float newR;
    float newG;
    float newB;

    newR = 0.6;
    newG = 0.6;
    newB = 0.6;
	
    vec4 newColor = vec4(newR,newG,newB,10);
    vec2 distortedTextureCoordinate = gl_TexCoord[0].st;
    gl_FragColor = newColor * texture2D(currentTexture, distortedTextureCoordinate);

}