# Texture Loading & File Format Guide

## Best File Formats for Your Use Case

Since `drawBatch()` just needs vertices in format `[x, y, r, g, b, x, y, r, g, b, ...]`, here are your options ranked by **ease + performance**:

### 🥇 **Option 1: PNG (Recommended for Development)**
**Pros:**
- Easy to create in any image editor
- Small file size (compressed)
- Perfect for pixel art with GL_NEAREST filtering
- Load with stb_image (single header file)

**Cons:**
- Need to decode at runtime (stb_image is fast though)
- Each frame, pixels → vertices conversion happens

**Load Time:** ~1-5ms for 32x32 PNG (acceptable)

### 🥈 **Option 2: Raw Binary Format (.raw or custom)**
**Pros:**
- Zero loading time (direct pixel data)
- Smallest at runtime
- Can skip decoding step

**Cons:**
- Not compressed (4KB per 32x32 RGBA image)
- Can't view/edit with normal tools
- Need custom encoder/decoder

### 🥉 **Option 3: KTX2 (Best for Large Games)**
**Pros:**
- GPU-compressed format
- Tiniest files
- Some support hardware decompression

**Cons:**
- Overkill for pixel art tiles
- Need external tool to create

---

## Implementation: PNG → Vertices

Here's how to convert your 32×32 PNG into vertex data:

### Step 1: Load PNG with stb_image

```cpp
// In textures.cpp
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool loadImageData(const fs::path& path, int& width, int& height, int& channels, std::vector<unsigned char>& pixels) {
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);  // Force RGBA
    
    if (!data) {
        std::cerr << "Failed to load: " << path << " - " << stbi_failure_reason() << '\n';
        return false;
    }
    
    // Copy to vector
    pixels.assign(data, data + (width * height * 4));
    stbi_image_free(data);
    
    return true;
}
```

### Step 2: Convert Pixels to Vertices

```cpp
std::vector<float> pixelsToVertices(const std::vector<unsigned char>& pixels, 
                                     int width, int height) {
    std::vector<float> vertices;
    
    // Each pixel becomes a small quad (2 triangles = 6 vertices)
    // Total: width * height * 6 vertices * 5 floats per vertex
    vertices.reserve(width * height * 30);
    
    float pixel_width = 2.0f / width;    // Normalize to screen space
    float pixel_height = 2.0f / height;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Get pixel color
            int idx = (y * width + x) * 4;  // RGBA
            float r = pixels[idx] / 255.0f;
            float g = pixels[idx + 1] / 255.0f;
            float b = pixels[idx + 2] / 255.0f;
            // float a = pixels[idx + 3] / 255.0f;  // Alpha (optional)
            
            // Position in NDC
            float px = -1.0f + (x * pixel_width);
            float py = -1.0f + (y * pixel_height);
            
            // Quad corners
            float x0 = px;
            float x1 = px + pixel_width;
            float y0 = py;
            float y1 = py + pixel_height;
            
            // Triangle 1: TL, BL, TR
            vertices.insert(vertices.end(), {
                x0, y1,  r, g, b,  // TL
                x0, y0,  r, g, b,  // BL
                x1, y1,  r, g, b   // TR
            });
            
            // Triangle 2: BR, TR, BL
            vertices.insert(vertices.end(), {
                x1, y0,  r, g, b,  // BR
                x1, y1,  r, g, b,  // TR
                x0, y0,  r, g, b   // BL
            });
        }
    }
    
    return vertices;
}
```

### Step 3: Integrate into Texture Loading

Update your `TextureMng::Loadtex()`:

```cpp
Texture TextureMng::Loadtex(fs::path path) {
    // Check cache
    if (cache && cache->is_tex_loaded(search_tex)) {
        return cached_texture;  // From cache
    }
    
    // Load PNG from disk
    int width, height, channels;
    std::vector<unsigned char> pixels;
    
    if (!loadImageData(path, width, height, channels, pixels)) {
        std::cerr << "Failed to load: " << path << '\n';
        return Texture(path, TileType::Unknown, std::vector<float>());
    }
    
    // Convert pixels to vertices
    std::vector<float> vertices = pixelsToVertices(pixels, width, height);
    
    // Create texture with vertex data
    Texture new_tex(path, TileType::Rock, vertices);  // Set actual TileType here
    
    // Cache it
    if (cache) {
        cache->add_tex_to_cache(new_tex);
    }
    
    return new_tex;
}
```

---

## Full Implementation in textures.cpp

```cpp
#include "textures.hpp"
#include "cache.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
    
    std::cout << "Loaded image: " << path << " (" << width << "x" << height << ")\n";
    return true;
}

// Convert pixel data to batch-ready vertices
std::vector<float> pixelsToVertices(const std::vector<unsigned char>& pixels, 
                                     int width, int height) {
    std::vector<float> vertices;
    vertices.reserve(width * height * 30);  // 6 verts * 5 floats per pixel
    
    float pixel_width = 2.0f / width;
    float pixel_height = 2.0f / height;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = (y * width + x) * 4;
            float r = pixels[idx] / 255.0f;
            float g = pixels[idx + 1] / 255.0f;
            float b = pixels[idx + 2] / 255.0f;
            
            float x0 = -1.0f + (x * pixel_width);
            float x1 = x0 + pixel_width;
            float y0 = -1.0f + (y * pixel_height);
            float y1 = y0 + pixel_height;
            
            // Triangle 1
            vertices.insert(vertices.end(), {
                x0, y1,  r, g, b,
                x0, y0,  r, g, b,
                x1, y1,  r, g, b
            });
            
            // Triangle 2
            vertices.insert(vertices.end(), {
                x1, y0,  r, g, b,
                x1, y1,  r, g, b,
                x0, y0,  r, g, b
            });
        }
    }
    
    return vertices;
}

Texture TextureMng::Loadtex(fs::path path) {
    Texture search_tex(path, TileType::Unknown, std::vector<float>());
    
    // Check cache first
    if (cache && cache->is_tex_loaded(search_tex)) {
        std::cout << "Cache hit: " << path << '\n';
        for (const auto& cached : cache->loaded_textures.textures) {
            if (cached.path_to_image == search_tex.path_to_image) {
                return cached;
            }
        }
    }
    
    // Load from disk
    std::cout << "Loading from disk: " << path << '\n';
    
    int width, height, channels;
    std::vector<unsigned char> pixels;
    
    if (!loadImageData(path, width, height, channels, pixels)) {
        return Texture(path, TileType::Unknown, std::vector<float>());
    }
    
    // Convert to vertices
    std::vector<float> vertices = pixelsToVertices(pixels, width, height);
    
    // Create and cache texture
    Texture new_tex(path, TileType::Unknown, vertices);  // TODO: Determine TileType from filename
    
    if (cache) {
        cache->add_tex_to_cache(new_tex);
        std::cout << "Cached: " << path << " (" << vertices.size() << " vertex floats)\n";
    }
    
    return new_tex;
}
```

---

## File Format Recommendation

**For your 32×32 tiles, use PNG:**
- ✅ Easy to work with
- ✅ Small files (few KB each)
- ✅ Universal image editor support
- ✅ stb_image is simple & reliable

**File Layout:**
```
textures/
  ├── wall.png        (32x32)
  ├── floor.png       (32x32)
  ├── rock.png        (32x32)
  └── water.png       (32x32)
```

**Loading:**
```cpp
TextureMng texMng;
texMng.cache = &my_cache;

Texture rock = texMng.Loadtex("textures/rock.png");
// rock.vertices now contains all the vertex data ready for drawBatch()

engine->drawBatch(rock.vertices);
```

---

## Performance Notes

- **PNG load + decode:** ~1-5ms per image
- **Pixel → vertex conversion:** ~0.5ms per 32×32 image
- **Total:** ~2-10ms per texture (one-time, then cached)
- **drawBatch() call:** <1ms (1 GPU draw call)

This is **very fast** since it's only done once per texture and then cached!
