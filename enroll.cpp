// ============================================================
// File: enroll.cpp  |  Module M-2: Student Enrolment


#include "functions.h"

static void growArrays(char** &names, int* &ids,float* &gpas,  int* &statuses, int n)
{
    char** newNames = new char*[n + 1];
    for (int i = 0; i < n; i++) *(newNames + i) = *(names + i);
    delete[] names;
    names = newNames;

    int* newIds = new int[n + 1];
    for (int i = 0; i < n; i++) *(newIds + i) = *(ids + i);
    delete[] ids;
    ids = newIds;

    float* newGpas = new float[n + 1];
    for (int i = 0; i < n; i++) *(newGpas + i) = *(gpas + i);
    delete[] gpas;
    gpas = newGpas;

    int* newStatuses = new int[n + 1];
    for (int i = 0; i < n; i++) *(newStatuses + i) = *(statuses + i);
    delete[] statuses;
    statuses = newStatuses;
}

void enrollStudent(int*** grid, char** &names, int* &ids,float* &gpas,  int* &statuses,int &numStudents,int nd, int nf, int nr)
{
    char tempName[256];
    cout << "\n  Enter student name: ";
    cin.getline(tempName, 256);

    // grow all four arrays
    growArrays(names, ids, gpas, statuses, numStudents);

    // sequential ID starting from 1001
    int newID = 1000 + numStudents + 1;

    // allocate exactly strlen+1 bytes — manual length count
    int len = 0;
    while (*(tempName + len) != '\0') len++;
    *(names + numStudents) = new char[len + 1];
    for (int i = 0; i <= len; i++)
        *(*(names + numStudents) + i) = *(tempName + i);

    *(ids      + numStudents) = newID;
    *(gpas     + numStudents) = 2.50f;
    *(statuses + numStudents) = 0;       // STUDYING

    // random initial room
    int dept  = rand() % nd;
    int floor = rand() % nf;
    int room  = rand() % nr;

    int skipped = 0;

    // if occupied: FULL triple-nested linear search — NO shortcuts
    if (*(*(*(grid + dept) + floor) + room) != 0)
     {
        int  fd = -1, ff = -1, fr = -1;
        for (int d = 0; d < nd; d++) 
        {
            for (int f = 0; f < nf; f++) 
            {
                for (int r = 0; r < nr; r++) 
                {
                    if (*(*(*(grid + d) + f) + r) == 0) 
                    {
                        if (fd == -1) 
                        {   // record first free slot found
                            fd = d; ff = f; fr = r;
                        }
                    } 
                    else 
                    {
                        skipped++;
                    }
                }
            }
        }

        if (fd == -1) 
        {
            cout << "  [ERROR] Campus is full! Cannot enrol student.\n";
            delete[] *(names + numStudents);
            return;
        }

        dept = fd; floor = ff; room = fr;
        cout << "  [INFO] Room was occupied. Skipped "<< skipped << " slot(s).\n";
    }

    // update grid via pointer arithmetic — bracket indexing forbidden
    *(*(*(grid + dept) + floor) + room) = newID;
    numStudents++;

    cout<<"  [ENROLLED] "<< tempName<< "  ID="<< newID<< "  Room=(" << dept << ","<<floor<< ","<<room<< ")\n";
}