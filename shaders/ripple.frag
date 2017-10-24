#pragma #version 130
#define M_PI 3.1415926535897932384626433832795

uniform sampler2D currentTexture; 
uniform float time;

void main( void ) {

	//intensity is the tightness of the wave, vertically
	float intensity = 20;

	// Pixel position.
	vec2 pos = gl_TexCoord[0].xy;
	pos.x = cos( pos.x + (cos( time + (pos.y*intensity))/4));
	
	// Set the color.
	gl_FragColor = texture2D(currentTexture,pos);

}