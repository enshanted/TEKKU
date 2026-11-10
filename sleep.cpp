#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;

int sleepLevel = 0;

void sleep() {
    sleepLevel = clamp(sleepLevel, 0, 100);
    if (sleepLevel == 100) {
        cout << "Your pet is fully rested!" << endl;
        return;
    }

    // Restore 20 points on sleep every 5 seconds interval until full
    while (sleepLevel < 100) {
        this_thread::sleep_for(chrono::seconds(5));
        const int gained = min(20, 100 - sleepLevel);
        sleepLevel += gained;
        cout << "+" << gained << " sleep. Your pet is resting. ("
             << sleepLevel << "%)" << endl;
    }

    cout << "Your pet is fully rested!" << endl;
}
