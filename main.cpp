// ============================================================
// UniVault - University Record & Simulation System

#include "functions.h"
using namespace std;

int main()
{
    srand((unsigned int)time(0));

    // M-1: read config and allocate campus grid
    int numDepts, numFloors, numRooms;
    readConfig(numDepts, numFloors, numRooms);
    int*** campus = initCampus(numDepts, numFloors, numRooms);

    // initialise parallel arrays — minimum size 1
    char** names    = new char*[1];
    int*   ids      = new int  [1];
    float* gpas     = new float[1];
    int*   statuses = new int  [1];
    int    numStudents = 0;

    // M-6: load saved data if database.txt exists
    loadData(campus, names, ids, gpas, statuses, numStudents,numDepts, numFloors, numRooms);

    // dashboard view state — passed by pointer to display & jump
    int currentDept  = 0;
    int currentFloor = 0;
    int step         = 0;

    // initial draw
    displayDashboard(campus, names, ids, gpas, statuses, numStudents,numDepts, numFloors, numRooms,&currentDept, &currentFloor, step);

    // main command loop
    bool running = true;
    while (running) {

        cout << "\nCommand: ";
        char cmd;
        cin  >> cmd;
        cin.ignore(1000, '\n');

        // convert lowercase to uppercase
        if (cmd >= 'a' && cmd <= 'z') cmd = cmd - 32;

        switch (cmd) {

            case 'E':
                enrollStudent(campus, names, ids, gpas, statuses,numStudents, numDepts, numFloors, numRooms);
                displayDashboard(campus, names, ids, gpas, statuses,numStudents, numDepts, numFloors, numRooms,&currentDept, &currentFloor, step);
                break;

            case 'J':
                jumpView(&currentDept, &currentFloor, numDepts, numFloors);
                displayDashboard(campus, names, ids, gpas, statuses,numStudents, numDepts, numFloors, numRooms,&currentDept, &currentFloor, step);
                break;

            case 'F':
                findStudent(campus, names, ids, gpas, statuses,numStudents, numDepts, numFloors, numRooms);
                break;

            case 'S':
                saveData(campus, names, ids, gpas, statuses,numStudents, numDepts, numFloors, numRooms);
                break;

            case 'X':
                cout << "\n[EXIT] Goodbye!\n";
                running = false;
                break;

            default:
                // any other key = time-step
                step++;
                runTimeStep(campus, names, ids, gpas, statuses, numStudents, numDepts, numFloors, numRooms);
                displayDashboard(campus, names, ids, gpas, statuses,numStudents, numDepts, numFloors, numRooms,&currentDept, &currentFloor, step);
                break;
        }
    }

    // free all heap memory
    for (int i = 0; i < numStudents; i++)
        delete[] *(names + i);
    delete[] names;
    delete[] ids;
    delete[] gpas;
    delete[] statuses;
    freeCampus(campus, numDepts, numFloors);

    return 0;
}