#include "Pet.h"

#include <iostream>
#include <string>
#include <ctime>      

using namespace std;

// how much each stat goes down EVERY SECOND
const float NOURISHMENT_LOSS = 0.24;   // 100 gone in 7 minutes
const float FUN_LOSS         = 0.24;   // 
const float ENERGY_LOSS      = 0.16;   // 100 gone in 10 mins
const float CLEANLINESS_LOSS = 0.16;   // 

// The pet complains every 60 seconds while a stat is 0%
const int WARNING_TIME = 60;

// The last time (in seconds) the stats were lowered
time_t lastTime;

// Seconds since the last complaint for each stat
// (starts at 60 so the first complaint shows as soon as the stat reaches 0)
int hungerTimer = WARNING_TIME;
int boredTimer  = WARNING_TIME;
int tiredTimer  = WARNING_TIME;
int dirtyTimer  = WARNING_TIME;


// Make a new pet with all stats at 100%
void initializePet(Pet& pet, string species, string name) {
    pet.species = species;
    pet.name = name;

    pet.nourishment = 100;
    pet.fun = 100;
    pet.energy = 100;
    pet.cleanliness = 100;

    lastTime = time(0);   // start counting from now
}


// Lower the stats for every second that passed
// (the second number from main.cpp is not needed anymore, this file counts the time itself)
void updatePet(Pet& pet, float unused) {

    // How many seconds passed since the last update?
    time_t now = time(0);
    int seconds = now - lastTime;

    if (seconds <= 0) {
        return;   // less than 1 second passed, nothing to do
    }
    lastTime = now;

    // Lower each stat: amount per second x number of seconds
    pet.nourishment = pet.nourishment - NOURISHMENT_LOSS * seconds;
    pet.fun         = pet.fun - FUN_LOSS * seconds;
    pet.energy      = pet.energy - ENERGY_LOSS * seconds;
    pet.cleanliness = pet.cleanliness - CLEANLINESS_LOSS * seconds;

    // stat cant go below 0
    if (pet.nourishment< 0) pet.nourishment = 0;
    if (pet.fun< 0)         pet.fun = 0;
    if (pet.energy< 0)      pet.energy = 0;
    if (pet.cleanliness< 0) pet.cleanliness = 0;

    //nourishment
    if (pet.nourishment == 0) {
        hungerTimer = hungerTimer + seconds;
        if (hungerTimer >= WARNING_TIME) {
            cout << "\n" << pet.name << ": I am hungry!\n";
            hungerTimer = 0;
        }
    } else {
        hungerTimer = WARNING_TIME;
    }

    // Fun
    if (pet.fun == 0) {
        boredTimer = boredTimer + seconds;
        if (boredTimer >= WARNING_TIME) {
            cout << "\n" << pet.name << ": I am bored!\n";
            boredTimer = 0;
        }
    } else {
        boredTimer = WARNING_TIME;
    }

    // Energy
    if (pet.energy == 0) {
        tiredTimer = tiredTimer + seconds;
        if (tiredTimer >= WARNING_TIME) {
            cout << "\n" << pet.name << ": I am tired!\n";
            tiredTimer = 0;
        }
    } else {
        tiredTimer = WARNING_TIME;
    }

    // Cleanliness
    if (pet.cleanliness == 0) {
        dirtyTimer = dirtyTimer + seconds;
        if (dirtyTimer >= WARNING_TIME) {
            cout << "\n" << pet.name << ": I am dirty!\n";
            dirtyTimer = 0;
        }
    } else {
        dirtyTimer = WARNING_TIME;
    }
}


// for stats printout
void printStatus(Pet pet) {
    cout << pet.name << " the " << pet.species << "\n";
    cout << "Nourishment: " << pet.nourishment << "%\n";
    cout << "Fun:         " << pet.fun << "%\n";
    cout << "Energy:      " << pet.energy << "%\n";
    cout << "Cleanliness: " << pet.cleanliness << "%\n";
}
