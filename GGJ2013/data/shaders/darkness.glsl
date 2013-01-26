extern number range;  // radius of fog circle
extern number blur;   // size of blur in each direction of circle radius
extern number width;  // screen width
extern number height; // screen height

vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 pixel_coords) {
    float dx = width / 2 - pixel_coords.x;
    float dy = height / 2 - pixel_coords.y;
    float d = sqrt(dx * dx + dy * dy) - range;
    float a = clamp(d, -blur, blur) / (blur * 2) + 0.5;
    return vec4(color.rgb, sin((a - 0.5) * 3.1415)); // mix(a, 0, 1.0 / 255));
}
