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

    srand (time(NULL));
    ifstream iFile;

    random_device rd;
    mt19937 mt(rd());
    normal_distribution<double> distribution(0., 1.);

    vector<double> v0, v1;
    for (int i = 0; i < 1000; i++) {
        v0.push_back(distribution(mt));
        v1.push_back(distribution(mt));
    }

    // Liczenie sumy częściowej
    partial_sum(v0.begin(), v0.end(), v0.begin());
    partial_sum(v1.begin(), v1.end(), v1.begin());

    // Przygotowanie danych w formacie (x, y) dla gnuplot
    vector<pair<int,double>> data_v0, data_v1;
    for (size_t i = 0; i < v0.size(); ++i) {
        data_v0.push_back({static_cast<int>(i), v0[i]});
        data_v1.push_back({static_cast<int>(i), v1[i]});
    }

    // Wysyłanie do gnuplota
    gp << "set title 'Wykres v0 i v1'\n";
    gp << "plot '-' with lines title 'v0', '-' with lines title 'v1'\n";
    gp.send1d(data_v0);  // pierwsza seria
    gp.send1d(data_v1);  // druga seria

    cout << "Wykres wysłany do gnuplota. Naciśnij Enter, aby zakończyć..." << endl;
    cin.get();

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