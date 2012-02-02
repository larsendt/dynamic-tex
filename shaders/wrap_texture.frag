uniform sampler2D texture;
varying vec4 pos;

void main()
{
    vec4 texel1 = texture2D(texture, gl_TexCoord[0].xy);
    vec4 texel2 = texture2D(texture, vec2(1.0-gl_TexCoord[0].x, gl_TexCoord[0].y));
    gl_FragColor = mix(texel1, texel2, abs(pos.x/2.0));
}
