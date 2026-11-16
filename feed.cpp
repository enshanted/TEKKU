#include "Pet.h"

#include <algorithm>
#include <iostream>
#include <limits>

std::string feed(Pet& pet, int foodChoice) {
    if (pet.sleeping) {
        return pet.name + " is sleeping. Wake them before feeding.";
    }

    if (pet.nourishment >= 100.0f) {
        return pet.name + " is full and doesn't want to eat.";
    }

    std::string message;

    if (foodChoice == 1) {
        pet.nourishment += 30.0f;
        message = pet.name + " enjoyed the burger!";
    } else if (foodChoice == 2) {
        pet.nourishment += 70.0f;
        message = pet.name + " enjoyed the sinigang!";
    } else if (foodChoice == 3) {
        pet.nourishment += 10.0f;
        pet.fun += 50.0f;
        pet.cleanliness -= 5.0f;
        message = pet.name + " enjoyed the Dubai Chewy!";
    } else {
        return "Invalid food choice.";
    }

    pet.nourishment = std::clamp(pet.nourishment, 0.0f, 100.0f);
    pet.fun = std::clamp(pet.fun, 0.0f, 100.0f);
    pet.cleanliness = std::clamp(pet.cleanliness, 0.0f, 100.0f);

    return message;
}

// Console-menu version, if you still use it elsewhere.
void feed(Pet& pet) {
    if (pet.sleeping) {
        std::cout << pet.name
                  << " is sleeping. Wake them before feeding.\n";
        return;
    }

    if (pet.nourishment >= 100.0f) {
        std::cout << pet.name
                  << " is full and doesn't want to eat.\n";
        return;
    }

    std::cout << "\n--- FOOD MENU ---\n"
              << "1 - Burger\n"
              << "2 - Sinigang\n"
              << "3 - Dubai Chewy\n"
              << "Choose food: ";

    int foodChoice;
    if (!(std::cin >> foodChoice)) {
        std::cin.clear();
        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n'
        );
        std::cout << "Enter a number from 1 to 3.\n";
        return;
    }

    std::cout << feed(pet, foodChoice) << '\n';
    printStatus(pet);
}