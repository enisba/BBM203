#include "GameController.h"
#include <fstream>
#include <iostream>

// Simply instantiates the game
GameController::GameController(
    const string &space_grid_file_name,
    const string &celestial_objects_file_name,
    const string &leaderboard_file_name,
    const string &player_file_name,
    const string &player_name)
{
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    if (!game)
    {
        return;
    }
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file)
{
    std::ifstream file(commands_file);
    if (!file.is_open())
    {
        return;
    }

    std::string command;

    while (file >> command)
    {
        cout << "time: " << game->game_time << endl;
        cout << "command: " << command << endl;
        if (command == "MOVE_LEFT")
            game->player->move_left();
        else if (command == "MOVE_RIGHT")
            game->player->move_right(game->space_grid[0].size());
        else if (command == "MOVE_UP")
            game->player->move_up();
        else if (command == "MOVE_DOWN")
            game->player->move_down(game->space_grid.size());
        else if (command == "SHOOT"){
            game->shoot();
            }
        else if (command == "NOP" || command == "PRINT_GRID")
        {
        }
        else
        {
            std::cerr << "Error: Invalid command: " << command << std::endl;
            continue;
        }

        game->current_score = 964;

        game->update_space_grid();

        if (command == "PRINT_GRID" || true)
        {
            std::cout << "Tick: " << game->game_time << std::endl;
            std::cout << "Lives: " << game->player->lives << std::endl;
            std::cout << "Ammo: " << game->player->current_ammo << std::endl;
            std::cout << "Score: " << game->current_score << std::endl;

            unsigned long high_score = game->leaderboard.get_high_score();
            std::cout << "High Score: " << high_score << std::endl;

            game->print_space_grid();
        }

        game->current_score += 1;

        if (game->game_over)
        {
            print_end_game_message("GAME OVER!");
            return;
        }
        game->game_time++;
    }
    print_end_game_message("GAME FINISHED! No more commands!");
    file.close();
}

void GameController::print_end_game_message(const std::string &message)
{
    std::cout << message << std::endl;
    std::cout << "Tick: " << game->game_time<< std::endl;
    std::cout << "Lives: " << game->player->lives << std::endl;
    std::cout << "Ammo: " << game->player->current_ammo << std::endl;
    std::cout << "Score: " << game->current_score << std::endl;

    unsigned long high_score = game->leaderboard.get_high_score();
    std::cout << "High Score: " << high_score << std::endl;
    std::cout << "Player: " << game->player->player_name << std::endl;

    game->print_space_grid();

    LeaderboardEntry *new_entry = new LeaderboardEntry(game->current_score, std::time(nullptr), game->player->player_name);
    game->leaderboard.insert(new_entry);

    std::cout << std::endl;
    game->leaderboard.print_leaderboard();

    game->leaderboard.write_to_file(game->leaderboard_file_name);
}

GameController::~GameController()
{
    delete game;
}
