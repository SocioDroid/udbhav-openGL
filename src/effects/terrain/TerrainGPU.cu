#include "TerrainGPU.h"

__global__ void generateTileGridKernel(float *positions, float offset_x, float offset_y, int gridLength, float tileW)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i < gridLength && j < gridLength)
    {
        float sc = tileW;

        float I_x = 1.0f * sc;
        float I_y = 0.0f;
        float J_x = 0.0f;
        float J_y = 1.0f * sc;

        float pos_x = (float)(j - gridLength / 2) * I_x + (float)(i - gridLength / 2) * J_x;
        float pos_y = (float)(j - gridLength / 2) * I_y + (float)(i - gridLength / 2) * J_y;

        int index = i * gridLength + j;
        positions[index * 2] = pos_x + offset_x;
        positions[index * 2 + 1] = pos_y + offset_y;
    }
}

void generateTileGridGPU(std::vector<std::pair<float, float>> &positionVec, std::pair<float, float> offset, int gridLength, float tileW)
{
    float *d_positions;
    size_t size = positionVec.size() * 2 * sizeof(float);

    cudaMalloc(&d_positions, size);

    dim3 blockDim(16, 16);
    dim3 gridDim((gridLength + blockDim.x - 1) / blockDim.x, (gridLength + blockDim.y - 1) / blockDim.y);

    generateTileGridKernel<<<gridDim, blockDim>>>(d_positions, offset.first, offset.second, gridLength, tileW);
    cudaDeviceSynchronize();

    cudaMemcpy(positionVec.data(), d_positions, size, cudaMemcpyDeviceToHost);

    cudaFree(d_positions);
}
