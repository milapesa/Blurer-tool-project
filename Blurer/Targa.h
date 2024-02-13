#pragma once

#include <fstream>
#include <vector>

typedef signed char SCHAR;
typedef unsigned short int USINT;
typedef unsigned char UCHAR;

struct Pixel
{
    UCHAR red = 0;
    UCHAR green = 0;
    UCHAR blue = 0;
};

class Targa
{
public:
    Targa(const char* file_path);

    // Writes the Targa image to a new TGA file.
    void write(const char* file_path) const;
    
    char id_length; // no. of chars in id field.
    char color_map_type; // 1 = map 0 = no map
    char image_type; // 1 = color mapped, 2 = rgb, 3 = b&w. All uncompressed
    USINT color_map_start; // index of first entry (lo-hi)
    USINT color_map_num_entries; // no. of color map entries (lo-hi)
    char bit_per_entry; // 16 for 16bit, 24 for 24bit...
    USINT x_origin; // lower left corner (lo-hi)
    USINT y_origin; // lower left corner (lo-hi)
    USINT width; // width of image (lo-hi)
    USINT height; // height of image (lo-hi)
    char bits_per_pixel; // 16 for 16bit, 24 for 24bit...
    std::vector<Pixel> image_data; // data stored differently depending on bitrate and colormap etc.
};