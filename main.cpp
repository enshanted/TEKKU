#include <chrono>
#include <cctype>
#include <iostream>
#include <string>

#include "Pet.h"

using namespace std;

void showGuide() {
    cout << "\n--- TEKKU KEY GUIDE ---\n"
         << "H - Show this guide\n"
         << "S - Check pet status\n"
         << "F - Feed\n"
         << "P - Play/tickle\n"
         << "Z - Sleep or wake\n"
         << "W - Shower\n"
         << "Q - Save and quit\n";
}

int main() {
    Pet pet;
    bool loadedGame = false;

    cout << "Welcome to Tekku!\n";
    cout << "Load a saved game? (y/n): ";

    char loadChoice;
    cin >> loadChoice;
    loadChoice = static_cast<char>(
        tolower(static_cast<unsigned char>(loadChoice))
    );

    if (loadChoice == 'y') {
        loadedGame = loadGame(pet);

        if (loadedGame) {
            cout << "Saved game loaded.\n";
            printStatus(pet);
        } else {
            cout << "No saved game was found. Starting a new game.\n";
        }
    }

    if (!loadedGame) {
        cout << "Would you like to hatch a pet? (y/n): ";

        char hatchChoice;
        cin >> hatchChoice;
        hatchChoice = static_cast<char>(
            tolower(static_cast<unsigned char>(hatchChoice))
        );

        if (hatchChoice != 'y') {
            cout << "The egg was not hatched. Goodbye!\n";
            return 0;
        }

        cout << "\nChoose your pet:\n"
             << "1 - Cat\n"
             << "2 - Dog\n"
             << "Choice: ";

        int petChoice;
        cin >> petChoice;

        string species;
        if (petChoice == 2) {
            species = "Dog";
        } else {
            species = "Cat";
            if (petChoice != 1) {
                cout << "Invalid choice. Choosing Cat.\n";
            }
        }

        string name;
        cout << "Name your " << species << ": ";
        getline(cin >> ws, name);

        initializePet(pet, species, name);
        saveGame(pet);
    }

    showGuide();

    using Clock = chrono::steady_clock;
    auto lastUpdate = Clock::now();
    bool running = true;

    while (running) {
        cout << "\nChoose an action: ";

        char action;
        cin >> action;
        action = static_cast<char>(
            toupper(static_cast<unsigned char>(action))
        );

        auto now = Clock::now();
        float elapsedSeconds =
            chrono::duration<float>(now - lastUpdate).count();

        updatePet(pet, elapsedSeconds);
        updateSleep(pet, elapsedSeconds);
        lastUpdate = now;

        if (action == 'H') {
            showGuide();
        } else if (action == 'S') {
            printStatus(pet);
        } else if (action == 'F') {
            feed(pet);
        } else if (action == 'P') {
            play(pet);
        } else if (action == 'Z') {
            toggleSleep(pet);
        } else if (action == 'W') {
            shower(pet);
        } else if (action == 'Q') {
            if (saveGame(pet)) {
                cout << "Game saved. Goodbye!\n";
            } else {
                cout << "Could not save the game.\n";
            }
            running = false;
            continue;
        } else {
            cout << "Unknown key. Press H to see the guide.\n";
        }

        if (!saveGame(pet)) {
            cout << "Could not save the game.\n";
        }
    }

    return 0;
}
