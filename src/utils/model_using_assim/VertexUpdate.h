
enum DisintegrateDirection
{
    DISINTEGRATE_UP,
    DISINTEGRATE_DOWN,
    DISINTEGRATE_LEFT,
    DISINTEGRATE_RIGHT
};

void launch_arrayUpdateKernel(void *cudaPointerPosition, void *cudaPointerColor, float *initialPosition, bool isDisintegrate, int N, float _speed, int direction, float maxDistance);

void launch_reduceVectorSizeKernel(float *input, float *output, int inputSize, int outputSize);
