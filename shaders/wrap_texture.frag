uniform sampler2D texture;
varying vec4 pos;

void main()
{
    vec4 tex1 = texture2D(texture, gl_TexCoord[0].xy);
    vec4 tex2 = texture2D(texture, vec2(1.0-gl_TexCoord[0].x, gl_TexCoord[0].y));
    gl_FragColor = mix(tex1, tex2, abs(pos.x/2.0));
}
