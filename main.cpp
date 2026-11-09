#include <chrono>
#include <cctype>
#include <iostream>
#include <string>

#include "Pet.h"

using namespace std;

void showGuide() {

// This is tentative (sa SFML iciclick lang ng user sa screen not type the corresponding letters)
    cout << "\nKEY GUIDE: \n"
         << "H - Show this guide\n"
         << "S - Check pet status\n"
         << "F - Feed\n"
         << "P - Play/tickle\n"
         << "Z - Sleep\n"
         << "W - Shower\n"
         << "Q - Quit\n";
}

int main() {
    string answer;

    // Hatch a pet
    cout << "Would you like to hatch a pet? (y/n): ";
    cin >> answer;

    if (answer != "y" && answer != "Y") {
        cout << "The egg was not hatched. Goodbye!\n";
        return 0;
    }

    // Choose a pet
    int petChoice;
    cout << "\nChoose your pet:\n"
         << "1 - Cat\n"
         << "2 - Dog\n"
         << "Choice: ";
    cin >> petChoice;

    string species;

    if (petChoice == 1) {
        species = "Cat";
    } else if (petChoice == 2) {
        species = "Dog";
    } else {
        cout << "Invalid choice. Choosing Cat.\n";
        species = "Cat";
    }

    // Name the pet
    string name;
    cout << "Name your " << species << ": ";
    getline(cin >> ws, name);

    Pet pet;
    initializePet(pet, species, name);

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
            sleepPet(pet);
            lastUpdate = Clock::now();
        } else if (action == 'W') {
            shower(pet);
        } else if (action == 'Q') {
            running = false;
        } else {
            cout << "Unknown key. Press H to see the guide.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}