#pragma once

#include <filesystem>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "utils.hpp"
#include "world.hpp" // for checking if alr existing for world before genning

using json = nlohmann::json;
namespace fs = std::filesystem;

struct GenWorld {
    const int TILES_PER_CHUNK = 8;
    const int CHUNKS_PER_WORLDX = 10;
    const int CHUNKS_PER_WORLDY = 10;
    const int SPAWN_PLAYERX = 40;
    const int SPAWN_PLAYERY = 40;

    const int *max_room_size;
    const int *min_room_size;
    const int *rooms_amount;
    const int *room_border; // closeness of rooms being able to spawn
    const int *max_rooms_in_cluster;
    const int *min_rooms_in_cluster;
    const int *clusters;
    const int *max_room_attempts;

    const int *world_border; // space between room spawn chance and world border

    int* seed; // set this at init

    std::mt19937 generator;

    fs::path FOLDER_PATH = "../world";

    void make_world_folder();
    void create_chunk_file(int chunk_x, int chunk_y);
    void create_meta_file();
    void insert_tile_on_chunk(fs::path chunk_path, Vec2 tp, TileType tt);

    Room gen_new_no_overlap_room(Rooms existing_rooms); 
    Room make_room(int posx, int posy, int sizex, int sizey);

    Rooms gen_room_cluster(Rooms existing_rooms);
    Rooms gen_room_clusters(Rooms existing_rooms);
    Rooms gen_rooms_no_overlap();
    void apply_rooms_to_disk(Rooms rooms);

    void fill_world_with_wall();
    
    void random_wall_placement(float randomness);
    void random_single_tile_room_placement(float randomness);


    void generate_empty_world();
    void gen_world();
    void init(); // call this in the start
};