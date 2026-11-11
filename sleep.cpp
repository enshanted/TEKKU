#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include "Pet.h"

using namespace std;

//int sleepLevel = 0; should use pet.energy

void sleepPet(Pet& pet) {
    pet.energy = clamp(pet.energy, 0.0f, 100.0f); // changed numbers compatible to float
    if (pet.energy == 100.0f) {
        cout << "Your pet is fully rested!" << endl;
        return;
    }

    // Restore 20 points on sleep every 5 seconds interval until full
    while (pet.energy < 100.0f) {
        this_thread::sleep_for(chrono::seconds(5));
        const float gained = min(20.0f, 100.0f - pet.energy); // changed int to float
        pet.energy += gained;
        cout << "+" << gained << " sleep. Your pet is resting. ("
             << pet.energy << "%)" << endl;
    }

    cout << "Your pet is fully rested!" << endl;
}

/*
 UPDATES:
 - removed variable sleepLevel
 - changed sleepLevel to pet.energy
 - changed int to float, adjusted number formats to float
 */
