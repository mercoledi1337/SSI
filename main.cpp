#include <iostream>
#include <fstream>
#include <time.h>
#include <random>

#include "gnuplot_iostream.h"
#include <numeric>
#include <random>

using namespace std;// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main()
{
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
        float group = readcount%4;
        location = line.find(',');
        first = line.substr(0, location);
        line = line.substr(location + 1, line.length());
        spiralka.push_back({stof(first), stof(line),group});

        readcount++;
    }

    string kolor = "green";
    // Wysyłanie do gnuplota
    gp << "set title 'Wykres spiralka'\n";
    gp << "plot '-' with points pt 7 ps 1.5 lc rgb '" + kolor + "' title 'dane'\n";
    gp.send1d(spiralka);  // pierwsza seria


    cout << "Wykres wysłany do gnuplota. Naciśnij Enter, aby zakończyć..." << endl;
    cin.get();

    const int M = 4;
    const int iters = 100;
    float V[4]; // mean





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