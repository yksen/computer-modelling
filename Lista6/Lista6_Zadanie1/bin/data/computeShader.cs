#version 440
layout(binding = 0) buffer dcSand1 { float Sand1[]; };
layout(binding = 1) buffer dcSand2 { float Sand2[]; };
layout(binding = 2) buffer dcGround { float Ground[]; };
layout(rgba8, binding = 4) uniform writeonly image2D img;
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

uniform int c;

const int W = 1280;
const int H = 720;

const float p = 0.5f;

int per(int x, int nx)
{
    if (x < 0)
        x += nx;
    if (x >= nx)
        x -= nx;
    return x;
}

float random (vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43766.705);
}

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    int index = pos.x + pos.y * W;

    if (pos.x < 1 || pos.x >= W - 1 || pos.y < 1 || pos.y >= H - 1) return;
    if (c == 1)
        if (pos.x % 2 == 0 || pos.y % 2 == 0) return;
    else
        if (pos.x % 2 == 1 || pos.y % 2 == 1) return;

    float Sul = Sand1[index];
    float Sur = Sand1[index + 1];
    float Sll = Sand1[index + W];
    float Slr = Sand1[index + W + 1];

    float Gul = Ground[index];
    float Gur = Ground[index + 1];

    if (Sul == 1.0f && Sur == 1.0f && Sll == 0.0f && Slr == 0.0f)
    {
        if (random(vec2(pos.x, pos.y)) < p)
        {
            Sand2[index] = 0.0f;
            Sand2[index + 1] = 0.0f;
            Sand2[index + W] = 1.0f;
            Sand2[index + W + 1] = 1.0f;
        }
        else
        {
            Sand2[index] = 1.0f;
            Sand2[index + 1] = 1.0f;
            Sand2[index + W] = 0.0f;
            Sand2[index + W + 1] = 0.0f;
        }
    }
    else
    {
        Sand2[index] =          Gul * Sul + (1 - Gul) * Sul * Sll * (Slr + (1 - Slr) * Sur);
        Sand2[index + 1] =      Gur * Sur + (1 - Gur) * Sur * Slr * (Sll + (1 - Sll) * Sul);
        Sand2[index + W] =      Sll + (1 - Sll) * (Sul * (1 - Gul) + (1 - Sul) * Sur * (1 - Gur) * Slr);
        Sand2[index + W + 1] =  Slr + (1 - Slr) * (Sur * (1 - Gur) + (1 - Sur) * Sul * (1 - Gul) * Sll);
    }

    vec4 col = vec4(Sand1[index] * 0.921 + Ground[index] * 1, Sand1[index] * 0.682, Sand1[index] * 0.141, 1);
    imageStore(img, ivec2(gl_GlobalInvocationID.xy), col);
}