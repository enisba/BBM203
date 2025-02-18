#ifndef QUIZ_H
#define QUIZ_H

#include <iostream>
#include <fstream>
#include "GameWorld.h"
#include "Player.h"
#include "RealmShapers.h"
#include "Map.h"
#include "Isle.h"

struct Question
{
    std::string text;
};

class Questions
{
protected:
    GameWorld gameWorld = GameWorld();
    std::vector<std::string> answers;
    std::vector<Question> questions;
    std::vector<int> usedIndices;
    std::string getStringInput(const std::string &prompt);
    std::string buildPlayer();
    static void showMenu();
    void duel(RealmShaper *player);
    void craft(RealmShaper *player);
    void viewProgress(RealmShaper* player);
    int getRandomIndex(const std::vector<Question>& questions);
    std::vector<Question> loadQuestions(const std::string &filename);
    std::vector<std::string> loadAnswers(const std::string &filename);
public:
    bool runQuiz(const std::vector<Question> &questions, const std::vector<std::string> &answers);
    void singlePlayerDemo();
};

#endif