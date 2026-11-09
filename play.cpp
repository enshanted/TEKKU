/* IF the user presses the play key, tickle the pet,
Increase fun by 20%, decrease energy and cleanliness by 5%
DISPLAY “The pet is playing!” and the updated status
*/

#include <iostream>

#include "Pet.h"
using namespace std;

void play (Pet& pet) {
    //increasing fun by 20%
    if (pet.fun + 20 <= 100) { // assuming na "fun" yung gagawing varname for this
        pet.fun += 20;
    } else
        pet.fun = 100;

    //decreasing energy
    if (pet.energy - 5 >= 0) {
        pet.energy -= 5;
    } else
        pet.energy =0;

    // decreasing cleanliness
    if (pet.cleanliness - 5 >= 0) {
        pet.cleanliness -= 5;
    } else
        pet.cleanliness =0;

    // DISPLAY “The pet is playing!” and the updated status
    // gagana lang yung pet name if na initialize na siya sa pet.cpp
    cout << pet.name << " is playing! Tickle them more!" << endl;

    printStatus(pet); // displays updated status
}

