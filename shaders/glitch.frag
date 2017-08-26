uniform sampler2D currentTexture; // Our render texture
uniform float r;
uniform float g;
uniform float b;
uniform float mode;

void main()
{
    float newR;
    float newG;
    float newB;

  if(mode == 1.0){

    newR = 0.0 + r;
    newG = 0.1 + g;
    newB = 0.3 + b;
  }

else{

    newR = r;
    newG = g;
    newB = b;

   }
	
    vec4 newColor = vec4(newR,newG,newB,10);
    vec2 distortedTextureCoordinate = gl_TexCoord[0].st;
    gl_FragColor = newColor * texture2D(currentTexture, distortedTextureCoordinate);

}