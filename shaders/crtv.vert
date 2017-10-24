#version 150

attribute vec4 VertexCoord;
attribute vec4 TexCoord;
varying vec4 TEX0;
uniform mat4 MVPMatrix;

void main() {
  gl_Position = VertexCoord.x*MVPMatrix[0] + VertexCoord.y*MVPMatrix[1] + VertexCoord.z*MVPMatrix[2] + VertexCoord.w*MVPMatrix[3];
  TEX0.xy = TexCoord.xy;
} 