#include "textures.hpp"
#include "cache.hpp"
#include <iostream>

#include "stb_image.h"

// Load PNG file into raw pixel data
bool loadImageData(const fs::path& path, int& width, int& height, int& channels, std::vector<unsigned char>& pixels) {
    if (!fs::exists(path)) {
        std::cerr << "File not found: " << path << '\n';
        return false;
    }
    
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    
    if (!data) {
        std::cerr << "Failed to load: " << path << " - " << stbi_failure_reason() << '\n';
        return false;
    }
    
    pixels.assign(data, data + (width * height * 4));
    stbi_image_free(data);
    
    return true;
}

// Convert pixel data to batch-ready vertices
std::vector<float> pixelsToVertices(const std::vector<unsigned char>& pixels, int width, int height) {
    std::vector<float> vertices;
    vertices.reserve(width * height * 30);  // 6 verts * 5 floats per pixel
    // Produce local UV-based vertices (u, v, r, g, b) where u,v are in [0,1].
    // This lets DrawtexWindow transform these to tile NDC positions per-tile.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 4;
            float r = pixels[idx] / 255.0f;
            float g = pixels[idx + 1] / 255.0f;
            float b = pixels[idx + 2] / 255.0f;

            // UV coordinates (u ranges left->right, v ranges bottom->top)
            float u0 = static_cast<float>(x) / static_cast<float>(width);
            float u1 = static_cast<float>(x + 1) / static_cast<float>(width);
            // flip vertical so v=0 is bottom
            float v0 = 1.0f - static_cast<float>(y + 1) / static_cast<float>(height);
            float v1 = 1.0f - static_cast<float>(y) / static_cast<float>(height);

            // Triangle 1: TL(u0,v1), BL(u0,v0), TR(u1,v1)
            vertices.insert(vertices.end(), {
                u0, v1,  r, g, b,
                u0, v0,  r, g, b,
                u1, v1,  r, g, b
            });

            // Triangle 2: BR(u1,v0), TR(u1,v1), BL(u0,v0)
            vertices.insert(vertices.end(), {
                u1, v0,  r, g, b,
                u1, v1,  r, g, b,
                u0, v0,  r, g, b
            });
        }
    }
    
    return vertices;
}

Texture TextureMng::Loadtex(fs::path path) {
    Texture search_tex(path, TileType::Unknown, std::vector<float>());
    
    // Check cache first
    if (cache && cache->is_tex_loaded(search_tex)) {
        for (const auto& cached : cache->loaded_textures.textures) {
            if (cached.path_to_image == search_tex.path_to_image) {
                return cached;
            }
        }
    }
    
    
    int width, height, channels;
    std::vector<unsigned char> pixels;
    
    if (!loadImageData(path, width, height, channels, pixels)) {
        return Texture(path, TileType::Unknown, std::vector<float>(), 0, 0);
    }
    
    // Convert to vertices
    std::vector<float> vertices = pixelsToVertices(pixels, width, height);
    
    // Create and cache texture with width/height
    Texture new_tex(path, TileType::Unknown, vertices, width, height);  // TODO: Determine TileType from filename
    
    if (cache) {
        cache->add_tex_to_cache(new_tex);
    }
    
    return new_tex;
}