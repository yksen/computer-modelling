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
float neighborhoodDensity(int[MAX_SIZE] innerDensities, int[MAX_SIZE] outerDensities, int size, ivec2 pos)
{
    int aliveCount = 0;
    int cellCount = 1;
    
    for (int n = 0; n < size; ++n)
    {
        int inner = innerDensities[n];
        int outer = outerDensities[n];

        for (int i = pos.x - outer; i <= pos.x + outer; ++i)
            for (int j = pos.y - outer; j <= pos.y + outer; ++j)
            {
                int index = per(i, W) + per(j, H) * W;
                if (distance(vec2(i, j), pos) >= inner && distance(vec2(i, j), pos) <= outer)
                {
                    ++cellCount;
                    if (A1[index] == 1.0f)
                        ++aliveCount;
                }
            }
    }

    return aliveCount / float(cellCount);
}

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    int index = pos.x + pos.y * W;

    int inners1[MAX_SIZE] = { 0,  0,  0,  0};
    int inners2[MAX_SIZE] = { 0,  4,  8,  0};
    int inners3[MAX_SIZE] = { 0,  0,  0,  0};
    int inners4[MAX_SIZE] = { 0,  4,  9, 12};
    
    int outers1[MAX_SIZE] = {14,  0,  0,  0};
    int outers2[MAX_SIZE] = { 1,  5,  8,  0};
    int outers3[MAX_SIZE] = {10,  0,  0,  0};
    int outers4[MAX_SIZE] = { 2,  7, 10, 15};

    float density1 = neighborhoodDensity(inners1, outers1, 1, pos);
    float density2 = neighborhoodDensity(inners2, outers2, 3, pos);
    float density3 = neighborhoodDensity(inners3, outers3, 1, pos);
    float density4 = neighborhoodDensity(inners4, outers4, 4, pos);

    if (density1 >= 0.262364076538086f && density1 <= 0.902710297241211f)
    {
        A1[index] = 0.0f;
    }
    if (density1 >= 0.876029204711914f && density1 <= 0.764857985839844f)
    {
        A1[index] = 1.0f;
    }
    if (density1 >= 0.533621850585938f && density1 <= 0.911603994750977f)
    {
        A1[index] = 0.0f;
    }
    if (density1 >= 0.787092229614258f && density1 <= 0.449131724243164f)
    {
        A1[index] = 0.0f;
    }

    if (density2 >= 0.342407354125977f && density2 <= 0.377982144165039f)
    {
        A1[index] = 1.0f;
    }
    if (density2 >= 0.453578572998047f && density2 <= 0.057809033813477f)
    {
        A1[index] = 1.0f;
    }
    if (density2 >= 0.484706514282227f && density2 <= 0.671474161987305f)
    {
        A1[index] = 1.0f;
    }
    if (density2 >= 0.057809033813477f && density2 <= 0.11117121887207f)
    {
        A1[index] = 0.0f;
    }

    if (density3 >= 0.342407354125977f && density3 <= 0.382428992919922f)
    {
        A1[index] = 1.0f;
    }
    if (density3 >= 0.755964288330078f && density3 <= 0.53806869934082f)
    {
        A1[index] = 1.0f;
    }
    if (density3 >= 0.195661345214844f && density3 <= 0.217895588989258f)
    {
        A1[index] = 0.0f;
    }
    if (density3 >= 0.671474161987305f && density3 <= 0.489153363037109f)
    {
        A1[index] = 1.0f;
    }

    if (density4 >= 0.889369750976563f && density4 <= 0.978306726074219f)
    {
        A1[index] = 1.0f;
    }
    if (density4 >= 0.035574790039063f && density4 <= 0.133405462646484f)
    {
        A1[index] = 0.0f;
    }
    if (density4 >= 0.88492290222168f && density4 <= 0.760411137084961f)
    {
        A1[index] = 0.0f;
    }
    if (density4 >= 0.635899371948242f && density4 <= 0.257917227783203f)
    {
        A1[index] = 1.0f;
    }

    A2[index] = A1[index];

    vec4 col = vec4(A1[index], A1[index], A1[index], 1);
    imageStore(img, ivec2(gl_GlobalInvocationID.xy), col);
}

// float innerRadius = 7;
// float outerRadius = 12;

// int innerAliveCount = 0;
// int innerCellsCount = 0;
// int outerAliveCount = 0;
// int outerCellsCount = 0;

// for (int i = x - int(outerRadius); i < x + int(outerRadius); ++i)
//     for (int j = y - int(outerRadius); j < y + int(outerRadius); ++j)
//     {
//         int i2 = per(i, W);
//         int j2 = per(j, H);
//         int index2 = i2 + j2 * W;
//         if (distance(vec2(x, y), vec2(i, j)) <= innerRadius)
//         {
//             ++innerCellsCount;
//             if (A1[index2] == 1.0f)
//                 ++innerAliveCount;
//         }
//         if (distance(vec2(x, y), vec2(i, j)) >= innerRadius && distance(vec2(x, y), vec2(i, j)) <= outerRadius)
//         {
//             ++outerCellsCount;
//             if (A1[index2] == 1.0f)
//                 ++outerAliveCount;
//         }
//     }
// float innerDensity = float(innerAliveCount) / float(innerCellsCount);
// float outerDensity = float(outerAliveCount) / float(outerCellsCount);