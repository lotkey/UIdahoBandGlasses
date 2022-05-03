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
    transmitter::Image image = bmp.toImage();
    bmp = transmitter::BMP(img);
    transmitter::Image img2(1, 1);
    img2 = transmitter::Image(1, 1, {255, 255, 255});
    bmp.save("../imgs/output.bmp");
    return 0;
}