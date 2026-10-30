#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include "iir1-master/Iir.h" // Include the IIR filter library

// Define a constant PRINTALL
#define PRINTALL -1

using namespace std;

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

// Task 1
// Function "removeDC" to remove the DC component (mean) from the second column of data
// Parameter: inputData - a reference to a vector of arrays containing the data
// Pre-condition: inputData is a vector of arrays, where each array contains two float values
// Post-condition: the second column of each array in inputData has had the mean subtracted from it
void removeDC(vector<array<float, 2>> &inputData)
{
    // Calculate the mean of the second column (data values)
    float sum = 0.0;
    for (int i = 0; i < inputData.size(); i++)
    {
        sum += inputData.at(i)[1];
    }
    float mean = sum / inputData.size();

    // Subtract the mean from each value in the second column
    for (array<float, 2> &value : inputData)
    {
        value[1] -= mean;
    }
}

// Task 2B: Lowpass
// Function "lowPassFilter" to apply a lowpass filter to the second column of data
// Parameter: inputData - a reference to a vector of arrays containing the data
// Parameter: cutoffFrequency - the cutoff frequency for the lowpass filter
// Parameter: sampleRate - the sample rate of the data
// Output: a vector of arrays containing the filtered data
// Pre-condition: inputData is a vector of arrays, where each array contains two float values
// Post-condition: the second column of each array in inputData has been filtered using a lowpass filter
vector<array<float, 2>> lowPassFilter(const vector<array<float, 2>> &inputData, float cutoffFrequency, float sampleRate)
{
    // Create a highpass filter with the specified cutoff frequency
    const int filterOrder = 4;
    Iir::Butterworth::HighPass<filterOrder> lowpass;

    lowpass.setup(sampleRate, cutoffFrequency);

    vector<array<float, 2>> filteredData;

    for (const auto &value : inputData)
    {
        // Apply the highpass filter to the second column (data values)
        float filteredValue = lowpass.filter(value[1]);
        filteredData.push_back({value[0], filteredValue});
    }

    return filteredData;
}

// Task 2A: Highpass
// Function "highPassFilter" to apply a highpass filter to the second column of data
// Parameter: inputData - a reference to a vector of arrays containing the data
// Parameter: cutoffFrequency - the cutoff frequency for the highpass filter
// Parameter: sampleRate - the sample rate of the data
// Output: a vector of arrays containing the filtered data
// Pre-condition: inputData is a vector of arrays, where each array contains two float values
// Post-condition: the second column of each array in inputData has been filtered using a highpass filter
vector<array<float, 2>> highPassFilter(const vector<array<float, 2>> &inputData, float cutoffFrequency, float sampleRate)
{
    // Create a lowpass filter with the specified cutoff frequency
    const int filterOrder = 4;
    Iir::Butterworth::LowPass<filterOrder> lowpass;

    lowpass.setup(sampleRate, cutoffFrequency);

    vector<array<float, 2>> filteredData;

    for (const auto &value : inputData)
    {
        // Apply the lowpass filter to the second column (data values)
        float filteredValue = lowpass.filter(value[1]);
        filteredData.push_back({value[0], filteredValue});
    }

    return filteredData;
}

// Task 2: Plotting the data
// Function "plotData" to plot the data using gnuplot
// Parameter: data - a vector of arrays containing the data to plot
// Parameter: extraTitle - an optional title to add to the plot, the stundents don't need to set this parameter, but it is a good practice to do so
// Output: the data is plotted using gnuplot
// Pre-condition: gnuplot is installed and available in the system path
// Post-condition: the data is plotted in a new window by use of gnuplot
void plotData(const vector<array<float, 2>> &data, string extraTitle = "")
{
    // Open a pipe to gnuplot
    FILE *gp = _popen("gnuplot -persistent", "w");
    if (!gp)
    {
        perror("popen");
        return;
    }

    // Set up gnuplot
    fprintf(gp, "set title '%s ECG Data'\n", extraTitle.c_str());
    // the students don't need to set the extraTitle like we do in the example above by use of the parameter extraTitle,
    // but it is a good practice to do so, because it allows to add a title to the plot
    // so it is OK, when students just do it like this:
    // fprintf(gp, "set title 'Filtered ECG Data'\n");
    fprintf(gp, "set xlabel 'Time (s)'\n");
    fprintf(gp, "set ylabel 'Amplitude'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "plot '-' with lines lt rgb 'blue' lw 1 title 'ECG Data'\n");

    // Write the data to gnuplot
    for (const auto &value : data)
    {
        fprintf(gp, "%f %f\n", value[0], value[1]);
    }
    fprintf(gp, "e\n");

    fflush(gp);
    _pclose(gp);
}

int main()
{
    string filename = "ECG_data_Noise.txt"; // Specify the filename

    // Check if the file exists
    if (!FileExists(filename))
    {
        cout << "File can't be opened: " << filename << endl;
        return 1; // Exit if file does not exist
    }

    // Read data from the file into a vector of arrays
    vector<array<float, 2>> data = readData(filename);

    // plot the raw data first, task 3
    plotData(data, "Raw Data");

    // Remove DC component
    removeDC(data);

    // just for testing, not necessary  for  the lab
    plotData(data, " Filtered Data After RemoveDC");

    // Task 1, apply filters
    float lowCutoffFrequency = 1; // Cutoff frequency in Hz
    float sampleRate = 500.0;     // Sample rate in Hz
    vector<array<float, 2>> filteredData = lowPassFilter(data, lowCutoffFrequency, sampleRate);

    float highCutoffFrequency = 50.0; // Cutoff frequency in Hz
    vector<array<float, 2>> finalFilteredData = highPassFilter(filteredData, highCutoffFrequency, sampleRate);

    // plot the filtered data, task 3
    plotData(finalFilteredData, " Final Filtered Data");

    return 0;
}
