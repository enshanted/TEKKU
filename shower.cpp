#include "Pet.h"

#include <iostream>

void shower(Pet& pet) {
    if (pet.sleeping) {
        std::cout << pet.name
                  << " is sleeping. Wake them before showering.\n";
        return;
    }

    if (pet.cleanliness >= 100.0f) {
        std::cout << pet.name << " is already clean!\n";
        return;
    }

    pet.cleanliness = 100.0f;
    std::cout << pet.name << " took a shower and is clean!\n";
    printStatus(pet);
}