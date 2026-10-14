// ============================================================
// File: init.cpp  |  Module M-1: Campus Initialiser


//header file --->>>>
#include "functions.h"

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//  readConfig
//  Opens config.txt, reads 3 integers.
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/


// readconfig present in main.cpp and values are passed by reference 
void readConfig(int &numDepts, int &numFloors, int &numRooms)
{
    //checking is that config.txt file exist or not??
    ifstream fin("config.txt");
    //if exist open it
    if (fin.is_open()) {
        fin >> numDepts>> numFloors >> numRooms;
        fin.close(); //close it now

        cout << "[CONFIG] Loaded: "<<endl;

        cout << "NUMBER OF DEPARTMENTS: " << numDepts << endl;

        cout << "NUMBER OF FLOORS:      " << numFloors << endl;

        cout << "NUMBER OF ROOMS:       " << numRooms << endl;    
    } 
    //if config.txt not exist then we are gonna create it!
    else {
        cout << "[CONFIG] config.txt not found. Enter values manually:"<<endl;

        cout << "  Enter Number of Departments :"<<endl;
        cin >> numDepts;

        cout << "  Enter Number of Floors      : "<<endl;
        cin >> numFloors;

        cout << "  Enter Number of Rooms/Floor : "<<endl;
         cin >> numRooms;

        cin.ignore(1000, '\n');


        //place these values of room, floor and department in same way in config.txt
        ofstream fout("config.txt");
        fout << numDepts  <<endl
             << numFloors <<endl
             << numRooms  <<endl;
        fout.close();//close it 

        cout << "[CONFIG] Saved to config.txt"<<endl;

    }
}

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//  initCampus
//  Allocates int*** grid on heap.
//  Initialises every room to 0 (vacant).
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

int*** initCampus(int numDepts, int numFloors, int numRooms){

    //allocating memory to grid in heap 
    int*** grid = new int**[numDepts];

    //allocating memory to departments in heap
    for (int d = 0; d < numDepts; d++) {

        *(grid + d) = new int*[numFloors];

      // allocating memory to floors in heap
        for (int f = 0; f < numFloors; f++) {
            
            *(*(grid + d) + f) = new int[numRooms];

            for (int r = 0; r < numRooms; r++){

            //placed equal to 0 so rooms will contain zero else it will contain garbage value
                *(*(*(grid + d) + f) + r) = 0;
            }
        }
    }

    cout << "[INIT] Campus grid allocated: "<< numDepts << "x" << numFloors << "x" << numRooms <<endl;
    return grid;
}

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//  freeCampus
//  Deallocates the 3D grid in reverse order.
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
void freeCampus(int*** grid, int numDepts, int numFloors){
    //deleting from room to floor to then department.....

    for (int d = 0; d < numDepts; d++) {

        for (int f = 0; f < numFloors; f++)
            delete[] *(*(grid + d) + f);
        delete[] *(grid + d);
    }
    delete[] grid;
}