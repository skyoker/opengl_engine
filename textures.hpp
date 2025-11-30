#pragma once
#include "utils.hpp"
#include <filesystem>
#include <vector>

// Forward declare Cache to avoid circular dependency
struct Cache;

namespace fs = std::filesystem;


struct TextureMng {
    Cache* cache = nullptr;  // Pointer to cache (set this before using)
    
    Texture Loadtex(fs::path path);  // Load texture, checking cache first
};