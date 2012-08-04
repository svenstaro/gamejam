uniform Image diffuseTexture;

vec4 effect(vec4 color, Image tex, vec2 uv, vec2 xy) {
    vec4 c = texture2D(diffuseTexture, xy);
    return vec4(c.rgb, c.a * color.a);
    //return vec4(uv, xy) + c * 0.001;
}
