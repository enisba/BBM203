#include <fstream>
#include <sstream>
#include <ctime>
#include "Leaderboard.h"
#include <iostream>


// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        return;
    }

    std::string line;

    while(std::getline(file, line)){
        std::istringstream iss(line);
        int score;
        time_t timer;
        std:: string player_name;

        if (!(iss >> score >> timer >> player_name)) {
            std::cerr << "Error: Invalid leaderboard file format" << std::endl;
            continue;
        }
        
        LeaderboardEntry *new_entry = new LeaderboardEntry(score, timer, player_name);
        insert(new_entry);
    }
    file.close();
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Leaderboard file is not open: " << filename << std::endl;
        return;
    }
    LeaderboardEntry *curr = head_leaderboard_entry;
    while(curr){
        file << curr->score << " " << curr->last_played << " " << curr->player_name << std::endl;
        curr = curr->next;
    }
    file.close();
}


// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    std::cout << "Leaderboard" << std::endl;
    std::cout << "-----------" << std::endl;

    int rank = 1;
    LeaderboardEntry *curr = head_leaderboard_entry;

    while(curr!= nullptr && rank <= 10){
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", std::localtime(&curr->last_played));
        std::cout << rank << ". " << curr->player_name << " " << curr->score << " " << buffer << std::endl;
        curr = curr->next;
        rank++;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *curr = head_leaderboard_entry;
        while (curr->next && curr->next->score >= new_entry->score) {
            curr = curr->next;
        }
        new_entry->next = curr->next;
        curr->next = new_entry;
    }

    int count = 1;
    LeaderboardEntry *curr = head_leaderboard_entry;
    while (curr->next) {
        if (count == 9 && curr->next->next) { 
            // delete curr->next;
            curr->next = nullptr;
            break;
        }
        curr = curr->next;
        count++;
    }
}
unsigned long Leaderboard::get_high_score() const {
    if (head_leaderboard_entry) {
        return head_leaderboard_entry->score;
    }
    return 0; 
}

// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    LeaderboardEntry *current = head_leaderboard_entry;
    while (current != nullptr) {
        LeaderboardEntry *next = current->next;
        delete current;
        current = next;
    }
    delete current;
}