// ============================================================
// File: simulation.cpp  |  Module M-3: Academic Simulation

#include "functions.h"

static void shrinkArrays(char** &names, int* &ids,float* &gpas,  int* &statuses,int idx, int n)
{
    // free name string
    delete[] *(names + idx);

    // shift left
    for (int i=idx; i<n-1; i++) 
    {
        *(names    + i) = *(names    + i + 1);
        *(ids      + i) = *(ids      + i + 1);
        *(gpas     + i) = *(gpas     + i + 1);
        *(statuses + i) = *(statuses + i + 1);
    }

    int allocSize = (n - 1 > 0) ? n - 1 : 1;

    char** newNames = new char*[allocSize];
    for (int i = 0; i < n - 1; i++) *(newNames + i) = *(names + i);
    delete[] names; names = newNames;

    int* newIds = new int[allocSize];
    for (int i = 0; i < n - 1; i++) *(newIds + i) = *(ids + i);
    delete[] ids; ids = newIds;

    float* newGpas = new float[allocSize];
    for (int i = 0; i < n - 1; i++) *(newGpas + i) = *(gpas + i);
    delete[] gpas; gpas = newGpas;

    int* newStatuses = new int[allocSize];
    for (int i = 0; i < n - 1; i++) *(newStatuses + i) = *(statuses + i);
    delete[] statuses; statuses = newStatuses;
}

// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//  clearStudentRoom  
// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/

static void clearStudentRoom(int*** grid, int studentID,int nd, int nf, int nr)
{
    for (int d = 0; d < nd; d++)
        for (int f = 0; f < nf; f++)
            for (int r = 0; r < nr; r++)
                if (*(*(*(grid + d) + f) + r) == studentID)
                    *(*(*(grid + d) + f) + r) = 0;
}

// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//  runTimeStep
//
//  Event table (rand() % 10 + 1):
//    1-5  Progress  : GPA += 0.1, cap 4.00
//    6-8  Steady    : no change
//    9    Warning   : GPA -= 0.2, floor 0.00, status=WARNING
//    10   Graduate  : clear room, remove from arrays
//
//  Bitwise flag (unsigned char) per student:
//    bit 0 (1) = progress   — used to guard progress print
//    bit 1 (2) = warning    — used to guard warning print
//    bit 2 (4) = graduated  — used to guard graduation print
// /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
void runTimeStep(int*** grid,char** &names, int* &ids,float* &gpas,  int* &statuses,int &numStudents,int nd, int nf, int nr)
{
    cout << "\n=== TIME-STEP RESULTS ===\n";

    int i = 0;
    while (i < numStudents) 
    {

        unsigned char flags = 0;          // fresh flag for each student
        int event = rand() % 10 + 1;     // 1..10

        //  1-5: Academic Progress
        if (event >= 1 && event <= 5) 
        {
            flags = flags | 1;            // set bit 0 = progress

            *(gpas + i) = *(gpas + i) + 0.1f;
            if (*(gpas + i) > 4.00f)
                *(gpas + i) = 4.00f;      // cap with conditional

            *(statuses + i) = 0;          // STUDYING

            // use bitwise flag to guard this print
            if (flags & 1)
                cout << "  [PROGRESS]   " << *(names + i)<< "   GPA=" << fixed << setprecision(2)<< *(gpas + i) << "\n";
            i++;

        } 
        
        // 6-8: Steady 
        else if (event >= 6 && event <= 8) 
        {

            cout << "  [STEADY]     " << *(names + i) << "\n";
            i++;

        } 

        // 9-Warning
        else if (event == 9) 
        {

            flags = flags | 2;            // set bit 1 = warning

            *(statuses + i) = 1;          // WARNING
            *(gpas + i) = *(gpas + i) - 0.2f;
            if (*(gpas + i) < 0.00f)
                *(gpas + i) = 0.00f;      // floor with conditional

            // use bitwise flag to guard this print
            if (flags & 2)
                cout << "  [WARNING]    " << *(names + i)<< "   GPA=" << fixed << setprecision(2)<< *(gpas + i) << "\n";
            i++;

        // 10- Graduation 
        } 

        else 
        {

            flags = flags | 4;            // set bit 2 = graduated

            *(statuses + i) = 2;          // GRADUATING
            int sid = *(ids + i);

            // full triple-nested scan, no shortcuts
            clearStudentRoom(grid, sid, nd, nf, nr);

            // use bitwise flag to guard graduation print
            if (flags & 4)
                cout << "  [GRADUATED]  " << *(names + i) << "   ID=" <<sid<< "   (room cleared)\n";

            // compact and shrink arrays
            shrinkArrays(names, ids, gpas, statuses, i, numStudents);
            numStudents--;
            // do NOT increment i
        }
    }

    cout << "=========================\n";
    cout << "  Students remaining: " << numStudents << "\n";
}