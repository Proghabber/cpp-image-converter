#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>
#include <bmp_image.h>

#include <filesystem>
#include <string_view>
#include <iostream>

using namespace std;

enum Format {UNKNOWN, PPM, JPEG, BMP};

Format GetFormatByExtension(const img_lib::Path& input_file) {
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return Format::JPEG;
    }

    if (ext == ".ppm"sv) {
        return Format::PPM;
    }

    if (ext == ".bmp"sv) {
        return Format::BMP;
    }

    return Format::UNKNOWN;
} 



class ImageFormatInterface {
public:
    virtual bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const = 0;
    virtual img_lib::Image LoadImage(const img_lib::Path& file) const = 0;
}; 

class ImageFormatJpeg :public ImageFormatInterface{
    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image)const  override{
        return img_lib::jpeg::SaveJPEG(file, image);
    }
    img_lib::Image LoadImage(const img_lib::Path& file)const override{
        return img_lib::jpeg::LoadJPEG(file);
    }
};

class ImageFormatMPP :public ImageFormatInterface{
    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image)const  override{
        return img_lib::ppm::SavePPM(file, image);
    }
    img_lib::Image LoadImage(const img_lib::Path& file)const override{
        return img_lib::ppm::LoadPPM(file);
    }
};

class ImageFormatBMP :public ImageFormatInterface{
    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image)const  override{
        return img_lib::bmp::SaveBMP(file, image);
    }
    img_lib::Image LoadImage(const img_lib::Path& file)const override{
        return img_lib::bmp::LoadBMP(file);
    }
};

ImageFormatInterface* GetFormatInterface(const img_lib::Path& path){
    auto mat = GetFormatByExtension(path);
    static ImageFormatJpeg jpeg;
    static ImageFormatMPP mpp;
    static ImageFormatBMP bmp;
    switch (mat){
        case BMP: return &bmp;
        break;
        case PPM: return &mpp;
        break;
        case JPEG: return &jpeg;
        break;

    }
    return nullptr;
    
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return 1;
    }

    img_lib::Path in_path = argv[1];
    img_lib::Path out_path = argv[2];

    Format in_format = GetFormatByExtension(in_path);
    Format out_format = GetFormatByExtension(out_path);

    if (in_format == Format::UNKNOWN){
        cerr << "Unknown format of the input file"sv << endl;
        return 2;
    }

    if (out_format == Format::UNKNOWN){
        cerr << "Unknown format of the output file"sv << endl;
        return 3;
    }

    ImageFormatInterface* interface = GetFormatInterface(in_path);
    
    
    if (interface == nullptr) {
        cerr << "Loading failed"sv << endl;
        return 4;
    }
    img_lib::Image image = interface->LoadImage(in_path);
    interface = GetFormatInterface(out_path);
    if (!interface->SaveImage(out_path, image)) {
        cerr << "Saving failed"sv << endl;
        return 5;
    }

    cout << "Successfully converted"sv << endl;
}