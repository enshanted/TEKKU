#include <iostream>
using namespace std;

int cleanliness;

void shower() {
    if (cleanliness >= 100) {
        cout << "Your pet is already clean!" << endl;
        return;
    }

    // One shower fully restores cleanlines. PS should this be timed based din?
    cleanliness = 100;
    cout << "Your pet took a shower. Cleanliness: 100%" << endl;
}
