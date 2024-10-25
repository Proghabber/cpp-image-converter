#pragma once
#include "img_lib.h"

#include <filesystem>

namespace img_lib {
using Path = std::filesystem::path;
namespace ppm{
    bool SavePPM(const Path& file, const Image& image);
    Image LoadPPM(const Path& file);
}
}  // namespace img_lib