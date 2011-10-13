uniform sampler2D tex;
uniform float new_sat;

void main (void)
{
	vec4 color = texture2D(tex, gl_TexCoord[0].st);

	vec3 rgb;
	rgb[0] = color.r * (new_sat);
	rgb[1] = color.g * (new_sat);
	rgb[2] = color.b * (new_sat);

	gl_FragColor = vec4(rgb[0],rgb[1],rgb[2],color.a);
}
