#include "gen_world.hpp"

void GenWorld::make_world_folder() {

    if (fs::exists(FOLDER_PATH)) {
        // Remove existing folder and all its contents
        fs::remove_all(FOLDER_PATH);

    }

    // Create folder
    fs::create_directory(FOLDER_PATH);
}


void GenWorld::create_chunk_file(int chunk_x, int chunk_y) {
    std::string file_name = "ch" + std::to_string(chunk_x) + "x" + std::to_string(chunk_y) + "y.json";
    fs::path file_path = FOLDER_PATH / file_name;

    std::ofstream chunk_file(file_path); // Create and open the file
    if (!chunk_file.is_open()) {
        std::cerr << "Failed to create chunk file: " << file_path << "\n";
        return;
    }

    chunk_file << "{\n";
    for (int tile_x = 0; tile_x < TILES_PER_CHUNK; ++tile_x) {
        for (int tile_y = 0; tile_y < TILES_PER_CHUNK; ++tile_y) {
            chunk_file << "    \"t" << tile_x << "x" << tile_y << "y\": {\"type\": \"NULL\"}";
            if (!(tile_x == TILES_PER_CHUNK - 1 && tile_y == TILES_PER_CHUNK - 1)) {
                chunk_file << ",";
            }
            chunk_file << "\n";
        }
    }
    chunk_file << "}\n";
    chunk_file.close();

}

void GenWorld::create_meta_file(int tiles_per_chunk, int chunks_per_worldx, int chunks_per_worldy) {
    std::string filename = "world_info.json";
    fs::path file_path = FOLDER_PATH / filename;

    std::ofstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to create meta file: " << file_path << "\n";
        return;
    }

    file << "{\n";
    file << "    \"tiles_per_chunk\": " << tiles_per_chunk << ",\n";
    file << "    \"chunks_per_worldx\": " << chunks_per_worldx << ",\n";
    file << "    \"chunks_per_worldy\": " << chunks_per_worldy << "\n";
    file << "}\n";

    file.close();
}
void GenWorld::generate_world() {
    make_world_folder();
    create_meta_file(TILES_PER_CHUNK, CHUNKS_PER_WORLDX, CHUNKS_PER_WORLDY);

    for (int chunk_x = 0; chunk_x < CHUNKS_PER_WORLDX; ++chunk_x) {
        for (int chunk_y = 0; chunk_y < CHUNKS_PER_WORLDY; ++chunk_y) {
            create_chunk_file(chunk_x, chunk_y);
        }
    }

}

