#include <curand_kernel.h>
#include <stdio.h>
#include "VertexUpdate.h"

__global__ void arrayUpdateKernel(float *vertexPosition, float *vertexColor, float *initialPosition, bool isDisintegrate, int N, float _speed, int direction, float maxDistance)
{
    if (maxDistance == 0)
        maxDistance = 2.0f;

    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    curandState_t state;
    float speed = _speed;
    if (idx < N)
    {
        int i = idx * 3;
        int iColor = idx * 4;
        float px = vertexPosition[i];
        float py = vertexPosition[i + 1];
        float pz = vertexPosition[i + 2];

        curand_init(clock64(), i, 0, &state);
        float randomVal = curand_uniform(&state);

        // Perform disintegration
        if (isDisintegrate)
        {
            switch (direction)
            {
            case DISINTEGRATE_UP:
            {
                if (py < maxDistance + 1.0f)
                {
                    vertexPosition[i] = px + 1.5 * (0.5 - randomVal) * speed * 0.1610f;
                    vertexPosition[i + 1] = py + 8.0 * (randomVal)*speed * 0.1610f;
                    vertexPosition[i + 2] = pz + 1.5 * (0.5 - randomVal) * speed * 0.1610f;
                }

                if (py > maxDistance - 2.0f)
                {
                    if (vertexColor[iColor + 3] > 0.0f)
                        vertexColor[iColor + 3] -= speed * 0.5f;
                }
            }
            break;
            case DISINTEGRATE_DOWN:
            {
                if (py > -maxDistance)
                {
                    vertexPosition[i] = px + 1.5 * (0.5 - randomVal) * speed * 0.1610f;
                    vertexPosition[i + 1] = py + 8.0 * (0.25 - randomVal) * speed * 0.1610f;
                    vertexPosition[i + 2] = pz + 1.5 * (0.5 - randomVal) * speed * 0.1610f;
                }

                if (py < -maxDistance + 3.0f)
                {
                    if (vertexColor[iColor + 3] > 0.0f)
                        vertexColor[iColor + 3] -= speed * 0.5f;
                }
            }
            break;
            case DISINTEGRATE_LEFT:
            {
                if (px > -maxDistance)
                {
                    vertexPosition[i] = px + 8.0 * (0.25 - randomVal) * speed * 0.1610f;
                    vertexPosition[i + 1] = py + 1.5 * (0.5 - randomVal) * speed * 0.1610f;
                    vertexPosition[i + 2] = pz + 1.5 * (0.5 - randomVal) * speed * 0.1610f;
                }

                if (px < -maxDistance + 1.0f)
                {
                    if (vertexColor[iColor + 3] > 0.0f)
                        vertexColor[iColor + 3] -= speed * 0.5f;
                }
            }
            break;
            case DISINTEGRATE_RIGHT:
            {
                if (px < maxDistance)
                {
                    vertexPosition[i] = px + 8.0 * (randomVal)*speed * 0.1610f;
                    vertexPosition[i + 1] = py + 1.5 * (0.5 - randomVal) * speed * 0.1610f;
                    vertexPosition[i + 2] = pz + 1.5 * (0.5 - randomVal) * speed * 0.1610f;
                }

                if (px > maxDistance - 1.0f)
                {
                    if (vertexColor[iColor + 3] > 0.0f)
                        vertexColor[iColor + 3] -= speed * 0.5f;
                }
            }
            break;
            }
        }
        // Perform Integration
        else
        {
            float ix = initialPosition[i];
            float iy = initialPosition[i + 1];
            float iz = initialPosition[i + 2];

            float dx = abs(px - ix);
            float dy = abs(py - iy);
            float dz = abs(pz - iz);

            float d = dx + dy + dx;

            switch (direction)
            {
            case DISINTEGRATE_UP:
            {
                if (d > -1.0)
                {
                    vertexPosition[i] = px - (px - ix) / dx * speed * 0.1610f * (0.85 - randomVal);
                    vertexPosition[i + 1] = py + (iy - py) / dy * speed * 0.1610f * (1.0 + randomVal);
                    vertexPosition[i + 2] = pz - (pz - iz) / dz * speed * 0.1610f * (0.85 - randomVal);
                }

                if (py < maxDistance + 1.0f)
                {
                    if (vertexColor[iColor + 3] < 1.0f)
                        vertexColor[iColor + 3] += speed * 0.5f;
                }
            }
            break;
            case DISINTEGRATE_DOWN:
            {
                if (d > -1.0)
                {
                    vertexPosition[i] = px - (px - ix) / dx * speed * 0.1610f * (0.85 - randomVal);
                    vertexPosition[i + 1] = py - (py - iy) / dy * speed * 0.1610f * (1 + randomVal);
                    vertexPosition[i + 2] = pz - (pz - iz) / dz * speed * 0.1610f * (0.85 - randomVal);
                }

                if (py > -maxDistance)
                {
                    if (vertexColor[iColor + 3] < 1.0f)
                        vertexColor[iColor + 3] += speed * 0.5f;
                }
            }
            break;
            case DISINTEGRATE_LEFT:
            {
                if (d > -1.0)
                {
                    vertexPosition[i] = px - (px - ix) / dx * speed * 0.1610f * (1.0 + randomVal);
                    vertexPosition[i + 1] = py - (py - iy) / dy * speed * 0.1610f * (0.85 - randomVal);
                    vertexPosition[i + 2] = pz - (pz - iz) / dz * speed * 0.1610f * (0.85 - randomVal);
                }

                if (px > -maxDistance)
                {
                    if (vertexColor[iColor + 3] < 1.0f)
                        vertexColor[iColor + 3] += speed * 0.5f;
                }
            }
            break;
            case DISINTEGRATE_RIGHT:
            {
                if (d > -1.0)
                {
                    vertexPosition[i] = px + (ix - px) / dx * speed * 0.1610f * (1.0 + randomVal);
                    vertexPosition[i + 1] = py - (py - iy) / dy * speed * 0.1610f * (0.85 - randomVal);
                    vertexPosition[i + 2] = pz - (pz - iz) / dz * speed * 0.1610f * (0.85 - randomVal);
                }

                if (px < maxDistance - 1.0f)
                {
                    if (vertexColor[iColor + 3] < 1.0f)
                        vertexColor[iColor + 3] += speed * 0.5f;
                }
            }
            break;
            }
        }
    }
}

void launch_arrayUpdateKernel(void *cudaPointerPosition, void *cudaPointerColor, float *initialPosition, bool isDisintegrate, int N, float _speed, int direction, float maxDistance)
{
    dim3 dimGrid = dim3((int)ceil((float)N / 256.0f), 1, 1);
    dim3 dimBlock = dim3(256, 1, 1);
    arrayUpdateKernel<<<dimGrid, dimBlock>>>((float *)cudaPointerPosition, (float *)cudaPointerColor, initialPosition, isDisintegrate, N, _speed, direction, maxDistance);
}

// REDUCE VECTOR SIZE
__global__ void reduceVectorSizeKernel(float *inputArray, int inputSize, float *outputArray, int outputSize, float multiplier)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    if (tid < outputSize)
    {
        int index = (int)(multiplier * tid);
        if (index < inputSize)
        {
            outputArray[tid * 3] = inputArray[index * 3];
            outputArray[tid * 3 + 1] = inputArray[index * 3 + 1];
            outputArray[tid * 3 + 2] = inputArray[index * 3 + 2];
        }
        else
        {
            outputArray[tid * 3] = inputArray[0];
            outputArray[tid * 3 + 1] = inputArray[1];
            outputArray[tid * 3 + 2] = inputArray[2];
        }
    }
}

void launch_reduceVectorSizeKernel(float *input, float *output, int inputSize, int outputSize)
{
    // Define thread block and grid dimensions
    int threadsPerBlock = 256;
    int blocksPerGrid = (outputSize + threadsPerBlock - 1) / threadsPerBlock;
    float step = (float)(inputSize - 1) / (outputSize - 1);

    reduceVectorSizeKernel<<<blocksPerGrid, threadsPerBlock>>>(input, inputSize / 3, output, outputSize, step);
}