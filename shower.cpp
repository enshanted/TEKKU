//
// Created by Alaina Mae Roguel on 11/11/26.
//
#include "Pet.h"
#include <iostream>
using namespace std;

void shower(Pet& pet) {
    if (pet.cleanliness >= 100) { // para di global variable
        cout << "Your pet is already clean!" << endl;
        return;
    }

    // One shower fully restores cleanlines. PS should this be timed based din?
    pet.cleanliness = 100;
    cout << "Your pet took a shower. Cleanliness: 100%" << endl;
}

//UPDATE: removed cleanliness declaration, changed cleanliness to pet.cleanliness
