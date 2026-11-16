#include "Pet.h"

#include <algorithm>

void play(Pet& pet) {
    if (pet.sleeping) {
        return;
    }

    pet.fun = std::clamp(pet.fun + 20.0f, 0.0f, 100.0f);
    pet.energy = std::clamp(pet.energy - 5.0f, 0.0f, 100.0f);
    pet.cleanliness =
        std::clamp(pet.cleanliness - 5.0f, 0.0f, 100.0f);
}