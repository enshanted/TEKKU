
#include <iostream>
#include "pet.h"
#include <algorithm> //para magstay sya sa 0% - 100% range; (aina: idk if may error dito basta di gumagana this directive sa compiler ko)
using namespace std;


void feed(Pet& pet) {
    int food;

    if (pet.nourishment == 100) {
        cout << "I'm already full. I don't want to eat" << endl;
    }
    else {
        cout << "\n===== FOOD MENU =====" << endl;
        cout << "[1] Burger" << endl;
        cout << "[2] Sinigang" << endl;
        cout << "[3] Dubai Chocolate" << endl;

        cout << "Choose a food to feed your pet: ";
        cin >> food;

        switch (food) //for determining food selection and its effects on the pet's attributes
        {
            case 1:
                pet.nourishment += 30;
                cout << "Your pet enjoyed the Burger!" << endl;
                break;
            case 2:
                pet.nourishment += 70;
                cout << "Your pet enjoyed the Sinigang!" << endl;
                break;
            case 3:
                pet.nourishment += 10;
                pet.fun += 50;
                pet.cleanliness -= 5;
                cout << "Your pet enjoyed the Dubai Chocolate!" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }

        if (pet.nourishment > 100) {
           pet.nourishment = 100;
        }

        if (pet.fun > 100) {
            pet.fun = 100;
        }


        if (pet.cleanliness > 100) {
            pet.cleanliness = 100;
        }

        cout << "\n===== UPDATED STATUS =====" << endl;
       /* cout << "Nourishment: " << pet.nourishment << "%" << endl;
        cout << "Fun: " << pet.fun << "%" << endl;
        cout << "Cleanliness: " << pet.cleanliness << "%" << endl;
        */
        printStatus(pet);
    }
}

/*
 UPDATES:
 - tinanggal mga declaration of nourishment, cleanliness, etc.
 ( nasa pet.h na talaga sha huhu)
 then nilagyan ko ng "pet." mga variables to connect with pet.h
*/
