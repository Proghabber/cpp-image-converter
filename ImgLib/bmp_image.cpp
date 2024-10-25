#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>
#include <iostream>



using namespace std;

namespace img_lib {
namespace bmp {
    PACKED_STRUCT_BEGIN BitmapFileHeader {
    // поля заголовка Bitmap File Header
    char name[2];
    uint32_t size_hed_data;
    uint32_t space = 0;
    uint32_t space_start;
    }
    PACKED_STRUCT_END

    PACKED_STRUCT_BEGIN BitmapInfoHeader {
        // поля заголовка Bitmap Info Header
        uint32_t size_head;
        uint32_t width_pixels;
        uint32_t height_pixels;
        short int count_layer = 1;
        short int count_bit_pixels = 24;
        uint32_t type_press = 0;
        uint32_t count_byte_data;
        uint32_t horizont_pixels_metre = 11811;
        uint32_t vertical_pixels_metre = 11811;
        uint32_t count_using_colors = 0;
        uint32_t count_main_colors = 0x1000000;
    }
    PACKED_STRUCT_END
    // функция вычисления отступа по ширине
    static int GetBMPStride(int w) {
        return 4 * ((w * 3 + 3) / 4);
    }

    
    bool SaveBMP(const Path& file, const Image& image){
        std::ofstream ofs(file, ios::binary);
        if (!ofs){
            return false;
        }
        BitmapFileHeader bfh;
        BitmapInfoHeader bih;
        size_t height = image.GetHeight();
        size_t weight = image.GetWidth();
        size_t pedding = GetBMPStride(image.GetStep());
        //----------------
        bfh.name[0] = 'B';
        bfh.name[1] = 'M';
        bfh.size_hed_data = height * pedding + sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
        bfh.space = 0;
        bfh.space_start = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
        //----------------
        bih.size_head = sizeof(BitmapInfoHeader);
        bih.width_pixels = weight;
        bih.height_pixels = height;
        bih.count_byte_data = height * pedding;
        //--------------
        ofs.write((char*)&bfh, sizeof(BitmapFileHeader));
        ofs.write((char*)&bih, sizeof(BitmapInfoHeader));
        std::vector<char> buff(pedding);
        for(int h = height - 1; h >= 0; --h){
            const Color* line = image.GetLine(h);
            for (int x = 0; x < weight; ++x) {
                buff[x * 3  + 0] = static_cast<char>(line[x].b);
                buff[x * 3  + 1] = static_cast<char>(line[x].g);
                buff[x * 3  + 2] = static_cast<char>(line[x].r);
            }
            ofs.write(buff.data(), pedding);   
        }
        ofs.close();
        return ofs.good();
    }

    Image LoadBMP(const Path& file){
        BitmapFileHeader bfh;
        BitmapInfoHeader bih;

        std::ifstream ifs(file, ios::binary);
        ifs.read((char*)&bfh, sizeof(bfh));
        ifs.read((char*)&bih, sizeof(bih));

        int pedding = GetBMPStride(bih.width_pixels);
        int height = bih.height_pixels;
        Image result(bih.width_pixels, height, Color::Black());

        std::vector<char> buff(pedding);
        for(int h = height - 1; h >= 0; --h){  
            Color* line = result.GetLine(h);
            ifs.read(buff.data(), pedding);
            for (int x = 0; x <  bih.width_pixels; ++x) {
                line[x].b = static_cast<byte>(buff[x * 3 + 0]);
                line[x].g = static_cast<byte>(buff[x * 3 + 1]);
                line[x].r = static_cast<byte>(buff[x * 3 + 2]);
            }   
        }
        ifs.close();
        return result;
    }
}

}  // namespace img_lib