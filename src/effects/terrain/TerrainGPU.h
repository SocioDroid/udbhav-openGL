#include <cuda_runtime.h>
#include <vector>
#include <utility>
#include "../../includes/vmath.h"

void generateTileGridGPU(std::vector<std::pair<float, float>> &positionVec, std::pair<float, float> offset, int gridLength, float tileW);