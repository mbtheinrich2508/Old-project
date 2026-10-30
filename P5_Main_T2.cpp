

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>

// Define a constant PRINTALL
#define PRINTALL -1

using namespace std;

// Function "plotData" plots one vector given by the parameter  data using gnuplot
// Parameter: data - a vector of arrays containing the data to plot
// Output: the data is plotted using gnuplot
// Pre-condition: gnuplot is installed and available in the system path
// Post-condition: the data is plotted in a new window by use of gnuplot
void plotData(const vector<array<float, 2>> &data)
{
    // Open a pipe to gnuplot
    FILE *gp = _popen("gnuplot -persistent", "w");
    if (!gp)
    {
        perror("popen");
        return;
    }

    // Set up gnuplot
    fprintf(gp, "set title 'ECG Data'\n");
    fprintf(gp, "set xlabel 'Time (s)'\n");
    fprintf(gp, "set ylabel 'Amplitude'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "plot '-' with lines lt rgb 'blue' lw 1 title 'ECG Data'\n");

    // Write the values of data to gnuplot
    for (const auto &value : data)
    {
        fprintf(gp, "%f %f\n", value[0], value[1]);
    }
    fprintf(gp, "e\n");

    fflush(gp);
    _pclose(gp);
}

// Function "printVector" to print a vector of arrays
// Parameter: inputData - the vector of arrays to print
// Parameter: maxPrintLines - maximum number of lines to print, default is PRINTALL
void printVector(vector<std::array<float, 2>> inputData, int maxPrintLines = PRINTALL)
{
    // If maxPrintLines is PRINTALL or greater than the size of inputData, set it to the size of inputData
    if ((maxPrintLines == PRINTALL) || (maxPrintLines > inputData.size()))
        maxPrintLines = inputData.size();

    for (int i = 0; i < maxPrintLines; i++)
        cout << i << ": " << inputData[i][0] << " " << inputData[i][1] << std::endl;
}

// Function "readData" to read data from a file into a vector of arrays
// Parameter: filename - the name of the file to read
// Returns: a vector of arrays containing the data read from the file
vector<array<float, 2>> readData(string filename)
{
    ifstream inputFile(filename);
    string line;
    vector<array<float, 2>> data;

    int skipLine = 4;

    while (getline(inputFile, line))
    {
        // Skip the first 4 lines
        if (skipLine > 0)
        {
            skipLine--;
            continue;
        }

        float value1, value2;
        // Use istringstream to parse the line into two float values
        // This assumes that each line contains two float values separated by whitespace
        istringstream iss(line);
        iss >> value1 >> value2;

        // Create an array from the two float values and add it to the vector valueline
        array<float, 2> valueLine = {value1, value2};
        data.push_back(valueLine);
    }
    return data;
}

// Function "FileExists" to check if a file exists
// Parameter: filename - the name of the file to check
// Returns: true if the file exists, false otherwise
bool FileExists(const string &filename)
{
    ifstream file(filename);

    // Returns true if the file is open and good for reading
    // Returns false if the file does not exist or cannot be opened
    // Note: This does not check if the file is empty, only if it can be opened
    return file.is_open() && file.good();
}

// Task 2: Estimate peaks in the ECG data
vector<array<float, 2>> estimatePeaks(vector<array<float, 2>> inputData)
{
    vector<array<float, 2>> peaks;
    for (int i = 1; i < inputData.size() - 1; i++)
    {
        if (inputData.at(i)[1] > inputData.at(i - 1)[1] && inputData.at(i)[1] > inputData.at(i + 1)[1])
        {
            if (inputData.at(i)[1] > 0.5)
            {
                array<float, 2> peak = {inputData.at(i)[0], inputData.at(i)[1]};
                peaks.push_back(peak);
            }
        }
    }
    return peaks;
}

// Task 2: Calculate mean heart rate from the detected peaks
float calculateMeanHeartRate(const vector<array<float, 2>> &peaks, float totalTime)
{

    return peaks.size() * 60 / totalTime; // Heart rate in bpm
}

int main()
{
    string filename = "ECG_data.txt"; // Specify the filename

    // Check if the file exists
    if (!FileExists(filename))
    {
        cout << "File can't be opened: " << filename << endl;
        return 1; // Exit if file does not exist
    }

    // Read data from the file into a vector of arrays
    vector<array<float, 2>> data = readData(filename);

    // Print out the first 50 entries for testing purposes
    printVector(data, 50);

    // For testing purposes also, works only if gnuplot is installed and available in the system path
    plotData(data);

    // Task 2: Estimate peaks and mean heart rate
    vector<array<float, 2>> peaks = estimatePeaks(data);           // Estimate peaks in the ECG data
    float duration = data.back()[0] - data.front()[0];             // Calculate the duration of the ECG recording
    float meanHeartRate = calculateMeanHeartRate(peaks, duration); // Calculate the mean heart rate

    std::cout << "Mean heart rate: " << meanHeartRate << " bpm" << std::endl;
    return 0;
}
