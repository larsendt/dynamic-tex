uniform vec4 light_coords;
uniform mat4 light_modelview;
varying vec2 light_pos;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
    light_pos = vec2(light_modelview * gl_ProjectionMatrix * light_coords) * 0.5 + 0.5;
}
