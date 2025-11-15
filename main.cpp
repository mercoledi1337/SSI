#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main()
{
    srand (time(NULL));
    ifstream iFile;

    const int M = 4;
    const int iters = 100;
    float V[4]; // mean
    const int MAX_SIZE = 101;
    string line = "", first = "", second = "";
    int location = 0, readcount = 0;

    iFile.open("spiralka.csv");

    float spiralka[MAX_SIZE][3];

    while (getline(iFile, line) && readcount < MAX_SIZE)
    {
        location = line.find(',');
        first = line.substr(0, location);
        line = line.substr(location + 1, line.length());
        spiralka[readcount][0] = stof(first);
        spiralka[readcount][1] = stof(line);
        spiralka[readcount][2] = 9;

        readcount++;
    }
    // choosing center of groups
    for (int i = 0; i < M; i++) {
        V[i] = rand() % 102;
        cout << V[i] << endl;
    }

    for (int s = 0; s < iters; s++)
    {

    }

    return 0;
}