#version 450
layout(binding = 0) buffer dcA1 { float A1[]; };
layout(binding = 1) buffer dcA2 { float A2[]; };
layout(rgba8, binding = 4) uniform writeonly image2D img;
layout(local_size_x = 20, local_size_y = 20, local_size_z = 1) in;

const int W = 800;
const int H = 600;

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
    int i, j;
    i = int(gl_GlobalInvocationID.x);
    j = int(gl_GlobalInvocationID.y);

    // Periodicity nad neighbors
    int idx0, idx1, idx2, idx3, idx4, idx5, idx6, idx7, idx8;
    int ip, jp, im, jm;
    ip = per(i + 1, W);
    im = per(i - 1, W);
    jp = per(j + 1, H);
    jm = per(j - 1, H);
    idx0 = i + W * j;
    idx1 = ip + W * (jp);
    idx2 = ip + W * (j); // i+1,j
    idx3 = ip + W * (jm);
    idx4 = i + W * (jm); // i,j-1
    idx5 = im + W * (jm);
    idx6 = im + W * (j); // i-1, j
    idx7 = im + W * (jp);
    idx8 = i + W * (jp); // i, j+1
    
    int neighborsCount = 0;
    neighborsCount += int(A1[idx0]);
    neighborsCount += int(A1[idx1]);
    neighborsCount += int(A1[idx2]);
    neighborsCount += int(A1[idx3]);
    neighborsCount += int(A1[idx4]);
    neighborsCount += int(A1[idx5]);
    neighborsCount += int(A1[idx6]);
    neighborsCount += int(A1[idx7]);
    neighborsCount += int(A1[idx8]);

    if (neighborsCount == 4 || neighborsCount == 6 || neighborsCount == 7 || neighborsCount == 8 || neighborsCount == 9)
        A1[idx0] = 1.0f;
    else
        A1[idx0] = 0.0f;

    A2[idx0] = A1[idx0];
    vec4 col = vec4(A1[idx0], A1[idx0], A1[idx0], 1);
    imageStore(img, ivec2(gl_GlobalInvocationID.xy), col);
}