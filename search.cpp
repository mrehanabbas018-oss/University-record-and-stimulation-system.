// ============================================================
// File: search.cpp  |  Module M-5: Search & Teleport

#include "functions.h"

// ─────────────────────────────────────────────────────────────
//  findStudent  [F]
// ─────────────────────────────────────────────────────────────
void findStudent(int*** grid,char** names, int* ids,float* gpas,  int* statuses,int numStudents,int nd, int nf, int nr){
    int target;
    // Taking id of target student 

    cout << " Enter Student ID to find: ";
    cin  >> target;
    cin.ignore(1000, '\n');

    // linear search through ids[]
    int idx = -1;
    //checking is that target student present or not?

    for (int i = 0; i < numStudents; i++) {
        if (*(ids + i) == target) {
            idx = i;
            break;
        }
    }
    // idx=-1 means not found!!

    if (idx == -1) {
        cout << "\n  *** NOT FOUND ***\n";
        cout << "  No student with ID " << target << " exists."<<endl;
        return;
    }
   
    // checking state of student with idx:

    const char* stat;
    if(*(statuses + idx) == 0) 
    {
        stat = "STUDYING";
    }
    else if (*(statuses + idx) == 1){
        stat = "WARNING";
    }
    else{            
    stat = "GRADUATING";
    }

    
    //fd: floor department (ya found department index) ,ff: found floor,  fr:  found room

    int fd = -1, ff = -1, fr = -1;
    // nd=number of department
    //nf=number of floor
    //nr=number of room
    for (int d = 0; d < nd; d++)
        for (int f = 0; f < nf; f++)
            for (int r = 0; r < nr; r++) 
            //(*(*(*(grid + d) + f) + r)==student id
                if (*(*(*(grid + d) + f) + r) == target) 
                {
                    fd = d;
                    ff = f; 
                    fr = r;
                }
    cout<<endl;

    cout << " +--------------------------------------------------+"<<endl;;
    cout << "  |              STUDENT RECORD                      |"<<endl;
    cout << "  +--------------------------------------------------+"<<endl;

    cout << "  |  ID     : " << *(ids+idx)<<endl<< "  |  Name   : " << *(names+idx)<<endl<< fixed << setprecision(2);
    cout << "  |  GPA    : " << *(gpas+idx)<<endl<<"  |  Status : " <<stat<<endl;

    if (fd != -1) 
    {
        cout << "  |  Dept   : " << fd << endl;
        cout << "  |  Floor  : " << ff << endl;
        cout << "  |  Room   : " << fr << endl;
        int* roomAddr = *(*(grid + fd) + ff) + fr;
        cout << "  |  Address  : " << (void*)roomAddr << endl;
    } 
    else {
        cout << "  |  Room   : (not found in grid)"<<endl;
    }
    cout << "  +--------------------------------------------------+"<<endl;
}

// ─────────────────────────────────────────────────────────────
//  jumpView  [J]
// ─────────────────────────────────────────────────────────────

void jumpView(int* currentDept, int* currentFloor, int nd, int nf){
    int dept, floor;
    cout<<endl;
    cout << " Enter Department index (0 to " << nd - 1 << "): ";
    cin  >> dept;
    cin.ignore(1000, '\n');

    //validation of department
    if (dept < 0 || dept >= nd) 
    {
        cout << "  [ERROR] Invalid department. Jump cancelled.\n";
        return;
    }

    cout << "  Enter Floor index (0 to " << nf - 1 << "): ";
    cin  >> floor;
    cin.ignore(1000, '\n');

    //validation of floors
    if (floor < 0 || floor >= nf) 
    {
        cout << "  [ERROR] Invalid floor. Jump cancelled.\n";
        return;
    }

    *currentDept  = dept;
    *currentFloor = floor;

    cout << "  [JUMP] View -> Dept=" << dept<< "  Floor=" << floor <<endl;
}