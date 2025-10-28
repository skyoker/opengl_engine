#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;



struct GenWorld {

    const int TILES_PER_CHUNK;
    const int CHUNKS_PER_WORLDX;
    const int CHUNKS_PER_WORLDY;

    const fs::path FOLDER_PATH;

    void make_world_folder();
    void create_chunk_file(int chunk_x, int chunk_y);
    void generate_world();
};
