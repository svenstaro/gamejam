uniform float width;
uniform float height;
uniform float pixelSize;

vec4 effect(vec4 color, Image tex, vec2 uv, vec2 xy) {
    float dx = pixelSize * (1.0 / width);
    float dy = pixelSize * (1.0 / height);
    vec2 coord = vec2(dx * floor(uv.x / dx), dy * floor(uv.y / dy));
    return vec4(texture2D(tex, coord).rgb, 1.0);
}
