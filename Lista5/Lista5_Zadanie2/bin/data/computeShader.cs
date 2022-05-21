#version 440
layout(binding = 0) buffer dcA1 { float A1[]; };
layout(binding = 1) buffer dcA2 { float A2[]; };
layout(rgba8, binding = 4) uniform writeonly image2D img;
layout(local_size_x = 20, local_size_y = 20, local_size_z = 1) in;

const int W = 1920;
const int H = 1080;

int per(int x, int nx)
{
    if (x < 0)
        x += nx;
    if (x >= nx)
        x -= nx;
    return x;
}

const uint MAX_SIZE = 4u;
int neighborhoodDensity(int[MAX_SIZE] innerDensities, int[MAX_SIZE] outerDensities, int size, ivec2 pos)
{
    int aliveCount = 0;
    
    for (int n = 0; n < size; ++n)
    {
        int inner = innerDensities[n];
        int outer = outerDensities[n];

        for (int i = pos.x - outer; i <= pos.x + outer; ++i)
            for (int j = pos.y - outer; j <= pos.y + outer; ++j)
            {
                if (i == pos.x && j == pos.y)
                    continue;
                int index = per(i, W) + per(j, H) * W;
                if (A1[index] == 1.0f)
                    ++aliveCount;
            }
    }

    return aliveCount;
}

void main()
{
    ivec2 position = ivec2(gl_GlobalInvocationID.xy);
    int index = position.x + position.y * W;

    int inners1[MAX_SIZE] = { 0,  0,  0,  0};
    // int inners2[MAX_SIZE] = { 0,  4,  8,  0};
    // int inners3[MAX_SIZE] = { 0,  0,  0,  0};
    // int inners4[MAX_SIZE] = { 0,  4,  9, 12};
    
    int outers1[MAX_SIZE] = { 5,  0,  0,  0};
    // int outers2[MAX_SIZE] = { 1,  5,  8,  0};
    // int outers3[MAX_SIZE] = {11,  0,  0,  0};
    // int outers4[MAX_SIZE] = { 2,  7, 10, 15};

    int density1 = neighborhoodDensity(inners1, outers1, 1, position);
    // float density2 = neighborhoodDensity(inners2, outers2, 3, position);
    // float density3 = neighborhoodDensity(inners3, outers3, 1, position);
    // float density4 = neighborhoodDensity(inners4, outers4, 4, position);

    if (density1 >= 34 && density1 <= 58 && A1[index] == 1.0f)
        A2[index] = 1.0f;
    else if (density1 >= 34 && density1 <= 45)
        A2[index] = 1.0f;
    else
        A2[index] = 0.0f;


    vec4 col = vec4(A1[index] * position.x / float(W), A1[index] * position.y / float(H), A1[index], 1);
    imageStore(img, ivec2(gl_GlobalInvocationID.xy), col);
}