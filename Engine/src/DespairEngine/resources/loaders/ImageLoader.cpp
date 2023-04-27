//#include "PrecompiledHeader.hpp"
//#include "ImageLoader.hpp"
//
//#include <soil/src/SOIL.h>
//
//#include "../Image.hpp"
//
//bool engine::resources::loaders::ImageLoader::load(Image& image, std::string_view image_path) noexcept
//{
//    int image_width = 0;
//    int image_height = 0;
//    unsigned char* image_data = SOIL_load_image(image_path.data(), &image_width, &image_height, 0, SOIL_LOAD_RGB);
//    if (image_data)
//    {
//        image.setData(image_data);
//        image.setWidth(static_cast<std::uint16_t>(image_width));
//        image.setHeight(static_cast<std::uint16_t>(image_height));
//        return true;
//    }
//
//    return false;
//}