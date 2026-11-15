#include "Pet.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

namespace {
    constexpr float WARNING_INTERVAL = 60.0f;
    constexpr float DECAY_7_MINUTES = 100.0f / 420.0f;
    constexpr float DECAY_10_MINUTES = 100.0f / 600.0f;

    void decreaseStat(float& stat, float lossPerSecond, float elapsedSeconds,
                      float& warningElapsed, const std::string& petName,
                      const std::string& warning) {
        if (elapsedSeconds <= 0.0f) {
            return;
        }

        float startingValue = stat;
        stat = std::clamp(
            stat - lossPerSecond * elapsedSeconds,
            0.0f,
            100.0f
        );

        if (stat <= 0.0f) {
            float timeAtZero = elapsedSeconds;

            // Count only the part of this update spent at 0%.
            if (startingValue > 0.0f) {
                timeAtZero = std::max(
                    0.0f,
                    elapsedSeconds - startingValue / lossPerSecond
                );
            }

            warningElapsed += timeAtZero;

            while (warningElapsed >= WARNING_INTERVAL) {
                std::cout << petName << ": " << warning << '\n';
                warningElapsed -= WARNING_INTERVAL;
            }
        } else {
            warningElapsed = 0.0f;
        }
    }
}

void initializePet(Pet& pet, const std::string& species,
                   const std::string& name) {
    pet.species = species;
    pet.name = name;

    pet.nourishment = 100.0f;
    pet.fun = 100.0f;
    pet.energy = 100.0f;
    pet.cleanliness = 100.0f;
    pet.sleeping = false;

    pet.hungerWarningElapsed = 0.0f;
    pet.funWarningElapsed = 0.0f;
    pet.energyWarningElapsed = 0.0f;
    pet.cleanlinessWarningElapsed = 0.0f;
}

void updatePet(Pet& pet, float elapsedSeconds) {
    decreaseStat(
        pet.nourishment, DECAY_7_MINUTES, elapsedSeconds,
        pet.hungerWarningElapsed, pet.name, "I am hungry!"
    );

    decreaseStat(
        pet.fun, DECAY_7_MINUTES, elapsedSeconds,
        pet.funWarningElapsed, pet.name, "I am bored!"
    );

    // Energy recovers while sleeping, so it does not passively decrease then.
    if (!pet.sleeping) {
        decreaseStat(
            pet.energy, DECAY_10_MINUTES, elapsedSeconds,
            pet.energyWarningElapsed, pet.name, "I am tired!"
        );
    }

    decreaseStat(
        pet.cleanliness, DECAY_10_MINUTES, elapsedSeconds,
        pet.cleanlinessWarningElapsed, pet.name, "I am dirty!"
    );
}

void printStatus(const Pet& pet) {
    std::cout << "\n" << pet.name << " the " << pet.species << "'s status:\n"
              << "Nourishment: " << pet.nourishment << "%\n"
              << "Fun:         " << pet.fun << "%\n"
              << "Energy:      " << pet.energy << "%\n"
              << "Cleanliness: " << pet.cleanliness << "%\n";
}

bool saveGame(const Pet& pet, const std::string& filename) {
    std::ofstream file(filename);

    if (!file) {
        return false;
    }

    file << std::quoted(pet.species) << '\n'
         << std::quoted(pet.name) << '\n'
         << std::setprecision(7)
         << pet.nourishment << '\n'
         << pet.fun << '\n'
         << pet.energy << '\n'
         << pet.cleanliness << '\n'
         << pet.sleeping << '\n';

    return static_cast<bool>(file);
}

bool loadGame(Pet& pet, const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        return false;
    }

    Pet loaded;
    if (!(file >> std::quoted(loaded.species)
               >> std::quoted(loaded.name)
               >> loaded.nourishment
               >> loaded.fun
               >> loaded.energy
               >> loaded.cleanliness
               >> loaded.sleeping)) {
        return false;
    }

    loaded.nourishment = std::clamp(loaded.nourishment, 0.0f, 100.0f);
    loaded.fun = std::clamp(loaded.fun, 0.0f, 100.0f);
    loaded.energy = std::clamp(loaded.energy, 0.0f, 100.0f);
    loaded.cleanliness = std::clamp(loaded.cleanliness, 0.0f, 100.0f);

    pet = loaded;
    return true;
}
