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
            chunk_file << "    \"t" << tile_x << "x" << tile_y << "y\": {\"type\": \"NULL\"}"; // here one can set the empty tile type
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

void GenWorld::insert_tile_on_chunk(fs::path chunk_path, Vec2 tp, TileType tt)
{
    // Resolve path if needed
    if (!fs::exists(chunk_path)) {
        fs::path p = FOLDER_PATH / chunk_path;
        if (fs::exists(p)) chunk_path = p;
    }

    if (!fs::exists(chunk_path)) {
        std::cerr << "Chunk file does not exist: " << chunk_path << "\n";
        return;
    }

    // Load JSON
    std::ifstream infile(chunk_path);
    if (!infile.is_open()) {
        std::cerr << "Failed to open chunk file for reading: " << chunk_path << "\n";
        return;
    }

    nlohmann::json j;
    try {
        infile >> j;
    } catch (std::exception &e) {
        std::cerr << "JSON parse error in " << chunk_path << ": " << e.what() << "\n";
        return;
    }
    infile.close();

    int tilex = static_cast<int>(tp.x);
    int tiley = static_cast<int>(tp.y);

    // Key format: t{X}x{Y}y
    std::string key = "t" + std::to_string(tilex) +
                      "x" + std::to_string(tiley) +
                      "y";

    // Validate the tile exists
    if (!j.contains(key)) {
        std::cerr << "Tile key not found: " << key << " in " << chunk_path << "\n";
        return;
    }

    // Convert enum to string
    std::string newTypeStr = (tt == TileType::Null)
                             ? "NULL"
                             : TileTypeToString(tt);

    // Apply update safely
    j[key]["type"] = newTypeStr;

    // Save JSON back to file
    std::ofstream outfile(chunk_path, std::ios::trunc);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open chunk file for writing: " << chunk_path << "\n";
        return;
    }

    outfile << j.dump(4); // pretty-print with indentation
    outfile.close();
}

void GenWorld::random_wall_placement(float randomness) {
    int randomn;
    for (int chunkx = 0; chunkx < CHUNKS_PER_WORLDX; chunkx++) {
        for (int chunky = 0; chunky < CHUNKS_PER_WORLDY; chunky++) {
            for (int tilex = 0; tilex < TILES_PER_CHUNK; tilex++) {
                for (int tiley = 0; tiley < TILES_PER_CHUNK; tiley++) {
                    randomn = randomFromSeed(generator, 0, static_cast<int>(randomness));

                    if (randomn == static_cast<int>(randomness) - 1) {
                        std::string chstrpath = get_chunk_string(chunkx, chunky);
                        std::string chpath = std::string(FOLDER_PATH) + "/" + chstrpath;
                        Vec2 tp = {static_cast<float>(tilex), static_cast<float>(tiley)};
                        insert_tile_on_chunk(chpath, tp, TileType::Wall);
                    }
                }
            }
        }
    }
}

void GenWorld::init() {
    generator = std::mt19937(*seed);            // Mersenne Twister seeded


}