uniform sampler2D currentTexture; // Our render texture
uniform float screenHeight;
uniform float tick;


float pulse(float val, float dst) {
  return floor(mod(val*dst,1.0)+.5);
}

void main()
{


    float newR;
    float newG;
    float newB;

    newR = 1.0;
    newG = pulse(gl_FragCoord.y/screenHeight+tick,50.0);
    newB = 1.0; 

    vec4 newColor = vec4(newR,newG,newB,10);
    vec2 distortedTextureCoordinate = gl_TexCoord[0].st;
    gl_FragColor = newColor * texture2D(currentTexture, 	distortedTextureCoordinate);

}