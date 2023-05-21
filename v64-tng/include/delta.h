// delta.h

#ifndef DELTA_H
#define DELTA_H

#include <vector>

/*
===============================================================================

    7th Guest - Delta Bitmap Frames

    This header file contains x

===============================================================================
*/

const std::vector<uint8_t> PREDEFINED_COLOUR_MAP = {
    0x00, 0xc8, 0x80, 0xec, 0xc8, 0xfe, 0xec, 0xff, 0xfe, 0xff, 0x00, 0x31, 0x10, 0x73, 0x31, 0xf7,
    0x73, 0xff, 0xf7, 0xff, 0x80, 0x6c, 0xc8, 0x36, 0x6c, 0x13, 0x10, 0x63, 0x31, 0xc6, 0x63, 0x8c,
    0x00, 0xf0, 0x00, 0xff, 0xf0, 0xff, 0x11, 0x11, 0x33, 0x33, 0x77, 0x77, 0x66, 0x66, 0xcc, 0xcc,
    0xf0, 0x0f, 0xff, 0x00, 0xcc, 0xff, 0x76, 0x00, 0x33, 0xff, 0xe6, 0x0e, 0xff, 0xcc, 0x70, 0x67,
    0xff, 0x33, 0xe0, 0x6e, 0x00, 0x48, 0x80, 0x24, 0x48, 0x12, 0x24, 0x00, 0x12, 0x00, 0x00, 0x21,
    0x10, 0x42, 0x21, 0x84, 0x42, 0x00, 0x84, 0x00, 0x88, 0xf8, 0x44, 0x00, 0x32, 0x00, 0x1f, 0x11,
    0xe0, 0x22, 0x00, 0x4c, 0x8f, 0x88, 0x70, 0x44, 0x00, 0x23, 0x11, 0xf1, 0x22, 0x0e, 0xc4, 0x00,
    0x3f, 0xf3, 0xcf, 0xfc, 0x99, 0xff, 0xff, 0x99, 0x44, 0x44, 0x22, 0x22, 0xee, 0xcc, 0x33, 0x77,
    0xf8, 0x00, 0xf1, 0x00, 0xbb, 0x00, 0xdd, 0x0c, 0x0f, 0x0f, 0x88, 0x0f, 0xf1, 0x13, 0xb3, 0x19,
    0x80, 0x1f, 0x6f, 0x22, 0xec, 0x27, 0x77, 0x30, 0x67, 0x32, 0xe4, 0x37, 0xe3, 0x38, 0x90, 0x3f,
    0xcf, 0x44, 0xd9, 0x4c, 0x99, 0x4c, 0x55, 0x55, 0x3f, 0x60, 0x77, 0x60, 0x37, 0x62, 0xc9, 0x64,
    0xcd, 0x64, 0xd9, 0x6c, 0xef, 0x70, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x44, 0x44, 0x22, 0x22};

std::vector<uint8_t> getDeltaBitmapData(std::vector<uint8_t> &buffer, std::vector<uint8_t> &staticBitmap);

#endif // DELTA_H