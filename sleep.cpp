#include "Pet.h"

#include <algorithm>

void toggleSleep(Pet& pet) {
    if (pet.sleeping) {

        pet.sleeping = false;
    } else if (pet.energy < 100.0f) {

        pet.sleeping = true;
    }
}

void updateSleep(Pet& pet, float elapsedSeconds) {
    if (!pet.sleeping || elapsedSeconds <= 0.0f) {
        return;
    }

    // Recover 1 energy point per second.
    pet.energy = std::clamp(
        pet.energy + elapsedSeconds,
        0.0f,
        100.0f
    );

    // Stop sleeping when energy reaches 100%.
    if (pet.energy >= 100.0f) {
        pet.sleeping = false;
    }
}