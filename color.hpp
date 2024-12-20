#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"

using color = vec3;

std::string write_color(const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components.
    return std::to_string(rbyte) + ' ' + std::to_string(gbyte) + ' ' + std::to_string(bbyte) + '\n';
}

#endif