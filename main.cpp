#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <cmath>
#include "gnuplot_iostream.h"
#include <numeric>
#include <cstdlib>

const int M = 3;
const int iters = 101;
const int MAX_SIZE = 101;
const int iteration = 10;

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
                spiralka_po[s][2] = i;
            }
        }
    }
}

void countGroups(vector<vector<float>>& spiralka_po, vector<int>& groupSize)
{
    for (int i = 0; i < M; i++)
    {
        int var = 0;
        for (int j = 0; j < 101; ++j) {
            var += count(spiralka_po[j].begin(), spiralka_po[j].end(), i);
        }

        if (groupSize.size() < M) {
            groupSize.push_back(var);
        } else {
            groupSize[i] = var;
        }
    }
}

void calcMean(vector<vector<float>> spiralka_po, vector<vector<float>>& groups, vector<int>& groupSize) {
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
        if (groups.size() < M)
            groups.push_back({meanX, meanY, g});
        else {
            groups[i][0] = meanX;
            groups[i][1] = meanY;
            groups[i][2] = g;
        }
    }
}

void showMinMax(vector<vector<float>> spiralka_po, float g) {
    float maxX = -10;
    float minX = 10;
    float maxY = -10;
    float minY = 10;
    for (int i = 0; i < spiralka_po.size(); i++) {
        if (spiralka_po[i][2] == g) {
            if (spiralka_po[i][0] > maxX) {
                maxX = spiralka_po[i][0];
            }
            if (spiralka_po[i][1] > maxY) {
                maxY = spiralka_po[i][1];
            }
            if (spiralka_po[i][0] < minX) {
                minX = spiralka_po[i][0];
            }
            if (spiralka_po[i][1] < minY) {
                minY = spiralka_po[i][1];
            }
        }
    }
    cout << maxX << " X " << minX << " x " << maxY << " Y " << minY << " y " << " dla grupy " << g << endl;
}

float* startArray(float V[]) {
    for (int i = 0; i < M; i++) {
        V[i] = rand() % 102;
        cout << V[i] << endl;
    }
    return V;
}

int main()
{
    float V[M]; //array for random start
    vector<int> groupSize; //size of groups for mean
    vector<vector<float>> spiralka;
    vector<vector<float>> groups;
    string line = "", first = "", second = ""; // reading csv rariables
    int location = 0, readcount = 0;
    Gnuplot gp("D:/gnuplot/bin/gnuplot.exe");
    ifstream iFile("../spiralka.csv");
    srand (time(NULL)); //seting seed



    if (!iFile.is_open()) {
        cerr << "Nie można otworzyc pliku spiralka.csv" << endl;
        return 1;
    }

    while (getline(iFile, line) && readcount < MAX_SIZE)
    {
        float group = readcount%4; // temporary colour
        location = line.find(','); // spliting line with,
        first = line.substr(0, location);
        line = line.substr(location + 1, line.length());
        spiralka.push_back({stof(first), stof(line),group});
        readcount++;
    }

    startArray(V); //choosing random groups

    euclides(spiralka, V, iters, M);

    //coloring center;
    for (int i = 0; i < M; i++) {
        spiralka[V[i]][2] = 5;
    }

    auto spiralka_po = spiralka; // kopia danych

    // Wysyłanie startowego do gnuplota
    gp << "set term wxt 1\n";
    gp << "set title 'Wykres spiralka'\n";
    gp << "plot '-' using 1:2:3 with points pt 7 lc palette notitle\n";
    gp.send1d(spiralka);  // pierwsza seria
    cout << "Wykres wyslany do gnuplota. Nacisnij Enter, aby zakonczyc..." << endl;
    cin.get();


    // Wysyłanie po iteracji do gnuplota
    for (int j = 2; j < iteration + 1; j++) {
        countGroups(spiralka_po, groupSize);
        calcMean(spiralka_po,groups,groupSize);
        euclidesForKMean(spiralka_po, groups, iters, M);

        //coloring centers;
        if (spiralka_po.size() == 101) {
            for (int i = 0; i < M; i++) {
                groups[i][2] = 5;
                spiralka_po.push_back(groups[i]);
            }
        } else {
            for (int i = 0; i < M; i++) {

                spiralka_po[101+i][0] = groups[i][0];
                spiralka_po[101+i][1] = groups[i][1];
                spiralka_po[101+i][2] = 5;
            }
        }

        if (j == 4 || j == 10) {
            for (int i = 0; i < M; i++) {
                showMinMax(spiralka_po,i);
                cout << groups[i][0] << " " << groups[i][1] << " srodek grupy " << i << "\n" << endl;
            }

            gp << "set term pngcairo size 800,600 enhanced font 'Arial,12' background '#D3D3D3'\n";
            gp << "set output 'D:\\SSI\\wykres" << j << ".png'\n"; // nazwa pliku wyjściowego
            gp << "set title 'Wykres spiralka " << j << " itetracjach'\n";
            gp << "plot '-' using 1:2:3 with points pt 7 lc palette notitle\n";
            gp.send1d(spiralka_po);  // pierwsza seria
            cout << "Wykres wyslany do gnuplota. Nacisnij Enter, aby zakonczyc..." << endl;

            cin.get();
        }
        int ret = system("python D:\\pythonProject\\main.py");
    }
    return 0;
}