#ifndef PET_H
#define PET_H

#include <string>
using namespace std;
// pet structure
struct Pet {
    string species;
    string name;

    float nourishment;
    float fun;
    float energy;
    float cleanliness;
};

// Functions written in pet.cpp
void initializePet(Pet& pet, std::string species, std::string name);
void updatePet(Pet& pet, float unused);
void printStatus(Pet pet);

// (revised) declaring functions
void feed(Pet& pet);
void play(Pet& pet);
void sleepPet(Pet& pet);
void shower(Pet& pet);

#endif
