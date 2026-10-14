// ============================================================
// File: persistence.cpp  |  Module M-6: Persistence Layer


#include "functions.h"

// /\/\/\/\/\/\//\/\/\/\/\/\//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
//  saveData  [S]
// /\/\/\/\/\/\//\/\/\/\/\/\//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
void saveData(int*** grid, char** names, int* ids, float* gpas, int* statuses, int numStudents, int nd, int nf, int nr)
{
    ofstream fout("database.txt");
    if (!fout.is_open())
    {
        cout << "  [ERROR] Cannot open database.txt for writing.\n";
        return;
    }

    fout << numStudents << "\n";

    for (int i = 0; i < numStudents; i++)
    {
        int sid = *(ids + i);

        // full triple-nested scan — no shortcuts (spec rule)
        int sd = -1, sf = -1, sr = -1;
        for (int d = 0; d < nd; d++)
            for (int f = 0; f < nf; f++)
                for (int r = 0; r < nr; r++)
                    if (*(*(*(grid + d) + f) + r) == sid)
                    {
                        sd = d; sf = f; sr = r;
                    }


        fout << sid << " "
             << *(names + i) << " "
             << fixed << setprecision(2) << *(gpas + i) << " "
             << *(statuses + i) << " "
             << sd << " "
             << sf << " "
             << sr << "\n";
    }

    // grid occupancy lines
    for (int d = 0; d < nd; d++)
        for (int f = 0; f < nf; f++)
            for (int r = 0; r < nr; r++)
            {
                int occ = *(*(*(grid + d) + f) + r);
                if (occ != 0)
                    fout << d << " " << f << " "
                         << r << " " << occ << "\n";
            }

    fout.close();
    cout << "  [SAVED] " << numStudents << " student(s) written to database.txt\n";
}

// ─────────────────────────────────────────────────────────────
//  loadData  (auto on startup)
//
//  Reads database.txt 
// ─────────────────────────────────────────────────────────────
bool loadData(int*** grid, char** &names, int* &ids, float* &gpas, int* &statuses, int &numStudents, int nd, int nf, int nr)
{
    ifstream fin("database.txt");
    if (!fin.is_open())
    {
        cout << "  [INFO] No database.txt found. Starting fresh.\n";
        return false;
    }

    fin >> numStudents;
    fin.ignore(1000, '\n');
    if (numStudents < 0) numStudents = 0;

    int allocSize = (numStudents > 0) ? numStudents : 1;
    names    = new char*[allocSize];
    ids      = new int  [allocSize];
    gpas     = new float[allocSize];
    statuses = new int  [allocSize];

    for (int i = 0; i < numStudents; i++)
    {
        char lineBuf[512];
        fin.getline(lineBuf, 512);

        // parse line manually using pointer arithmetic
        char* ptr = lineBuf;
        char  tok[256];
        int   t;

        //  read id
        t = 0;
        while (*ptr == ' ') ptr++;
        while (*ptr != ' ' && *ptr != '\0') { *(tok + t) = *ptr; t++; ptr++; }
        *(tok + t) = '\0';
        int sid = 0;
        for (int k = 0; *(tok + k) != '\0'; k++)
            sid = sid * 10 + (*(tok + k) - '0');

        // ── read name 

        char nameBuf[256];
        int  nameLen = 0;
        nameBuf[0] = '\0';

        while (1)
        {
            while (*ptr == ' ') ptr++;
            if (*ptr == '\0') break;

            // peek ahead to classify this token
            char* peek = ptr;
            int isNum  = 1;
            int hasDot = 0;
            while (*peek != ' ' && *peek != '\0')
            {
                if (*peek == '.')
                {
                    hasDot = 1;
                }
                else if (*peek < '0' || *peek > '9')
                {
                    isNum = 0;
                    // don't break early — keep scanning so hasDot is set if needed
                }
                peek++;
            }
            if (isNum || hasDot) break;

            // append this token to name
            if (nameLen > 0) { *(nameBuf + nameLen) = ' '; nameLen++; }
            while (*ptr != ' ' && *ptr != '\0')
            {
                *(nameBuf + nameLen) = *ptr;
                nameLen++; ptr++;
            }
            *(nameBuf + nameLen) = '\0';
        }

        // allocate exactly strlen+1 bytes
        int len = 0;
        while (*(nameBuf + len) != '\0') len++;
        *(names + i) = new char[len + 1];
        for (int k = 0; k <= len; k++)
            *(*(names + i) + k) = *(nameBuf + k);

        // read gpa 
        t = 0;
        while (*ptr == ' ') ptr++;
        while (*ptr != ' ' && *ptr != '\0') { *(tok + t) = *ptr; t++; ptr++; }
        *(tok + t) = '\0';
        float sgpa = 0.0f;
        int dotPos = -1;
        for (int k = 0; *(tok + k) != '\0'; k++)
            if (*(tok + k) == '.') { dotPos = k; break; }
        if (dotPos == -1)
        {
            for (int k = 0; *(tok + k) != '\0'; k++)
                sgpa = sgpa * 10.0f + (*(tok + k) - '0');
        }
        else
        {
            for (int k = 0; k < dotPos; k++)
                sgpa = sgpa * 10.0f + (*(tok + k) - '0');
            float dec = 0.1f;
            for (int k = dotPos + 1; *(tok + k) != '\0'; k++)
            {
                sgpa = sgpa + (*(tok + k) - '0') * dec;
                dec  = dec * 0.1f;
            }
        }

        // read status 
        t = 0;
        while (*ptr == ' ') ptr++;
        while (*ptr != ' ' && *ptr != '\0') { *(tok + t) = *ptr; t++; ptr++; }
        *(tok + t) = '\0';
        int sstat = *(tok) - '0';

        // read dept 
        t = 0;
        while (*ptr == ' ') ptr++;
        while (*ptr != ' ' && *ptr != '\0') { *(tok + t) = *ptr; t++; ptr++; }
        *(tok + t) = '\0';
        int sd = 0;
        for (int k = 0; *(tok + k) != '\0'; k++)
            sd = sd * 10 + (*(tok + k) - '0');

        //read floor 
        t = 0;
        while (*ptr == ' ') ptr++;
        while (*ptr != ' ' && *ptr != '\0') { *(tok + t) = *ptr; t++; ptr++; }
        *(tok + t) = '\0';
        int sf = 0;
        for (int k = 0; *(tok + k) != '\0'; k++)
            sf = sf * 10 + (*(tok + k) - '0');

        //read room 
        t = 0;
        while (*ptr == ' ') ptr++;
        while (*ptr != ' ' && *ptr != '\0') { *(tok + t) = *ptr; t++; ptr++; }
        *(tok + t) = '\0';
        int sr = 0;
        for (int k = 0; *(tok + k) != '\0'; k++)
            sr = sr * 10 + (*(tok + k) - '0');

        // store into parallel arrays 
        *(ids      + i) = sid;
        *(gpas     + i) = sgpa;
        *(statuses + i) = sstat;

        // restore grid via pointer arithmetic
        if (sd >= 0 && sd < nd && sf >= 0 && sf < nf && sr >= 0 && sr < nr)
            *(*(*(grid + sd) + sf) + sr) = sid;
    }

    // restore remaining grid occupancy lines
    int d, f, r, occ;
    while (fin >> d >> f >> r >> occ)
        if (d >= 0 && d < nd && f >= 0 && f < nf && r >= 0 && r < nr)
            *(*(*(grid + d) + f) + r) = occ;

    fin.close();
    cout << "  [LOADED] " << numStudents << " student(s) from database.txt\n";
    return true;
}