#include <iostream>
#include "menuInput.h"

using namespace std;

int getMenuChoice(const string &label)
{
    int value;
    cout << label;

    if (!(cin >> value))
    {
        cin.clear();
        cin.ignore(1000, '\n');

        cout << "\nPilihan Tidak Valid!\n";
        return -1;  // Error Code in CLI (U can edit it as you want)
    }

    cin.ignore(1000, '\n');
    return value;
}
