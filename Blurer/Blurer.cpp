
#include <complex>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>

#include "Targa.h"

constexpr int MAX_BLUR = 15;

// Converts the input blur value to a blur factor in the range [0, MAX_BLUR].
int get_blur_factor(const char* blur_value)
{
    char* output;
    double blur_float = std::strtod(blur_value, &output);

    // Ensure the value is in the range [0, 1]
    blur_float = std::max(0.0, std::min(blur_float, 1.0));

    // Convert the value to the desired range [0, MAX_BLUR]
    const int blur_factor = static_cast<int>(std::round(blur_float * MAX_BLUR));

    return blur_factor;
}

// Applies a blur effect to the input Targa image & returns a new Targa image with the applied blur effect.
Targa blur_image(const Targa& targa, int blur)
{
    Targa new_targa = targa;
    const int area = static_cast<int>(std::pow(2 * blur + 1, 2));
    
    // Iterate through each pixel in the image.
    for (int i = 0; i < targa.width; ++i)
    {
        for (int j = 0; j < targa.height; ++j)
        {
            std::tuple<int, int, int> sum_of_pixels = std::make_tuple(0, 0, 0);
            for (int x = std::max(0, i - blur); x < i + blur + 1 && x < targa.width; ++x)
            {
                for (int y = std::max(0, j - blur); y < j + blur + 1 && y < targa.height; y++)
                {
                    const Pixel& pixel = targa.image_data[targa.width * x + y];
                    std::get<0>(sum_of_pixels) += static_cast<int>(pixel.red);
                    std::get<1>(sum_of_pixels) += static_cast<int>(pixel.green);
                    std::get<2>(sum_of_pixels) += static_cast<int>(pixel.blue);
                }
            }
            
            // Calculate average color values and update the new pixel.
            Pixel& new_pixel = new_targa.image_data[i * targa.width + j];
            new_pixel.red = static_cast<UCHAR>(std::get<0>(sum_of_pixels) / area);
            new_pixel.green = static_cast<UCHAR>(std::get<1>(sum_of_pixels) / area);
            new_pixel.blue = static_cast<UCHAR>(std::get<2>(sum_of_pixels) / area);
        }
    }
    return new_targa;
}

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "Usage: " << argv[0] << " <image_path> <new_image_path> <blur_value>" << std::endl;
        return -1;
    }
    
    const std::string image_path = argv[1];
    const std::string new_image_path = argv[2];

    if (image_path.find(".tga")  == std::string::npos || new_image_path.find(".tga") == std::string::npos)
    {
        std::cout << "Both image paths must be TGA images!" << std::endl;
        return 1;
    }
    
    const Targa targa(image_path.c_str());
    const int blur_factor = get_blur_factor(argv[3]);
    const Targa new_targa = blur_image(targa, blur_factor);
    new_targa.write(new_image_path.c_str());
    
    return 0;
}
