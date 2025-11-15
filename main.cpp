#include <iostream>
#include <fstream>

using namespace std;// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main()
{
    ifstream iFile;
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







    return 0;
}