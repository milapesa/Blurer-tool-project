#include "Targa.h"

Targa::Targa(const char* file_path)
{
    std::fstream h_file(file_path, std::ios::in | std::ios::binary);
    if (!h_file.is_open())
    {
        throw std::invalid_argument("File Not Found.");
    }

    h_file.read(&id_length, sizeof(id_length));
    h_file.read(&color_map_type, sizeof(color_map_type));
    h_file.read(&image_type, sizeof(image_type));
    h_file.read(reinterpret_cast<char*>(&color_map_start), sizeof(color_map_start));
    h_file.read(reinterpret_cast<char*>(&color_map_num_entries), sizeof(color_map_num_entries));
    h_file.read(&bit_per_entry, sizeof(bit_per_entry));
    h_file.read(reinterpret_cast<char*>(&x_origin), sizeof(x_origin));
    h_file.read(reinterpret_cast<char*>(&y_origin), sizeof(y_origin));
    h_file.read(reinterpret_cast<char*>(&width), sizeof(width));
    h_file.read(reinterpret_cast<char*>(&height), sizeof(height));
    h_file.read(&bits_per_pixel, sizeof(bits_per_pixel));

    const int image_data_size = width * height;
    image_data.reserve(image_data_size);
    for (int i = 0; i < image_data_size; ++i)
    {
        Pixel p;
        h_file.read(reinterpret_cast<char*>(&p.blue), sizeof(unsigned char));
        h_file.read(reinterpret_cast<char*>(&p.green), sizeof(unsigned char));
        h_file.read(reinterpret_cast<char*>(&p.red), sizeof(unsigned char));
        image_data.push_back(p);
    }

    h_file.close();
}

void Targa::write(const char* file_path) const
{
    std::fstream file;
    file.open(file_path, std::ios::out | std::ios::binary);
    if (!file)
    {
        throw std::invalid_argument("File Not Found.");
    }

    // Write TGA header information.
    file.write(&id_length, sizeof(id_length));
    file.write(&color_map_type, sizeof(color_map_type));
    file.write(&image_type, sizeof(image_type));
    file.write(reinterpret_cast<const char*>(&color_map_start), sizeof(color_map_start));
    file.write(reinterpret_cast<const char*>(&color_map_num_entries), sizeof(color_map_num_entries));
    file.write(&bit_per_entry, sizeof(bit_per_entry));
    file.write(reinterpret_cast<const char*>(&x_origin), sizeof(x_origin));
    file.write(reinterpret_cast<const char*>(&y_origin), sizeof(y_origin));
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));
    file.write(&bits_per_pixel, sizeof(bits_per_pixel));

    // Write each pixel's color values to the file.
    for (const Pixel& pixel : image_data)
    {
        file.write(reinterpret_cast<const char*>(&pixel.blue), sizeof(pixel.blue));
        file.write(reinterpret_cast<const char*>(&pixel.green), sizeof(pixel.green));
        file.write(reinterpret_cast<const char*>(&pixel.red), sizeof(pixel.red));
    }

    file.close();
}


