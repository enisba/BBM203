#include <iostream>
#include "Questions.h"
#include "GameWorld.h"
#include "Player.h"
#include "Isle.h"

#define NUMBER_OF_INPUTS 4
#define NUMBER_OF_OUTPUTS 4

int main(int argc, char **argv)
{

    Questions quiz = Questions();
    quiz.singlePlayerDemo();

    return 0;

}