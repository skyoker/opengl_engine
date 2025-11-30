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
            chunk_file << "    \"t" << tile_x << "x" << tile_y << "y\": {\"type\": \"" << TileTypeToString(TileType::Null) << "\"}"; // here one can set the empty tile type
            if (!(tile_x == TILES_PER_CHUNK - 1 && tile_y == TILES_PER_CHUNK - 1)) {
                chunk_file << ",";
            }
            chunk_file << "\n";
        }
    }
    chunk_file << "}\n";
    chunk_file.close();

}

void GenWorld::create_meta_file() {
    std::string filename = "world_info.json";
    fs::path file_path = FOLDER_PATH / filename;

    // Ensure folder exists (creates parents if needed).
    std::error_code ec;
    fs::create_directories(FOLDER_PATH, ec);
    if (ec) {
        std::cerr << "Failed to create world folder " << FOLDER_PATH << ": " << ec.message() << "\n";
        return;
    }

    std::ofstream file(file_path, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Failed to create meta file: " << file_path << "\n";
        return;
    }

    file << "{\n";
    file << "    \"tiles_per_chunk\": " << TILES_PER_CHUNK << ",\n";
    file << "    \"chunks_per_worldx\": " << CHUNKS_PER_WORLDX << ",\n";
    file << "    \"chunks_per_worldy\": " << CHUNKS_PER_WORLDY << ",\n";
    file << "    \"spawnpoint_x\": " << SPAWN_PLAYERX << ",\n";
    file << "    \"spawnpoint_y\": " << SPAWN_PLAYERY << ",\n";
    if (seed) {
        file << "    \"seed\": " << *seed << "\n";
    } else {
        file << "    \"seed\": 0\n";
    }
    file << "}\n";
}

void GenWorld::generate_empty_world() {
    // Create (or recreate) the world folder and all chunk files, then write
    // the meta file into that folder.
    make_world_folder();

    for (int chunk_x = 0; chunk_x < CHUNKS_PER_WORLDX; ++chunk_x) {
        for (int chunk_y = 0; chunk_y < CHUNKS_PER_WORLDY; ++chunk_y) {
            create_chunk_file(chunk_x, chunk_y);
        }
    }

    // Write metadata after creating the folder and chunk files so it isn't
    // removed by make_world_folder.
    create_meta_file();

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

Room GenWorld::gen_new_no_overlap_room(Rooms existing_rooms) {
    // Generate candidate rooms until we find one that doesn't overlap
    int attempts = 0;
    
    while (attempts < *max_room_attempts) {
        // Generate a random room using existing gen_room logic
        Room cand;
        
        // Defensive: ensure pointers are valid
        int min_sz = (min_room_size) ? *min_room_size : 1;
        int max_sz = (max_room_size) ? *max_room_size : 1;
        if (min_sz > max_sz) std::swap(min_sz, max_sz);

        cand.width = randomFromSeed(generator, min_sz, max_sz);
        cand.height = randomFromSeed(generator, min_sz, max_sz);

        int half_max = max_sz / 2;

        int min_x = 0 + ((world_border) ? *world_border : 0) + half_max;
        int max_x = (CHUNKS_PER_WORLDX * TILES_PER_CHUNK) - (((world_border) ? *world_border : 0) + half_max);
        int min_y = 0 + ((world_border) ? *world_border : 0) + half_max;
        int max_y = (CHUNKS_PER_WORLDY * TILES_PER_CHUNK) - (((world_border) ? *world_border : 0) + half_max);

        // Clamp ranges to valid values
        if (min_x > max_x) { int c = (min_x + max_x) / 2; min_x = max_x = c; }
        if (min_y > max_y) { int c = (min_y + max_y) / 2; min_y = max_y = c; }

        cand.world_posx = randomFromSeed(generator, min_x, max_x);
        cand.world_posy = randomFromSeed(generator, min_y, max_y);

        // Check for overlap with existing rooms
        bool conflict = false;
        int half_w = cand.width / 2;
        int half_h = cand.height / 2;
        int left = cand.world_posx - half_w;
        int right = cand.world_posx + half_w;
        int bottom = cand.world_posy - half_h;
        int top = cand.world_posy + half_h;

        for (const Room& r : existing_rooms.rooms) {
            int r_half_w = r.width / 2;
            int r_half_h = r.height / 2;
            int r_left = r.world_posx - r_half_w - ((room_border) ? *room_border : 0);
            int r_right = r.world_posx + r_half_w + ((room_border) ? *room_border : 0);
            int r_bottom = r.world_posy - r_half_h - ((room_border) ? *room_border : 0);
            int r_top = r.world_posy + r_half_h + ((room_border) ? *room_border : 0);

            // Check A vs B overlap (including border). If rectangles intersect, it's a conflict.
            bool no_overlap = (right < r_left) || (left > r_right) || (top < r_bottom) || (bottom > r_top);
            if (!no_overlap) {
                conflict = true;
                break;
            }
        }

        if (!conflict) {
            return cand;
        }

        attempts++;
    }

    // If we've exhausted attempts, return a default/empty room
    std::cerr << "Warning: could not generate non-overlapping room after " << *max_room_attempts << " attempts\n";
    return Room{0, 0, 0, 0};
}

Rooms GenWorld::gen_rooms_no_overlap() {
    Rooms out;

    if (!rooms_amount || *rooms_amount <= 0) return out;

    // Safety cap to avoid infinite loops when world is too crowded
    int max_total_attempts = std::max((*rooms_amount) * 1000, 5000);
    int total_attempts = 0;

    while ((int)out.rooms.size() < *rooms_amount && total_attempts < max_total_attempts) {
        Room new_room = gen_new_no_overlap_room(out);
        
        // Only add if it's a valid room (width/height > 0)
        if (new_room.width > 0 && new_room.height > 0) {
            out.add_room(new_room);
        }

        total_attempts++;
    }

    if ((int)out.rooms.size() < *rooms_amount) {
        std::cerr << "Warning: only generated " << out.rooms.size() << " of " << *rooms_amount << " rooms (attempts=" << total_attempts << ")\n";
    }

    return out;
}

Room GenWorld::make_room(int posx, int posy, int sizex, int sizey) {
    // Create a room with the specified position and size
    Room room;
    room.world_posx = posx;
    room.world_posy = posy;
    room.width = sizex;
    room.height = sizey;
    return room;
}

Rooms GenWorld::gen_room_cluster(Rooms existing_rooms) {
    // Generate a cluster of adjacent rooms
    // The cluster expands by attaching new rooms to the cluster perimeter
    Rooms cluster;

    // Determine target cluster size
    int min_cluster = (min_rooms_in_cluster) ? *min_rooms_in_cluster : 1;
    int max_cluster = (max_rooms_in_cluster) ? *max_rooms_in_cluster : 5;
    if (min_cluster > max_cluster) std::swap(min_cluster, max_cluster);

    int target_cluster_size = randomFromSeed(generator, min_cluster, max_cluster);

    int attempts = 0;
    int max_total_attempts = std::max(target_cluster_size * 1000, 5000);

    // Create the first room in the cluster
    Room first_room = gen_new_no_overlap_room(existing_rooms);
    if (first_room.width > 0 && first_room.height > 0) {
        cluster.add_room(first_room);
    } else {
        return cluster; // Failed to create first room
    }

    // Keep adding rooms adjacent to the cluster
    while ((int)cluster.rooms.size() < target_cluster_size && attempts < max_total_attempts) {
        // Pick a random room from the cluster to attach to
        int cluster_idx = randomFromSeed(generator, 0, cluster.rooms.size() - 1);
        const Room& anchor_room = cluster.rooms[cluster_idx];

        // Pick a random side: 0=left, 1=right, 2=bottom, 3=top
        int side = randomFromSeed(generator, 0, 3);

        // Generate a new room size
        int min_sz = (min_room_size) ? *min_room_size : 1;
        int max_sz = (max_room_size) ? *max_room_size : 1;
        if (min_sz > max_sz) std::swap(min_sz, max_sz);
        
        int new_width = randomFromSeed(generator, min_sz, max_sz);
        int new_height = randomFromSeed(generator, min_sz, max_sz);

        // Calculate position based on which side of the anchor room
        int new_posx, new_posy;
        int anchor_left = anchor_room.world_posx - (anchor_room.width / 2);
        int anchor_right = anchor_room.world_posx + (anchor_room.width / 2);
        int anchor_bottom = anchor_room.world_posy - (anchor_room.height / 2);
        int anchor_top = anchor_room.world_posy + (anchor_room.height / 2);

        switch (side) {
            case 0: // Attach to left
                new_posx = anchor_left - (new_width / 2) - 1;
                new_posy = anchor_room.world_posy; // Center vertically on anchor
                break;
            case 1: // Attach to right
                new_posx = anchor_right + (new_width / 2) + 1;
                new_posy = anchor_room.world_posy;
                break;
            case 2: // Attach to bottom
                new_posx = anchor_room.world_posx; // Center horizontally on anchor
                new_posy = anchor_bottom - (new_height / 2) - 1;
                break;
            case 3: // Attach to top
                new_posx = anchor_room.world_posx;
                new_posy = anchor_top + (new_height / 2) + 1;
                break;
            default:
                new_posx = anchor_room.world_posx;
                new_posy = anchor_room.world_posy;
        }

        // Clamp to world bounds
        int total_tiles_x = CHUNKS_PER_WORLDX * TILES_PER_CHUNK;
        int total_tiles_y = CHUNKS_PER_WORLDY * TILES_PER_CHUNK;
        int world_border_val = (world_border) ? *world_border : 0;

        new_posx = std::max(world_border_val + new_width / 2,
                   std::min(new_posx, total_tiles_x - world_border_val - new_width / 2));
        new_posy = std::max(world_border_val + new_height / 2,
                   std::min(new_posy, total_tiles_y - world_border_val - new_height / 2));

        // Create the candidate room
        Room candidate = make_room(new_posx, new_posy, new_width, new_height);

        // Check overlap with existing_rooms (global) but NOT with cluster (adjacent is OK)
        bool conflict = false;
        int half_w = candidate.width / 2;
        int half_h = candidate.height / 2;
        int cand_left = candidate.world_posx - half_w;
        int cand_right = candidate.world_posx + half_w;
        int cand_bottom = candidate.world_posy - half_h;
        int cand_top = candidate.world_posy + half_h;

        for (const Room& r : existing_rooms.rooms) {
            int r_half_w = r.width / 2;
            int r_half_h = r.height / 2;
            int r_left = r.world_posx - r_half_w - ((room_border) ? *room_border : 0);
            int r_right = r.world_posx + r_half_w + ((room_border) ? *room_border : 0);
            int r_bottom = r.world_posy - r_half_h - ((room_border) ? *room_border : 0);
            int r_top = r.world_posy + r_half_h + ((room_border) ? *room_border : 0);

            bool no_overlap = (cand_right < r_left) || (cand_left > r_right) ||
                             (cand_top < r_bottom) || (cand_bottom > r_top);
            if (!no_overlap) {
                conflict = true;
                break;
            }
        }

        if (!conflict) {
            cluster.add_room(candidate);
        }

        attempts++;
    }

    return cluster;
}

Rooms GenWorld::gen_room_clusters(Rooms existing_rooms) {
    // Generate multiple clusters based on the clusters pointer value
    Rooms all_clusters;
    
    if (!clusters || *clusters <= 0) {
        return all_clusters; // No clusters to generate
    }

    int num_clusters = *clusters;
    int attempts = 0;
    int max_total_attempts = num_clusters * 1000;

    for (int i = 0; i < num_clusters && attempts < max_total_attempts; i++) {
        // Generate a single cluster
        Rooms cluster = gen_room_cluster(all_clusters);
        
        // Add all rooms from this cluster to the overall collection
        all_clusters.add_rooms(cluster);

        attempts++;
    }

    return all_clusters;
}

void GenWorld::fill_world_with_wall() {
    for (int chunkx = 0; chunkx < CHUNKS_PER_WORLDX; chunkx++) {
        for (int chunky = 0; chunky < CHUNKS_PER_WORLDY; chunky++) {
            for (int tilex = 0; tilex < TILES_PER_CHUNK; tilex++) {
                for (int tiley = 0; tiley < TILES_PER_CHUNK; tiley++) {
                    std::string chstrpath = get_chunk_string(chunkx, chunky);
                    std::string chpath = std::string(FOLDER_PATH) + "/" + chstrpath;
                    Vec2 tp = {static_cast<float>(tilex), static_cast<float>(tiley)};
                    insert_tile_on_chunk(chpath, tp, TileType::Wall);
                    }
                }
            }
        }
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

void GenWorld::random_single_tile_room_placement(float randomness) {
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
                        insert_tile_on_chunk(chpath, tp, TileType::Room);
                    }
                }
            }
        }
    }
}

void GenWorld::apply_rooms_to_disk(Rooms rooms) {
    // Iterate rooms and write each room's tiles into the appropriate chunk JSON files
    for (const Room& room : rooms.rooms) {
        // compute rectangle for the room: left..right, bottom..top (inclusive)
        int left = room.world_posx - (room.width / 2);
        int bottom = room.world_posy - (room.height / 2);
        int right = left + room.width - 1;
        int top = bottom + room.height - 1;

        // Clamp to world bounds
        int total_tiles_x = CHUNKS_PER_WORLDX * TILES_PER_CHUNK;
        int total_tiles_y = CHUNKS_PER_WORLDY * TILES_PER_CHUNK;

        for (int tx = left; tx <= right; ++tx) {
            if (tx < 0 || tx >= total_tiles_x) continue;
            int chunk_x = tx / TILES_PER_CHUNK;
            int inside_x = tx % TILES_PER_CHUNK;

            for (int ty = bottom; ty <= top; ++ty) {
                if (ty < 0 || ty >= total_tiles_y) continue;
                int chunk_y = ty / TILES_PER_CHUNK;
                int inside_y = ty % TILES_PER_CHUNK;

                std::string chstrpath = get_chunk_string(chunk_x, chunk_y);
                std::string chpath = std::string(FOLDER_PATH) + "/" + chstrpath;
                Vec2 tp = {static_cast<float>(inside_x), static_cast<float>(inside_y)};

                // Determine if this tile is on the edge of the room
                bool on_edge = (tx == left || tx == right || ty == bottom || ty == top);
                TileType tile_type = on_edge ? TileType::RoomEdge : TileType::Room;

                insert_tile_on_chunk(chpath, tp, tile_type);
            }
        }
    }
}

void GenWorld::gen_world() {
    // Create empty world and populate walls, then place non-overlapping rooms
    Rooms all_rooms;

    this->generate_empty_world();
    this->fill_world_with_wall();
    Rooms single_rooms = this->gen_rooms_no_overlap();
    Rooms clusters = this->gen_room_clusters(single_rooms);

    all_rooms.add_rooms(single_rooms);
    all_rooms.add_rooms(clusters);
    this->apply_rooms_to_disk(all_rooms);

}

void GenWorld::init() {
    // Validate all required pointers are set
    if (!seed) {
        std::cerr << "ERROR GenWorld::init(): seed pointer not set!\n";
        std::exit(1);
    }
    if (!max_room_size) {
        std::cerr << "ERROR GenWorld::init(): max_room_size pointer not set!\n";
        std::exit(1);
    }
    if (!min_room_size) {
        std::cerr << "ERROR GenWorld::init(): min_room_size pointer not set!\n";
        std::exit(1);
    }
    if (!rooms_amount) {
        std::cerr << "ERROR GenWorld::init(): rooms_amount pointer not set!\n";
        std::exit(1);
    }
    if (!room_border) {
        std::cerr << "ERROR GenWorld::init(): room_border pointer not set!\n";
        std::exit(1);
    }
    if (!max_room_attempts) {
        std::cerr << "ERROR GenWorld::init(): max_room_attempts pointer not set!\n";
        std::exit(1);
    }
    if (!world_border) {
        std::cerr << "ERROR GenWorld::init(): world_border pointer not set!\n";
        std::exit(1);
    }
    if (!max_rooms_in_cluster) {
        std::cerr << "ERROR GenWorld::init(): max_rooms_in_cluster pointer not set!\n";
        std::exit(1);
    }
    if (!min_rooms_in_cluster) {
        std::cerr << "ERROR GenWorld::init(): min_rooms_in_cluster pointer not set!\n";
        std::exit(1);
    }
    if (!clusters) {
        std::cerr << "ERROR GenWorld::init(): clusters pointer not set!\n";
        std::exit(1);
    }

    // Initialize the RNG with the seed
    generator = std::mt19937(*seed);  // Mersenne Twister seeded

}

