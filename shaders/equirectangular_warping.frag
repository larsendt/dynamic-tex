uniform sampler2D texture;

void main()
{
    vec2 p = gl_TexCoord[0].xy * 2.0 - 1.0;
    p.x = p.x * cos(p.y*1.57079633);
    p = (p + 1.0) * 0.5;
    gl_FragColor = texture2D(texture, p);
}
