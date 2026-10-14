//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// File: dashboard.cpp  |  Module M-4: Live Dashboard

#include "functions.h"

//string array for checking student status
static const char* statusStr(int s)
{
    if (s == 0){
        return "STUDYING";
    }
    if (s == 1)
    {
        return "WARNING";
    }
    else
    {
        return "GRADUATING";
    }
}

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//  displayDashboard
// //\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

void displayDashboard(int*** grid,char** names,int* ids,float* gpas,int* statuses,int numStudents,int nd,int nf,int nr,int* currentDept,int* currentFloor,int  step){

// it will clear terminal screen run for previous input or output
    #ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    int cd = *currentDept;
    int cf = *currentFloor;

    cout << "-----------------------------------------------------------------------------"<<endl;
    cout << "                    UniVault . LIVE RESOURCE DASHBOARD"<<endl;
    cout << "------------------------------------------------------------------------------"<<endl;
    cout << "[DEPT: ";
    cout << setfill('0')<<right << setw(2) << cd;
    cout << setfill(' ') << "] [FLOOR: ";
    cout << setfill('0')<<right << setw(2) << cf;
    cout << setfill(' ') << "] [ROOMS: ";
    cout << setfill('0')<<right << setw(2) << nr;
    cout << setfill(' ') << "] [STEP: ";
    cout << setfill('0')<< setw(2) << step;
    cout << setfill(' ') << "]"<<endl;

    cout << "---------------------------------------------------------------------------"<<endl;

    cout << left<< setw(14) << "ROOM ADDR"<< setw(8)  << "ID"<< setw(20) << "NAME"<< setw(8)  << "GPA"<< "STATUS"<<endl;

    cout << "---------------------------------------------------------------------------"<<endl;
    
    // nr= number of rooms
    for (int r = 0; r < nr; r++)
    {

        //checking room number by first going to department then floor no then lastly taking address of room
        int* roomPtr  = *(*(grid + cd) + cf) + r;

        //doing dereference to get value of occupant present in pointed room 
        int  occupant = *(roomPtr);

        // This line is printing address of roomptr with proper alignment
        cout << left << setw(14) << (void*)roomPtr;

        if (occupant == 0) {
            // empty rooms
            cout << setw(8)  << "----" << setw(20) << "----------"<< setw(8)  << "0.00"<< "EMPTY"<<endl;
        } 
        
        else {
            // linear search for student in parallel arrays
            //this loop will work go like it will store the student no--> s in idx if occupant found else idx will remain idx=-1 (means no occupant found ) 

            int idx = -1;
            for (int s = 0; s < numStudents; s++) 
            {
                if (*(ids + s) == occupant) 
                {
                    idx = s;
                    break;
                }
            }

            if (idx == -1) 
            {
                // stale entry(invalid record )
                cout << setw(8)  << occupant << setw(20) << "???" << setw(8)  << "0.00" << "STALE"<<endl;
            }
             else 
             {
                const char* suffix = "";
                if (*(statuses + idx) == 1){
                    //!! represents warring
                 suffix = " !!";
                }
                if (*(statuses + idx) == 2){
                    //**  represents important
                     suffix = " **";
                }

//*(ids+idx), *(ids+names),  *(ids+GPAs), *(ids+status)------> student id,name,gpa, status value.

cout<<setw(8)<<*(ids + idx)<<setw(20)<<*(names+idx)<<fixed<<setprecision(2)<<setw(8)<<*(gpas + idx)<<statusStr(*(statuses+idx))<<suffix<<endl;
            }
        }
    }

    cout << "---------------------------------------------------------------------------"<<endl;;
    cout << "COMMANDS: [ENTER] Step | [E] Enroll | [J] Jump | [F] Find | [S] Save | [X] Exit"<<endl;
    cout << "==========================================================================="<<endl;;
}