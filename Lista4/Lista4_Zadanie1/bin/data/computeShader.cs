#version 440

layout(binding = 0) buffer dcA1 { float A1 []; };
layout(rgba8, binding = 1) uniform writeonly image2D img;
layout(local_size_x = 20, local_size_y = 20, local_size_z = 1) in;

const int W = 1280;
const int H = 720;

const int centerX = W / 2;
const int centerY = H / 2;

void main()
{
    int x, y;
    x = int(gl_GlobalInvocationID.x);
    y = int(gl_GlobalInvocationID.y);
    
    int idx = x + y * W;
    A1[idx] = distance(vec2(x, y), vec2(centerX, centerY)) / float(min(W, H));

    vec4 col = vec4(A1[idx], 0.2, 0.3, 1);
    imageStore(img, ivec2(gl_GlobalInvocationID.xy), col);
}