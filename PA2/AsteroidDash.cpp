#include "AsteroidDash.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <algorithm>

// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

    : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard())
{
    read_space_grid(space_grid_file_name);
    read_player(player_file_name, player_name);          // Initialize player using the player.dat file                                                      // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name); // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);
}

void AsteroidDash::read_space_grid(const string &input_file)
{
    ifstream file(input_file);

    if (!file)
    {
        return;
    }

    string line;
    space_grid.clear();

    while (getline(file, line))
    {
        istringstream iss(line);
        vector<int> row;
        int value;

        while (iss >> value)
        {
            if (value == 0 || value == 1)
            {
                row.push_back(value);
            }
        }

        if (!row.empty())
        {
            space_grid.push_back(row);
        }
    }

    file.close();

    if (space_grid.empty())
    {
        cerr << "Error: Space grid file is empty or unreadable: " << input_file << endl;
    }

    object_grid = space_grid;
}

void AsteroidDash::read_player(const string &player_file_name, const string &player_name)
{
    ifstream file(player_file_name);
    if (!file)
    {
        cerr << "Error opening player file: " << player_file_name << endl;
        return;
    }

    int row, col;
    if (!(file >> row >> col))
    {
        cerr << "Error: Player position data is missing." << endl;
        return;
    }

    file.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<vector<bool>> shape;
    string line;
    while (getline(file, line))
    {
        vector<bool> shape_row;
        for (char ch : line)
        {
            if (ch == '1')
            {
                shape_row.push_back(true);
            }
            else if (ch == '0')
            {
                shape_row.push_back(false);
            }
        }
        if (!shape_row.empty())
            shape.push_back(shape_row);
    }

    if (shape.empty())
    {
        cerr << "Error: Player shape data is missing." << endl;
        return;
    }

    if (row < 0 || row + shape.size() > space_grid.size() ||
        col < 0 || col + shape[0].size() > space_grid[0].size())
    {
        cerr << "Error: Player position (" << row << ", " << col << ") or shape exceeds grid bounds." << endl;
        return;
    }

    player = new Player(shape, row, col, player_name);

    file.close();
}

void AsteroidDash::read_celestial_objects(const string &input_file)
{
    std::ifstream file(input_file);
    if (!file.is_open())
    {
        return;
    }

    if (file.peek() == std::ifstream::traits_type::eof())
    {
        return;
    }

    CelestialObject *prev_object = nullptr;
    std::string line;
    std::vector<std::vector<bool>> shape;
    int start_row = 0, time_appearance = 0;
    ObjectType type = ASTEROID;

    while (std::getline(file, line))
    {
        if (line[0] == '[' || line[0] == '{')
        {
            shape.clear();
            type = (line[0] == '[') ? ASTEROID : AMMO;

            std::vector<bool> shape_row;
            for (char ch : line.substr(1, line.size() - 1))
            {
                if (ch == '1')
                    shape_row.push_back(true);
                else if (ch == '0')
                    shape_row.push_back(false);
            }
            if (!shape_row.empty())
            {
                shape.push_back(shape_row);
                shape_row.clear();
            }

            while (std::getline(file, line) && line[0] != 's' && line[0] != 't' && line[0] != 'e')
            {
                for (char ch : line)
                {
                    if (ch == '1')
                        shape_row.push_back(true);
                    else if (ch == '0')
                        shape_row.push_back(false);
                }
                if (!shape_row.empty())
                {
                    shape.push_back(shape_row);
                    shape_row.clear();
                }
            }
        }

        if (line[0] == 's')
        {
            start_row = std::stoi(line.substr(2));
        }
        else if (line[0] == 't')
        {
            time_appearance = std::stoi(line.substr(2));
        }
        else if (line[0] == 'e')
        {
            type = line.substr(2, line.size() - 2) == "ammo" ? AMMO : LIFE_UP;
        }

        if (!shape.empty() && (line.empty() /* || line[0] == '[' || line[0] == '{'*/))
        {
            CelestialObject *new_object = new CelestialObject(shape, type, start_row, time_appearance);

            if (!celestial_objects_list_head)
            {
                celestial_objects_list_head = new_object;
            }
            else
            {
                prev_object->next_celestial_object = new_object;
            }

            prev_object = new_object;
            shape.clear();
            start_row = time_appearance = 0;
            type = ASTEROID;
        }
    }
    file.close();

    if (!shape.empty())
    {

        CelestialObject *new_object = new CelestialObject(shape, type, start_row, time_appearance);

        if (!celestial_objects_list_head)
        {
            celestial_objects_list_head = new_object;
        }
        else
        {
            prev_object->next_celestial_object = new_object;
        }
    }

    CelestialObject *current = celestial_objects_list_head;
    while (current != nullptr)
    {
        current->calculateRotation();
        current = current->next_celestial_object;
    }
}

void AsteroidDash::print_space_grid() const
{
    for (const auto &row : space_grid)
    {
        for (int cell : row)
        {
            if (cell == 1)
            {
                std::cout << occupiedCellChar;
            }
            else
            {
                std::cout << unoccupiedCellChar;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void AsteroidDash::update_space_grid()
{
    // Tüm grid'i temizle
    for (auto &row : object_grid)
    {
        std::fill(row.begin(), row.end(), EMPTY);
    }

    // Oyuncunun pozisyonunu grid'e ekle
    int player_col = player->position_col;
    int player_row = player->position_row;

    for (size_t i = 0; i < player->spacecraft_shape.size(); i++)
    {
        for (size_t j = 0; j < player->spacecraft_shape[i].size(); j++)
        {
            if (player->spacecraft_shape[i][j])
            {
                int grid_row = player_row + i;
                int grid_col = player_col + j;
                if (grid_row >= 0 && grid_row < object_grid.size() &&
                    grid_col >= 0 && grid_col < object_grid[0].size())
                {
                    object_grid[grid_row][grid_col] = PLAYER;
                }
            }
        }
    }

    // Gök cisimlerini güncelle
    CelestialObject *curr = celestial_objects_list_head;
    CelestialObject *prev = nullptr;


    while (curr != nullptr)
    {

        bool remove_object = false;
        int obj_col = static_cast<int>(object_grid[0].size()) - 1 - (game_time - curr->time_of_appearance);

        for (int i = 0; i < curr->shape.size(); i++)
        {
            for (int j = 0; j < curr->shape[i].size(); j++)
            {
                int grid_row = curr->starting_row + i;
                int grid_col = obj_col + j;

                if (grid_row >= 0 && grid_row < static_cast<int>(object_grid.size()) &&
                    grid_col >= 0 && grid_col < static_cast<int>(object_grid[0].size()))
                {
                    if (curr->shape[i][j])
                    {
                        if (object_grid[grid_row][grid_col] == PLAYER)
                        {
                            switch (curr->object_type)
                            {
                            case ASTEROID:
                                player->lives--;
                                if (player->lives == 0)
                                {
                                    game_over = true;
                                }
                                break;
                            case LIFE_UP:
                                player->lives += 1;
                                
                                break;
                            case AMMO:
                                player->current_ammo = player->max_ammo;
                                break;
                            }
                            remove_object = true;
                            break;
                        }
                        else
                        {
                            object_grid[grid_row][grid_col] = CELESTIAL_OBJECT;
                        }
                    }
                }
            }
            if (remove_object)
                break;
        }

        if (remove_object)
        {
            clear_celestial_object_from_grid(curr);

            CelestialObject *temp = curr;
            curr = curr->next_celestial_object;
            if (prev == nullptr)
            {
                celestial_objects_list_head = curr;
            }
            else
            {
                prev->next_celestial_object = curr;
            }

            delete temp;
            temp = nullptr;
        }
        else
        {
            prev = curr;
            curr = curr->next_celestial_object;
        }
    }

    // Mermileri güncelle
    // Bullet *current_bullet = bullets_head;
    // Bullet *prev_bullet = nullptr;

    // while (current_bullet != nullptr)
    // {
    //     // collision check et
    //     // varsa continue at
    //     if (!check_collision(current_bullet, celestial_objects_list_head, object_grid, game_time, current_score))
    //     {
    //         Bullet *to_delete = current_bullet;
    //         if (prev_bullet == nullptr)
    //         {
    //             bullets_head = current_bullet->next;
    //             current_bullet = bullets_head;
    //         }
    //         else
    //         {
    //             prev_bullet->next = current_bullet->next;
    //             current_bullet = current_bullet->next;
    //         }
    //         delete to_delete;
    //         to_delete = nullptr;
    //         continue;
    //     }

    //     // Mermiyi sağa doğru hareket ettir
    //     current_bullet->col++;

    //     // Mermi sınırların dışına çıktıysa, mermiyi listeden kaldır
    //     if ( current_bullet->col < 0||current_bullet->col >= object_grid[0].size())
    //     {
    //         Bullet *to_delete = current_bullet;
    //         if (prev_bullet == nullptr)
    //         {
    //             bullets_head = current_bullet->next;
    //             current_bullet = bullets_head;
    //         }
    //         else
    //         {
    //             prev_bullet->next = current_bullet->next;
    //             current_bullet = current_bullet->next;
    //         }
    //         continue;
    //     }

        
    //     // collision check et
    //     bool bullet_removed = false;

    //     // Mermi bir gök cismiyle çarpışıyor mu kontrol et
    //     if (current_bullet->row >= 0 && current_bullet->row < object_grid.size() &&
    //         object_grid[current_bullet->row][current_bullet->col] == CELESTIAL_OBJECT)
    //     {
    //         // Çarpılan gök cismini bul
    //         CelestialObject *hit_object = celestial_objects_list_head;
    //         CelestialObject *prev_object = nullptr;

    //         while (hit_object != nullptr)
    //         {
    //             int obj_col = static_cast<int>(object_grid[0].size()) - 1 - (game_time - hit_object->time_of_appearance);
    //             int start_row = hit_object->starting_row;
    //             int end_row = start_row + hit_object->shape.size();
    //             int start_col = obj_col;
    //             int end_col = start_col + hit_object->shape[0].size();

    //             if (current_bullet->row >= start_row && current_bullet->row < end_row &&
    //                 current_bullet->col >= start_col && current_bullet->col < end_col)
    //             {
    //                 int shape_row = current_bullet->row - start_row;
    //                 int shape_col = current_bullet->col - start_col;

    //                 if (hit_object->shape[shape_row][shape_col])
    //                 {
    //                     // Çarpılan hücreyi kaldır

    //                     // Çarpılan hücre başına puan eklenir
    //                     current_score += 10;

    //                     // Gök cisminin tamamen yok olup olmadığını kontrol et
    //                     bool object_destroyed = true;
    //                     for (const auto &row : hit_object->shape)
    //                     {
    //                         for (bool cell : row)
    //                         {
    //                             if (cell)
    //                             {
    //                                 object_destroyed = false;
    //                                 break;
    //                             }
    //                         }
    //                         if (!object_destroyed)
    //                             break;
    //                     }

    //                     if (object_destroyed)
    //                     {
    //                         // Gök cismi tamamen yok olduysa bonus puan ekle ve cismi kaldır
    //                         current_score += 100;
    //                         for (int i = 0; i < hit_object->shape.size(); ++i)
    //                         {
    //                             for (int j = 0; j < hit_object->shape[i].size(); ++j)
    //                             {
    //                                 int grid_row = hit_object->starting_row + i;
    //                                 int grid_col = obj_col + j;

    //                                 if (grid_row >= 0 && grid_row < object_grid.size() &&
    //                                     grid_col >= 0 && grid_col < object_grid[0].size() &&
    //                                     hit_object->shape[i][j])
    //                                 {
    //                                     object_grid[grid_row][grid_col] = EMPTY;
    //                                 }
    //                             }
    //                         }

    //                         CelestialObject *to_delete_object = hit_object;
    //                         if (prev_object == nullptr)
    //                         {
    //                             celestial_objects_list_head = hit_object->next_celestial_object;
    //                         }
    //                         else
    //                         {
    //                             prev_object->next_celestial_object = hit_object->next_celestial_object;
    //                         }
    //                         hit_object = hit_object->next_celestial_object;
    //                         delete to_delete_object;
    //                         to_delete_object = nullptr;
    //                     }
    //                     else
    //             {
    //                 // Rotasyon işlemi
    //                 int middle_row = hit_object->shape.size() / 2;

    //                 if (shape_row < middle_row)
    //                 {
    //                     hit_object->shape = hit_object->right_rotation->shape;

    //                     int new_shape_row = shape_col;
    //                     int new_shape_col = hit_object->shape.size() - shape_row - 1;

    //                     hit_object->shape[new_shape_row][new_shape_col] = false;
    //                 }
    //                 else
    //                 {
    //                     hit_object->shape = hit_object->left_rotation->shape;

    //                     int new_shape_row = hit_object->shape.size() - shape_col - 1;
    //                     int new_shape_col = shape_row;

    //                     hit_object->shape[new_shape_row][new_shape_col] = false;
    //                 }

    //                 // Hemen dönüş sonrası grid güncelle
    //                 for (int i = 0; i < hit_object->shape.size(); ++i)
    //                 {
    //                     for (int j = 0; j < hit_object->shape[i].size(); ++j)
    //                     {
    //                         int grid_row = hit_object->starting_row + i;
    //                         int grid_col = obj_col + j;

    //                         if (grid_row >= 0 && grid_row < object_grid.size() &&
    //                             grid_col >= 0 && grid_col < object_grid[0].size())
    //                         {
    //                             object_grid[grid_row][grid_col] = hit_object->shape[i][j] ? CELESTIAL_OBJECT : EMPTY;
    //                         }
    //                     }
    //                 }

    //                 CelestialObject::delete_rotations(hit_object);
    //                 hit_object->calculateRotation();
    //             }
                
    //                     bullet_removed = true;
    //                     break;
    //                 }
    //             }
    //             else
    //             {
    //                 prev_object = hit_object;
    //                 hit_object = hit_object->next_celestial_object;
    //             }
    //         }

    //         // Mermiyi çarpışmadan sonra kaldır
    //         Bullet *to_delete = current_bullet;
    //         if (prev_bullet == nullptr)
    //         {
    //             bullets_head = current_bullet->next;
    //             current_bullet = bullets_head;
    //         }
    //         else
    //         {
    //             prev_bullet->next = current_bullet->next;
    //             current_bullet = current_bullet->next;
    //         }
    //         delete to_delete;
    //         to_delete = nullptr;
    //         bullet_removed = true;
    //     }
    //     else
    //     {
    //         // Çarpışma yoksa, merminin yeni pozisyonunu gride ekle
    //         object_grid[current_bullet->row][current_bullet->col] = BULLET;
    //     }

    //     // Eğer mermi silinmediyse sıradaki mermiye geç
    //     if (!bullet_removed)
    //     {
    //         prev_bullet = current_bullet;
    //         current_bullet = current_bullet->next;
    //     }
    // }

    // Tüm grid'i güncelle
    for (size_t i = 0; i < space_grid.size(); ++i)
    {
        for (size_t j = 0; j < space_grid[i].size(); ++j)
        {
            if (object_grid[i][j] != EMPTY)
                space_grid[i][j] = 1;
            else
                space_grid[i][j] = 0;
        }
    }
}

bool AsteroidDash::check_collision(Bullet *current_bullet, CelestialObject *celestial_objects_list_head,
                                   std::vector<std::vector<int>> &object_grid, unsigned long game_time, unsigned long &current_score)
{
    if (current_bullet->row >= 0 && current_bullet->row < object_grid.size() &&
        current_bullet->col >= 0 && current_bullet->col < object_grid[0].size() &&
        object_grid[current_bullet->row][current_bullet->col] == CELESTIAL_OBJECT)
    {
        if (celestial_objects_list_head == nullptr)
        {
            return true;
        }
        // Çarpılan gök cismini bul
        CelestialObject *hit_object = celestial_objects_list_head;
        CelestialObject *prev_object = nullptr;

        while (hit_object != nullptr)
        {
            int obj_col = static_cast<int>(object_grid[0].size()) - 1 - (game_time - hit_object->time_of_appearance);
            int start_row = hit_object->starting_row;
            int end_row = start_row + hit_object->shape.size();
            int start_col = obj_col;
            int end_col = start_col + hit_object->shape[0].size();

            // Merminin gök cisminin içinde olup olmadığını kontrol et
            if (current_bullet->row >= start_row && current_bullet->row < end_row &&
                current_bullet->col >= start_col && current_bullet->col < end_col &&
                hit_object->shape[current_bullet->row - start_row][current_bullet->col - start_col])
            {
                int shape_row = current_bullet->row - start_row;
                int shape_col = current_bullet->col - start_col;

                // Çarpılan hücre başına puan eklenir
                current_score += 10;

                // Eğer gök cismi tamamen yok olduysa bonus puan ekle ve cismi kaldır
                bool object_destroyed = true;
                for (const auto &row : hit_object->shape)
                {
                    for (bool cell : row)
                    {
                        if (cell)
                        {
                            object_destroyed = false;
                            break;
                        }
                    }
                    if (!object_destroyed)
                        break;
                }

                if (object_destroyed)
                {
                    // Tüm grid hücrelerini boşalt
                    for (int i = 0; i < hit_object->shape.size(); ++i)
                    {
                        for (int j = 0; j < hit_object->shape[i].size(); ++j)
                        {
                            int grid_row = hit_object->starting_row + i;
                            int grid_col = obj_col + j;

                            if (grid_row >= 0 && grid_row < object_grid.size() &&
                                grid_col >= 0 && grid_col < object_grid[0].size() &&
                                hit_object->shape[i][j])
                            {
                                object_grid[grid_row][grid_col] = EMPTY;
                            }
                        }
                    }

                    // Cismi listeden kaldır
                    CelestialObject *to_delete_object = hit_object;
                    if (prev_object == nullptr)
                    {
                        celestial_objects_list_head = hit_object->next_celestial_object;
                    }
                    else
                    {
                        prev_object->next_celestial_object = hit_object->next_celestial_object;
                    }
                    delete to_delete_object;
                    to_delete_object = nullptr;
                }
                else
                {
                    // Rotasyon işlemi
                    int middle_row = hit_object->shape.size() / 2;

                    if (shape_row < middle_row)
                    {
                        hit_object->shape = hit_object->right_rotation->shape;

                        int new_shape_row = shape_col;
                        int new_shape_col = hit_object->shape.size() - shape_row - 1;

                        hit_object->shape[new_shape_row][new_shape_col] = false;
                    }
                    else
                    {
                        hit_object->shape = hit_object->left_rotation->shape;

                        int new_shape_row = hit_object->shape.size() - shape_col - 1;
                        int new_shape_col = shape_row;

                        hit_object->shape[new_shape_row][new_shape_col] = false;
                    }

                    // Hemen dönüş sonrası grid güncelle
                    for (int i = 0; i < hit_object->shape.size(); ++i)
                    {
                        for (int j = 0; j < hit_object->shape[i].size(); ++j)
                        {
                            int grid_row = hit_object->starting_row + i;
                            int grid_col = obj_col + j;

                            if (grid_row >= 0 && grid_row < object_grid.size() &&
                                grid_col >= 0 && grid_col < object_grid[0].size())
                            {
                                object_grid[grid_row][grid_col] = hit_object->shape[i][j] ? CELESTIAL_OBJECT : EMPTY;
                            }
                        }
                    }

                    CelestialObject::delete_rotations(hit_object);
                    hit_object->calculateRotation();
                }

                // Çarpışma gerçekleştiği için `false` döndür
                return false;
            }

            // Bir sonraki gök cismini kontrol et
            prev_object = hit_object;
            hit_object = hit_object->next_celestial_object;
        }
    }

    // Çarpışma gerçekleşmediyse `true` döndür
    return true;
}

void AsteroidDash::clear_celestial_object_from_grid(CelestialObject *obj)
{
    int current_time = game_time;
    int appearance_time = obj->time_of_appearance;
    int obj_width = obj->shape[0].size();
    int obj_height = obj->shape.size();

    for (int t = appearance_time; t <= current_time; ++t)
    {
        int obj_col = static_cast<int>(object_grid[0].size()) - 1 - (t - appearance_time);

        for (int i = 0; i < obj_height; ++i)
        {
            for (int j = 0; j < obj_width; ++j)
            {
                int grid_row = obj->starting_row + i;
                int grid_col = obj_col + j;

                if (grid_row >= 0 && grid_row < static_cast<int>(object_grid.size()) &&
                    grid_col >= 0 && grid_col < static_cast<int>(object_grid[0].size()))
                {
                    if (obj->shape[i][j])
                    {
                        if (object_grid[grid_row][grid_col] == CELESTIAL_OBJECT)
                        {
                            object_grid[grid_row][grid_col] = EMPTY;
                        }
                    }
                }
            }
        }
    }
}

// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot()
{   
    
    if (player && player->current_ammo > 0)
    {
        player->current_ammo--;

        int projectile_row = player->position_row + player->spacecraft_shape.size() / 2;
        int projectile_col = player->position_col + player->spacecraft_shape[0].size() - 1;

        Bullet *new_bullet = new Bullet();
        new_bullet->row = projectile_row;
        new_bullet->col = projectile_col;
        new_bullet->next = bullets_head;
        bullets_head = new_bullet;
    }
}



AsteroidDash::~AsteroidDash()

{
    delete player;
    player = nullptr;

    CelestialObject *current = celestial_objects_list_head;
    while (current)
    {
        CelestialObject::delete_rotations(current);

        CelestialObject *temp = current->next_celestial_object;
        delete current;
        current = temp;
    }
    celestial_objects_list_head = nullptr;

    // Bullet *current_bullet = bullets_head;
    // while (current_bullet != nullptr)
    // {
    //     Bullet *next_bullet = current_bullet->next;
    //     delete current_bullet;
    //     current_bullet = next_bullet;
    // }
    // bullets_head = nullptr;

    
}
