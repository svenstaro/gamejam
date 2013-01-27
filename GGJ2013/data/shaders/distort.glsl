extern number lifetime;     // time since start
extern number distortion;   // level of distortion (0..1)

vec4 randomizer4(const vec4 x)
{
    vec4 z = mod(x, vec4(5612.0));
    z = mod(z, vec4(3.1415927 * 2.0));
    return(fract(cos(z) * vec4(56812.5453)));
}

const float A = 1.0;
const float B = 57.0;
const float C = 113.0;
const vec3 ABC = vec3(A, B, C);
const vec4 A3 = vec4(0, B, C, C+B);
const vec4 A4 = vec4(A, A+B, C+A, C+A+B);

float cnoise4(const in vec3 xx)
{
    vec3 x = mod(xx + 32768.0, 65536.0);
    vec3 ix = floor(x);
    vec3 fx = fract(x);
    vec3 wx = fx*fx*(3.0-2.0*fx);
    float nn = dot(ix, ABC);

    vec4 N1 = nn + A3;
    vec4 N2 = nn + A4;
    vec4 R1 = sin(N1);
    vec4 R2 = sin(N2);
    vec4 R = mix(R1, R2, wx.x);
    float re = mix(mix(R.x, R.y, wx.y), mix(R.z, R.w, wx.y), wx.z);

    return 1.0 - 2.0 * re;
}

vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 pixel_coords) {
    float dist = distortion * abs(sin(floor(lifetime * 0.4)));
    float dist2 = distortion * abs(sin(floor(lifetime * 0.6))) * 0.2;
    vec2 distCoords = texture_coords;
    float dx = cnoise4(vec3(lifetime + texture_coords.y * 25 * dist2, 0, 0));
    dx = mod(dx, 0.9) > 0.8 ? 0.0 : floor(dx * 5.0) / 9.0;
    distCoords.x += dx * 0.0006 * cnoise4(vec3(0, lifetime * dist * dist, 0) * dist * 100);
    color.b += 0.06 + dx * 0.06 * cnoise4(vec3(0, sin(floor(lifetime)), 0) * dist * 100000);
    //distCoords.y += noise(vec2(texture_coords.y, texture_coords.x) * -213.21) * dist * 0.01;

    if(mod(pixel_coords.y, 4) <= 2) {
        color *= 0.95;
    }

    if(mod(pixel_coords.y + 1, 4) <= 2) {
        color.b += 0.1;
    }

    return texture2D(texture, distCoords) * color;
}
