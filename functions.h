// ============================================================
// UniVault - University Record & Simulation System


#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <ctime>
using namespace std;

// M-1
int***  initCampus(int nd, int nf, int nr);
void    freeCampus(int*** grid, int nd, int nf);
void    readConfig(int &nd, int &nf, int &nr);

// M-2
void    enrollStudent(int*** grid,char** &names, int* &ids,float* &gpas,  int* &statuses,int &numStudents,int nd, int nf, int nr);

// M-3
void    runTimeStep(int*** grid,char** &names, int* &ids,float* &gpas,  int* &statuses,int &numStudents,int nd, int nf, int nr);

// M-4
void    displayDashboard(int*** grid, char** names, int* ids, float* gpas,  int* statuses,int numStudents,int nd, int nf, int nr,int* currentDept,int* currentFloor,int  step);

// M-5
void    findStudent     (int*** grid,char** names, int* ids, float* gpas,  int* statuses, int numStudents,int nd, int nf, int nr);

void    jumpView        (int* currentDept, int* currentFloor,int nd, int nf);

// M-6
void    saveData        (int*** grid,
                         char** names, int* ids,float* gpas,  int* statuses, int numStudents,int nd, int nf, int nr);

bool    loadData        (int*** grid, char** &names, int* &ids,float* &gpas,  int* &statuses,int &numStudents,
                         int nd, int nf, int nr);

#endif