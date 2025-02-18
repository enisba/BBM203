#ifndef ASTEROIDDASH_H
#define ASTEROIDDASH_H

#include <vector>
#include <string>
#include <iostream>

#include "CelestialObject.h"
#include "LeaderboardEntry.h"
#include "Leaderboard.h"
#include "Player.h"

#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"

using namespace std;

enum CELL_TYPE {
    EMPTY = 0,
    PLAYER = 1,
    CELESTIAL_OBJECT = 2,
    BULLET = 3
};

struct Bullet {
    int row;
    int col;
    Bullet* next; // Bağlı liste için
};
// Class that encapsulates the game play internals
class AsteroidDash {
public:

    // Constructor to initialize the game
    AsteroidDash(const string &space_grid_file_name, const string &celestial_objects_file_name,
                 const string &leaderboard_file_name, const string &player_file_name, const string &player_name);

    // Destructor. Remove dynamically allocated member variables here
    virtual ~AsteroidDash();
    
    Bullet* bullets_head; 

    // 2D space_grid
    vector<vector<int> > space_grid;

    vector<std::vector<int>> object_grid;


    // Pointer to track the player instance
    Player *player = nullptr;

    // A reference to the head of the celestial objects linked list
    CelestialObject *celestial_objects_list_head = nullptr;

    // Current score of the game
    unsigned long current_score = 0;

    // Current game tick
    unsigned long game_time = 0;

    // Leaderboard file name to store and read the leaderboard from
    string leaderboard_file_name;

    // Leaderboard member variable
    Leaderboard leaderboard;

    // True if the game is over
    bool game_over = false;

    // Function to print the space_grid
    void print_space_grid() const;

    // Function to read the space grid from a file
    void read_space_grid(const string &input_file);

    // Function to read the player from a file
    void read_player(const string &player_file_name, const string &player_name);

    // Function to read celestial objects from a file
    // Reads the input file and calls the read_celestial_object() function for each celestial_object;
    void read_celestial_objects(const string &input_file);

    // Updates the grid based on player and celestial object states
    void update_space_grid();

    void clear_celestial_object_from_grid(CelestialObject *object);

    // Corresponds to the SHOOT command.
    void shoot();

    // Function to find the celestial object at the given row and column
    CelestialObject *find_celestial_object(int row, int col);

    // Function to update the asteroid shape after a hit
    bool update_asteroid_shape(CelestialObject *asteroid, int hit_row);

    // Function to rotate the asteroid shape in the given direction
    void rotate_asteroid_shape(CelestialObject *asteroid, const string &direction);

    // Function to update the rotations of the given asteroid
    void update_asteroid_rotations(CelestialObject *asteroid);

    // Function to remove the hit cell from the asteroid shape
    void remove_hit_cell(CelestialObject *asteroid, int hit_row);

    void remove_celestial_object(CelestialObject *object);

    bool check_collision(Bullet *current_bullet, CelestialObject *celestial_objects_list_head, 
                         std::vector<std::vector<int>> &object_grid, unsigned long game_time, unsigned long &current_score);
};



#endif // ASTEROIDDASH_H
