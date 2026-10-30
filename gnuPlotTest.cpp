#include <cstdio>
#include <vector>
#include <utility> // for std::pair
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>

using namespace std;

int main()
{
    vector<array<float, 2>> samplePoints = {
        {0.0, 0.0},
        {1.0, 1.0},
        {2.0, 4.0},
        {3.0, 9.0},
        {4.0, 16.0},
        {5.0, 25.0}};

    vector<array<float, 2>> samplePoints2 = {
        {0.0, 0.0},
        {1.0, -1.0},
        {2.0, -4.0},
        {3.0, -9.0},
        {4.0, -16.0},
        {5.0, -25.0}};

    // First gnuplot instance: samplePoints as blue line
    FILE *gp = _popen("gnuplot -persistent", "w");
    if (!gp)
    {
        perror("popen");
        return 1;
    }

    fprintf(gp, "set title 'y = x^2'\n");
    fprintf(gp, "set xlabel 'X'\n");
    fprintf(gp, "set ylabel 'Y'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "plot '-' with lines lt rgb 'blue' lw 1 title 'y = x^2'\n");
    for (const auto &p : samplePoints)
    {
        fprintf(gp, "%f %f\n", p.at(0), p.at(1));
    }
    fprintf(gp, "e\n");
    fflush(gp);

    // Second gnuplot instance: samplePoints2 as red line and red points
    FILE *gp2 = _popen("gnuplot -persistent", "w");
    if (!gp2)
    {
        perror("popen");
        return 1;
    }

    fprintf(gp2, "set title 'y = -x^2'\n");
    fprintf(gp2, "set xlabel 'X'\n");
    fprintf(gp2, "set ylabel 'Y'\n");
    fprintf(gp2, "set grid\n");
    fprintf(gp2, "plot '-' with lines lt rgb 'red' lw 1 title 'y = -x^2', '-' with points lt rgb 'red' pt 7 ps 1.5 title 'y = -x^2 points'\n");

    // samplePoints2 as line
    for (const auto &p : samplePoints2)
    {
        fprintf(gp2, "%f %f\n", p.at(0), p.at(1));
    }
    fprintf(gp2, "e\n");

    // samplePoints2 as points
    for (const auto &p : samplePoints2)
    {
        fprintf(gp2, "%f %f\n", p.at(0), p.at(1));
    }
    fprintf(gp2, "e\n");
    fflush(gp2);

    _pclose(gp);
    _pclose(gp2);
    return 0;
}