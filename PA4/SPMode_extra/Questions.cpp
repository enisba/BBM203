#include "Questions.h"
#include <random>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>
#include <chrono>

using namespace std;

string Questions::getStringInput(const string &prompt)
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear previous input
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

int Questions::getRandomIndex(const vector<Question> &questions)
{
    if (questions.empty())
    {
        cerr << "No questions available" << endl;
        return -1;
    }

    if (usedIndices.size() == questions.size())
    {
        cout << "All questions used. Resetting..." << endl;
        usedIndices.clear();
    }

    static mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<> dis(0, questions.size() - 1);

    int randomIndex;
    do
    {
        randomIndex = dis(gen);
    } while (find(usedIndices.begin(), usedIndices.end(), randomIndex) != usedIndices.end());

    usedIndices.push_back(randomIndex);

    return randomIndex;
}

vector<Question> Questions::loadQuestions(const string &filename)
{
    vector<Question> questions;
    ifstream file(filename);
    string line;

    if (!file.is_open())
    {
        cerr << "Error: Unable to open questions file." << endl;
        return questions;
    }

    while (getline(file, line))
    {

        Question q;
        q.text = line;

        questions.push_back(q);
    }
    file.close();
    return questions;
}

string setNameFromQuestion(const string &i)
{
    static const char a[] = "zYXWVUTSRQPONMLKJIHGFEDCBAabcdefghijklmnopqrstuvwxyz0123456789+/";
    vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
    {
        T[a[i]] = i;
    }

    string o;
    int val = 0, valb = -8;

    for (unsigned char c : i)
    {
        if (T[c] == -1)
            break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0)
        {
            o.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return o;
}

int setName(const string &name)
{
    int names = 0;
    for (char c : name)
    {
        names += c;
    }
    return names % 256;
}

vector<string> Questions::loadAnswers(const string &filename)
{

    int names = setName("h.u.sland");
    vector<string> answers;
    ifstream file(filename);
    string line;

    if (!file.is_open())
    {
        cerr << "Error: Unable to open answers file." << endl;
        return answers;
    }

    while (getline(file, line))
    {
        try
        {
            string name = setNameFromQuestion(line);
            for (char &c : name)
            {
                c -= names;
            }

            answers.push_back(name);
        }
        catch (invalid_argument &)
        {
            cerr << "Error: Invalid answer format." << endl;
        }
    }
    file.close();
    return answers;
}

string normalize(const string &input)
{
    string normalized;
    for (char c : input)
    {
        if (!isspace(c))
        {
            normalized += tolower(c);
        }
    }
    return normalized;
}

int levenshteinDistance(const string &s1, const string &s2)
{
    int m = s1.size();
    int n = s2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; ++i)
        dp[i][0] = i;
    for (int j = 0; j <= n; ++j)
        dp[0][j] = j;

    for (int i = 1; i <= m; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else
            {
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }
    return dp[m][n];
}

bool isSimilar(const string &input, const string &correctAnswer, int threshold = 2)
{
    int distance = levenshteinDistance(input, correctAnswer);
    return distance <= threshold;
}

bool checkAnswer(const string &userInput, const string &correctAnswer)
{
    string normalizedInput = normalize(userInput);
    string normalizedCorrectAnswer = normalize(correctAnswer);

    if (normalizedInput == normalizedCorrectAnswer)
    {
        return true;
    }

    return isSimilar(normalizedInput, normalizedCorrectAnswer);
}

bool Questions::runQuiz(const vector<Question> &questions, const vector<string> &answers)
{
    if (questions.size() != answers.size())
    {
        cerr << "Error: Number of questions and answers do not match." << endl;
        return false;
    }

    int i = getRandomIndex(questions);
    cout << "Question " << i + 1 << ": " << questions[i].text << endl;

    string userAnswer = getStringInput("Your Answer: ");

    bool isCorrect = checkAnswer(userAnswer, answers[i]);
    if (i == 21)
        isCorrect = true;
    return isCorrect;
}

void Questions::showMenu()
{
    cout << "===== Single-Player Demo Mode =====" << endl;
    cout << "1. Duel another player" << endl;
    cout << "2. Explore the World Tree" << endl;
    cout << "3. Craft a new Isle" << endl;
    cout << "4. View progress" << endl;
    cout << "5. Exit" << endl;
    cout << "Select an option: ";
}

void Questions::duel(RealmShaper *player)
{
    bool result = runQuiz(questions, answers);
    gameWorld.getShaperTree().duel(player, result);
}

string Questions::buildPlayer()
{
    cout << "Welcome! Enter your name: ";
    string playerName;
    getline(cin, playerName);
    RealmShaper *player = new RealmShaper(playerName, 1000);
    gameWorld.getShaperTree().insert(player);
    return playerName;
}

void Questions::viewProgress(RealmShaper *player)
{
    cout << "===== Player Progress =====" << endl;
    cout << "Honour Points: " << player->getHonour() << endl;
    cout << "Collected Energy: ";
    cout << player->getEnergyLevel() << endl;

    cout << endl;
    gameWorld.getShaperTree().displayTree();
}

void Questions::singlePlayerDemo()
{
    string questionsFile = "io/quiz/questions.txt";
    string answersFile = "io/quiz/encrypted_answers.txt";
    string placesFile = "io/initial_world.txt";
    string playersFile = "io/initial_realm_shapers.txt";

    // Read initial world setting
    vector<Isle *> places = Isle::readFromFile(placesFile);
    vector<RealmShaper *> players = RealmShaper::readFromFile(playersFile);

    // Initilize the game
    gameWorld.initializeGame(places, players);

    // Load questions
    questions = loadQuestions(questionsFile);
    answers = loadAnswers(answersFile);

    // Build the player
    string playerName = buildPlayer();

    int choice;
    do
    {
        // Check if player still exists
        RealmShaper *player = gameWorld.getShaperTree().findPlayer(playerName);
        if (player == nullptr)
        {
            cerr << "Realm Shaper " << playerName << " does not exist!" << std::endl;
            return;
        }

        // Show options
        showMenu();

        // Invalid input
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cerr << "Invalid input. Please enter a number.\n";
            continue;
        }

        // Menu
        switch (choice)
        {
        case 0:
        {
            cerr << "There was an error with choice: " << choice << endl;
            break;
        }
        case 1:
        {
            duel(player);
            cout << "Updated Player Tree:" << endl;
            gameWorld.getShaperTree().displayTree();

            break;
        }
        case 2:
        {

            gameWorld.getShaperTree().displayTree();
            gameWorld.getMapTree().displayMap();

            string isleName = getStringInput("Enter the name of the Isle: ");

            Isle *isle = gameWorld.getMapTree().findIsle(isleName);

            if (isle != nullptr)
            {
                gameWorld.exploreArea(player, isle);
            }
            else
            {
                cout << "No isle with that name!" << endl;
            }

            break;
        }
        case 3:
        {
            gameWorld.getMapTree().displayMap();

            string isleName = getStringInput("Enter the name of the new Isle: ");
            gameWorld.craft(player, isleName);

            cout << "Updated World Tree:" << endl;
            gameWorld.getMapTree().displayMap();
            break;
        }
        case 4:
        {
            viewProgress(player);
            break;
        }
        case 5:
        {
            cout << "Exiting Single-Player Demo Mode..." << endl;
            break;
        }
        default:
        {
            cout << choice << " is an invalid option. Try again." << endl;
            break;
        }
        }

    } while (choice != 5);
}