#include <iostream>
#include <fstream>
#include <time.h>
#include <random>
#include <cmath>
#include "gnuplot_iostream.h"
#include <numeric>

const int M = 4;
const int iters = 101;
const int MAX_SIZE = 101;

using namespace std;

void euclides(vector<vector<float>>& spiralka, float V[], int iters, int M)
{
    for (int s = 0; s < iters; s++)
    {
        float tmpShortest = sqrt(pow((spiralka[s][0] - spiralka[V[0]][0]),2) + pow((spiralka[s][1] - spiralka[V[0]][1]),2));
        for (int i = 0; i < M; i++)
        {
            int g = static_cast<int>(V[i]);
            float distance = sqrt(pow((spiralka[s][0] - spiralka[g][0]),2) + pow((spiralka[s][1] - spiralka[g][1]),2));
            if (distance <= tmpShortest)
            {
                tmpShortest = distance;
                spiralka[s][2] = i;
            }
        }
    }
}

void euclidesForKMean(vector<vector<float>>& spiralka_po, vector<vector<float>> groups, int iters, int M)
{
    for (int s = 0; s < iters; s++)
    {
        float tmpShortest = sqrt(pow((spiralka_po[s][0] - groups[0][0]),2) + pow((spiralka_po[s][1] - groups[0][1]),2));
        spiralka_po[s][2] = groups[0][2];
        for (int i = 0; i < M; i++)
        {
            float distance = sqrt(pow((spiralka_po[s][0] - groups[i][0]),2) + pow((spiralka_po[s][1] - groups[i][1]),2));
            if (distance <= tmpShortest)
            {
                tmpShortest = distance;
                spiralka_po[s][2] = groups[i][2];
            }
        }
    }
}


int main()
{
    float V[4]; // mean
    int groupSize[4];

    vector<vector<float>> spiralka;
    vector<vector<float>> groups;

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

    while (getline(iFile, line) && readcount < MAX_SIZE)
    {
        float group = readcount%4; // temporary colour
        location = line.find(','); // spliting line with,
        first = line.substr(0, location);
        line = line.substr(location + 1, line.length());
        spiralka.push_back({stof(first), stof(line),group});
        readcount++;
    }

    // choosing random center of groups
    for (int i = 0; i < M; i++) {
        V[i] = rand() % 102;
        cout << V[i] << endl;
    }

    euclides(spiralka, V, iters, M);

    //coloring center;
    for (int i = 0; i < M; i++) {
        spiralka[V[i]][2] = 5;
    }

    auto spiralka_po = spiralka; // kopia danych
    gp << "set multiplot layout 1,2 title 'K-Means'\n";
    // Wysyłanie do gnuplota
    gp << "set title 'Wykres spiralka'\n";
    gp << "plot '-' using 1:2:3 with points pt 7 lc palette notitle\n";
    gp.send1d(spiralka);  // pierwsza seria

    //find size of each group then loop inside
    for (int i = 0; i < M; i++)
    {
        int var = 0;
        for (auto& v: spiralka_po) {
             var += count(v.begin(), v.end(), i);
        }
        groupSize[i] = var;
    }

    //looping in groups
    for (int i = 0; i < M; i++) { // looping by groups
        float meanX = 0;
        float meanY = 0;
        if (groupSize[i] == 0)
            continue;
        for (int j = 0; j < iters; j++) { // looping whole vector
            if (spiralka_po[j][2] == i) { //if group in spiralka_po is valid add it to meanX/meanY
                meanX += spiralka_po[j][0];
                meanY += spiralka_po[j][1];
            }
        }
        meanX /= groupSize[i];
        meanY /= groupSize[i];
        float g = i;
        groups.push_back({meanX, meanY, g});
    }

    euclidesForKMean(spiralka_po, groups, iters, M);

    //coloring centers;
    for (int i = 0; i < M; i++) {
        groups[i][2] = 5;
        spiralka_po.push_back(groups[i]);
    }

    // Wysyłanie do gnuplota
    gp << "set title 'Wykres spiralka Po'\n";
    gp << "plot '-' using 1:2:3 with points pt 7 lc palette notitle\n";
    gp.send1d(spiralka_po);  // druga seria
    cout << "Wykres wysłany do gnuplota. Naciśnij Enter, aby zakończyć..." << endl;
    cin.get();

    return 0;
}