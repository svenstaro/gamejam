extern number range;  // radius of fog circle
extern number blur;   // size of blur in each direction of circle radius
extern number width;  // screen width
extern number height; // screen height
extern sampler2D shadowmap;

vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 pixel_coords) {
    float dx = width / 2 - pixel_coords.x;
    float dy = height / 2 - pixel_coords.y;

    float alpha = atan(dy, dx);
    float tx = (alpha / 3.1415927) * 0.5 + 0.5;

    float d = sqrt(dx * dx + dy * dy);
    vec4 shadowV = texture2D(shadowmap, vec2(tx, 0.5));
    float shadow = shadowV.r + shadowV.g * 256 + shadowV.b * 256 * 256;
    shadow = shadow / 1000.0;

    float diff = shadow * 256 - d;

    float a = mix(0, 1, clamp(diff / 100 + 0.5, 0, 1));

    float dd = d - range;
    float aa = sin((clamp(dd, -blur, blur) / (blur * 2)) * 3.1415);

    return vec4(color.rgb, 1 - a * (1 - aa)); // sin((a - 0.5) * 3.1415)); // mix(a, 0, 1.0 / 255));
}
