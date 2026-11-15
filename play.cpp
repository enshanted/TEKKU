#include "Pet.h"

#include <algorithm>
#include <iostream>

void play(Pet& pet) {
    if (pet.sleeping) {
        std::cout << pet.name << " is sleeping. Wake them before playing.\n";
        return;
    }

    pet.fun = std::clamp(pet.fun + 20.0f, 0.0f, 100.0f);
    pet.energy = std::clamp(pet.energy - 5.0f, 0.0f, 100.0f);
    pet.cleanliness = std::clamp(
        pet.cleanliness - 5.0f, 0.0f, 100.0f
    );

    std::cout << pet.name << " is playing!\n";
    printStatus(pet);
}
