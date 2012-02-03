uniform float screen;

void main()
{
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   gl_TexCoord[0] = vec4(gl_Position.xyz * 0.5 + 0.5, 1.0);
}
