#include "BMP.hpp"
#include "Mask.hpp"
#include "colors.hpp"
#include "common/common.hpp"

#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    transmitter::BMP bmp("../imgs/test.bmp");
    auto img = bmp.toImage().compress();
    bmp = transmitter::BMP(img);
    bmp.save("../imgs/output.bmp");
    return 0;
}