#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <cmath>
#include "gnuplot_iostream.h"
#include <numeric>
#include <cstdlib>

const int Me = 3;
const int Mo = 4;
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

void odl(vector<vector<float>>& spiralka, float V[], int iters, int M)
{
    for (int s = 0; s < iters; s++)
    {
        float tmpShortest = abs(spiralka[s][0] - spiralka[V[0]][0]);
        for (int i = 0; i < M; i++)
        {
            int g = static_cast<int>(V[i]);
            float distance = abs(spiralka[s][0] - spiralka[g][0]);
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

void odl_mean(vector<vector<float>>& spiralka_po, vector<vector<float>> groups, int iters, int M)
{
    for (int s = 0; s < iters; s++)
    {
        float tmpShortest = abs(spiralka_po[s][0] - groups[0][0]);
        spiralka_po[s][2] = groups[0][2];
        for (int i = 0; i < M; i++)
        {
            float distance = abs(spiralka_po[s][0] - groups[i][0]);
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
    for (int i = 0; i < Me; i++)
    {
        int var = 0;
        for (int j = 0; j < 101; ++j) {
            var += count(spiralka_po[j].begin(), spiralka_po[j].end(), i);
        }

        if (groupSize.size() < Me) {
            groupSize.push_back(var);
        } else {
            groupSize[i] = var;
        }
    }
}

void countGroups_odl(vector<vector<float>>& spiralka_po, vector<int>& groupSize)
{
    for (int i = 0; i < Mo; i++)
    {
        int var = 0;
        for (int j = 0; j < 101; ++j) {
            var += count(spiralka_po[j].begin(), spiralka_po[j].end(), i);
        }

        if (groupSize.size() < Mo) {
            groupSize.push_back(var);
        } else {
            groupSize[i] = var;
        }
    }
}

void calcMean(vector<vector<float>> spiralka_po, vector<vector<float>>& groups, vector<int>& groupSize) {
    for (int i = 0; i < Me; i++) { // looping by groups
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
        if (groups.size() < Me)
            groups.push_back({meanX, meanY, g});
        else {
            groups[i][0] = meanX;
            groups[i][1] = meanY;
            groups[i][2] = g;
        }
    }
}

void calcMean_odl(vector<vector<float>> spiralka_po, vector<vector<float>>& groups, vector<int>& groupSize) {
    for (int i = 0; i < Mo; i++) { // looping by groups
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
        if (groups.size() < Mo)
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
    cout << maxX << " X max " << minX << " x min " << maxY << " Y max " << minY << " y min " << " dla grupy " << g << endl;
}

float* startArray(float V[]) {
    for (int i = 0; i < Me; i++) {
        V[i] = rand() % 102;
    }
    return V;
}

float* startArray_odl(float V[]) {
    for (int i = 0; i < Mo; i++) {
        V[i] = rand() % 102;
    }
    return V;
}

int main() {
    float V[Me]; //array for random start
    float V_odl[Mo]; //array for random start

    vector<int> groupSize; //size of groups for mean
    vector<int> groupSize_odl; //size of groups for mean

    vector<vector<float>> spiralka;
    vector<vector<float>> spiralka_odl;

    vector<vector<float>> groups;
    vector<vector<float>> groups_odl;

    string line = "", first = "", second = ""; // reading csv rariables
    int location = 0, readcount = 0;
    Gnuplot gp("D:/gnuplot/bin/gnuplot.exe");
    ifstream iFile("../spiralka.csv");
    srand (time(NULL)); //seting seed

    ofstream outFile("..\\wykresy.txt");
    cout.rdbuf(outFile.rdbuf());

    if (!iFile.is_open()) {
        cerr << "Nie można otworzyc pliku spiralka.csv" << endl;
        return 1;
    }

    while (getline(iFile, line) && readcount < MAX_SIZE)
    {
        float group = readcount%4; // temporary colour
        float group_odl = readcount%4; // temporary colour
        location = line.find(','); // spliting line with,
        first = line.substr(0, location);
        line = line.substr(location + 1, line.length());
        spiralka.push_back({stof(first), stof(line),group});
        spiralka_odl.push_back({stof(first), stof(line),group_odl});
        readcount++;
    }
    startArray(V); //choosing random groups
    startArray_odl(V_odl); //choosing random groups

    //euclides//
    euclides(spiralka, V, iters, Me);

    //coloring center;
    for (int i = 0; i < Me; i++) {
        spiralka[V[i]][2] = 5;
    }
    //euclides//

    //odl//
    odl(spiralka_odl, V_odl, iters, Mo);

    //coloring center;
    for (int i = 0; i < Mo; i++) {
        spiralka_odl[V_odl[i]][2] = 5;
    }
    //odl//

    auto spiralka_po = spiralka; // kopia danych
    auto spiralka_odl_po = spiralka_odl; // kopia danych

    // Wysyłanie startowego do gnuplota
    gp << "set term wxt 1\n";
    gp << "set title 'Wykres spiralka'\n";
    gp << "plot '-' using 1:2:3 with points pt 7 lc palette notitle\n";
    gp.send1d(spiralka);  // pierwsza seria
    //euclides//
    countGroups(spiralka_po, groupSize);
    calcMean(spiralka_po,groups,groupSize);
    //euclides//

    //odl//
    countGroups_odl(spiralka_odl_po, groupSize_odl);
    calcMean_odl(spiralka_odl_po,groups_odl,groupSize_odl);
    //odl//
    // Wysyłanie po iteracji do gnuplota

    //euclides//
    for (int j = 2; j < iteration + 1; j++) {
        euclidesForKMean(spiralka_po, groups, iters, Me);
        countGroups(spiralka_po, groupSize);
        calcMean(spiralka_po,groups,groupSize);
        //coloring centers;
        if (spiralka_po.size() == 101) {
            for (int i = 0; i < Me; i++) {
                groups[i][2] = 5;
                spiralka_po.push_back(groups[i]);
            }
        }
        else
        {
            for (int i = 0; i < Me; i++) {

                spiralka_po[101+i][0] = groups[i][0];
                spiralka_po[101+i][1] = groups[i][1];
                spiralka_po[101+i][2] = 5;
            }
        }
        //euclides//

            odl_mean(spiralka_odl_po, groups_odl, iters, Mo);
            countGroups_odl(spiralka_odl_po, groupSize_odl);
            calcMean_odl(spiralka_odl_po,groups_odl,groupSize_odl);
            //coloring centers;
            if (spiralka_odl_po.size() == 101) {
                for (int i = 0; i < Mo; i++) {
                    groups_odl[i][2] = 5;
                    spiralka_odl_po.push_back(groups_odl[i]);
                }
            }
            else
            {
                for (int i = 0; i < Mo; i++) {

                    spiralka_odl_po[101+i][0] = groups_odl[i][0];
                    spiralka_odl_po[101+i][1] = groups_odl[i][1];
                    spiralka_odl_po[101+i][2] = 5;
                }
            }

            //odl//

            if (j == 4 || j == 10) {
                cout << " ITERACJA "  << j << endl;
                for (int i = 0; i < Me; i++) {
                    showMinMax(spiralka_po,i);
                    cout << groups[i][0] << " " << groups[i][1] << " srodek grupy " << i << "\n" << endl;
                }
                gp << "set term pngcairo size 800,600 enhanced font 'Arial,12' background '#D3D3D3'\n";
                gp << "set output '..\\wykres" << j << ".png'\n"; // nazwa pliku wyjściowego
                gp << "set title 'Wykres spiralka " << j << " itetracjach'\n";
                gp << "plot '-' using 1:2:3 with points pt 7 lc palette notitle\n";
                gp.send1d(spiralka_po);  // pierwsza seria
            }
            if (j == 4 || j == 10) {
                cout << " ITERACJA "  << j << endl;
                for (int i = 0; i < Mo; i++) {
                    showMinMax(spiralka_odl_po,i);
                    cout << groups_odl[i][0] << " " << groups_odl[i][1] << " srodek grupy " << i << "\n" << endl;
                }
                gp << "set term pngcairo size 800,600 enhanced font 'Arial,12' background '#D3D3D3'\n";
                gp << "set output '..\\wykres_odl" << j << ".png'\n"; // nazwa pliku wyjściowego
                gp << "set title 'Wykres spiralka odl" << j << " itetracjach'\n";
                gp << "plot '-' using 1:2:3 with points pt 7 lc palette notitle\n";
                gp.send1d(spiralka_odl_po);  // pierwsza seria
            }
        }

        outFile.close();
        return 0;
}