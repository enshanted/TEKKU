#ifndef PET_H
#define PET_H

#include <string>

struct Pet {
    std::string species;
    std::string name;

    float nourishment = 100.0f;
    float fun = 100.0f;
    float energy = 100.0f;
    float cleanliness = 100.0f;

    bool sleeping = false;

    float hungerWarningElapsed = 0.0f;
    float funWarningElapsed = 0.0f;
    float energyWarningElapsed = 0.0f;
    float cleanlinessWarningElapsed = 0.0f;
};

void initializePet(Pet& pet, const std::string& species,
                   const std::string& name);
void updatePet(Pet& pet, float elapsedSeconds);
void printStatus(const Pet& pet);

bool saveGame(const Pet& pet,
              const std::string& filename = "tekku_save.txt");
bool loadGame(Pet& pet,
              const std::string& filename = "tekku_save.txt");

void feed(Pet& pet);  // Console menu version
std::string feed(Pet& pet, int foodChoice);  // GUI keyboard version

void play(Pet& pet);
void toggleSleep(Pet& pet);
void updateSleep(Pet& pet, float elapsedSeconds);
void shower(Pet& pet);

#endif