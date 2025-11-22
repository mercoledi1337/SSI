#include <iostream>
#include <fstream>
#include <time.h>
#include <random>
#include <cmath>
#include "gnuplot_iostream.h"
#include <numeric>
#include <random>

using namespace std;// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main()
{
    srand (time(NULL));
    Gnuplot gp("D:/gnuplot/bin/gnuplot.exe");

    //reading csv part
    ifstream iFile("../spiralka.csv");
    if (!iFile.is_open()) {
        cerr << "Nie można otworzyć pliku spiralka.csv" << endl;
        return 1;
    }
    string line = "", first = "", second = "";
    int location = 0, readcount = 0;
    vector<vector<float>> spiralka;
    const int MAX_SIZE = 101;

    while (getline(iFile, line) && readcount < MAX_SIZE)
    {
        float group = readcount%4; // temporary colour
        location = line.find(','); // spliting line with,
        first = line.substr(0, location);
        line = line.substr(location + 1, line.length());
        spiralka.push_back({stof(first), stof(line),group});

        readcount++;
    }



    const int M = 4;
    const int iters = 101;
    float V[4]; // mean



    // choosing center of groups
    for (int i = 0; i < M; i++) {
        V[i] = rand() % 102;
        cout << V[i] << endl;
    }
    // euclides
    for (int s = 0; s < iters; s++)
    {
        float tmpShortest = sqrt(pow((spiralka[s][0] - spiralka[V[0]][0]),2) + pow((spiralka[s][1] - spiralka[V[0]][1]),2));
        spiralka[s][2] = V[0];
        for (int i = 0; i < M; i++) {
            int g = V[i];
            if (sqrt(pow((spiralka[s][0] - spiralka[g][0]),2) + pow((spiralka[s][1] - spiralka[g][1]),2)) < tmpShortest) {
                spiralka[s][2] = g;
            }
        }
    }

    // Wysyłanie do gnuplota
    gp << "set title 'Wykres spiralka'\n";
    gp << "plot '-' using 1:2:3 with points pt 7 lc palette notitle\n";
    gp.send1d(spiralka);  // pierwsza seria


    cout << "Wykres wysłany do gnuplota. Naciśnij Enter, aby zakończyć..." << endl;
    cin.get();

    return 0;
}