#include <iostream>
#include <algorithm> //para magstay sya sa 0% - 100% range
using namespace std;

//attributes
int nourishment = 100;
int fun = 100; 
int cleanliness = 100;

void feed() {
    int food;

    if (nourishment == 100) {
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
                nourishment += 30;
                cout << "Your pet enjoyed the Burger!" << endl;
                break;
            case 2:
                nourishment += 70;
                cout << "Your pet enjoyed the Sinigang!" << endl;
                break;
            case 3:
                nourishment += 10;
                fun += 50;
                cleanliness -= 5;
                cout << "Your pet enjoyed the Dubai Chocolate!" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }

        if (nourishment > 100) {
            nourishment = 100;
        }
        
        if (fun > 100) {
            fun = 100;
        }
        
        if (cleanliness > 100) {
            cleanliness = 100;
        }
        
        cout << "\n===== UPDATED STATUS =====" << endl;
        cout << "Nourishment: " << nourishment << "%" << endl;
        cout << "Fun: " << fun << "%" << endl;
        cout << "Cleanliness: " << cleanliness << "%" << endl;
    }
}

int main() {
    feed();

    return 0;
}
