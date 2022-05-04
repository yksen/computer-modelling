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

void main()
{
    int x, y;
    x = int(gl_GlobalInvocationID.x);
    y = int(gl_GlobalInvocationID.y);

    int index = x + y * W;

    int innerRadius = 3;
    int outerRadius = 11;

    int innerAliveCount = 0;
    int innerCellsCount = 0;
    int outerAliveCount = 0;
    int outerCellsCount = 0;

    for (int i = x - outerRadius; i <= x + outerRadius; ++i)
        for (int j = y - outerRadius; j <= y + outerRadius; ++j)
        {
            int i2 = per(i, W);
            int j2 = per(j, H);
            int index2 = i2 + j2 * W;
            if (distance(vec2(x, y), vec2(i, j)) <= float(innerRadius))
            {
                ++innerCellsCount;
                if (A1[index2] == 1.0f)
                    ++innerAliveCount;
            }
            if (distance(vec2(x, y), vec2(i, j)) <= float(outerRadius) && distance(vec2(x, y), vec2(i, j)) > float(innerRadius))
            {
                ++outerCellsCount;
                if (A1[index2] == 1.0f)
                    ++outerAliveCount;
            }
        }
    float innerDensity = float(innerAliveCount) / float(innerCellsCount);
    float outerDensity = float(outerAliveCount) / float(outerCellsCount - innerCellsCount);

    if (innerDensity >= 0.210f && innerDensity <= 0.220f)
    {
        A1[index] = 1.0f;
    }
    if (innerDensity >= 0.350f && innerDensity <= 0.500f)
    {
        A1[index] = 0.0f;
    }
    if (innerDensity >= 0.750f && innerDensity <= 0.850f)
    {
        A1[index] = 0.0f;
    }
    if (outerDensity >= 0.100f && outerDensity <= 0.280f)
    {
        A1[index] = 0.0f;
    }
    if (outerDensity >= 0.430f && outerDensity <= 0.550f)
    {
        A1[index] = 1.0f;
    }
    if (innerDensity >= 0.120f && innerDensity <= 0.150f)
    {
        A1[index] = 0.0f;
    }

    // if (innerDensity >= 0.185f && innerDensity <= 0.200f)
    // {
    //     A1[index] = 1.0f;
    // }
    // if (innerDensity >= 0.343f && innerDensity <= 0.580f)
    // {
    //     A1[index] = 0.0f;
    // }
    // if (innerDensity >= 0.750f && innerDensity <= 0.850f)
    // {
    //     A1[index] = 0.0f;
    // }
    // if (outerDensity >= 0.150f && outerDensity <= 0.280f)
    // {
    //     A1[index] = 0.0f;
    // }
    // if (outerDensity >= 0.445f && outerDensity <= 0.680f)
    // {
    //     A1[index] = 1.0f;
    // }
    // if (innerDensity >= 0.150f && innerDensity <= 0.180f)
    // {
    //     A1[index] = 0.0f;
    // }

    // if (innerDensity >= 0.262364076538086f && innerDensity <= 0.902710297241211f)
    // {
    //     A1[index] = 0.0f;
    // }
    // if (innerDensity >= 0.876029204711914f && innerDensity <= 0.764857985839844f)
    // {
    //     A1[index] = 1.0f;
    // }
    // if (innerDensity >= 0.533621850585938f && innerDensity <= 0.911603994750977f)
    // {
    //     A1[index] = 0.0f;
    // }
    // if (innerDensity >= 0.787092229614258f && innerDensity <= 0.449131724243164f)
    // {
    //     A1[index] = 0.0f;
    // }

    // if (outerDensity >= 0.342407354125977f && outerDensity <= 0.377982144165039f)
    // {
    //     A1[index] = 1.0f;
    // }
    // if (outerDensity >= 0.453578572998047f && outerDensity <= 0.057809033813477f)
    // {
    //     A1[index] = 1.0f;
    // }
    // if (outerDensity >= 0.484706514282227f && outerDensity <= 0.671474161987305f)
    // {
    //     A1[index] = 1.0f;
    // }
    // if (outerDensity >= 0.057809033813477f && outerDensity <= 0.11117121887207f)
    // {
    //     A1[index] = 0.0f;
    // }

    A2[index] = A1[index];

    vec4 col = vec4(A1[index], A1[index], A1[index], 1);
    imageStore(img, ivec2(gl_GlobalInvocationID.xy), col);
}