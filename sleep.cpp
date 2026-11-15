#include "Pet.h"

#include <algorithm>
#include <iostream>

void toggleSleep(Pet& pet) {
    if (pet.sleeping) {
        pet.sleeping = false;
        std::cout << pet.name << " woke up.\n";
        return;
    }

    if (pet.energy >= 100.0f) {
        std::cout << pet.name << " is already fully rested.\n";
        return;
    }

    pet.sleeping = true;
    std::cout << pet.name << " is now sleeping.\n";
}

void updateSleep(Pet& pet, float elapsedSeconds) {
    if (!pet.sleeping || elapsedSeconds <= 0.0f) {
        return;
    }

    // Recover 1 energy point per second.
    pet.energy = std::clamp(
        pet.energy + elapsedSeconds, 0.0f, 100.0f
    );

    if (pet.energy >= 100.0f) {
        pet.sleeping = false;
        std::cout << pet.name << " is fully rested!\n";
    }
}
