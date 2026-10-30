#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include "iir1-master\Iir.h" // Include the IIR filter library

using namespace std;

// Define a constant PRINTALL
#define PRINTALL -1

// Function "replaceMissingValues" to replace missing values in the data with 0
// Parameter: inputData - a reference to a vector of arrays containing the data
// Returns: a vector of arrays with missing values replaced by 0
// Pre-condition: inputData is a vector of arrays, where each array contains two float values
// Post-condition: the second column of each array in inputData has had missing values replaced by
vector<array<float, 2>> replaceMissingValues(vector<array<float, 2>> inputData)
{
    for (int i = 0; i < inputData.size(); i++)
    {
        if (inputData.at(i)[1] == -1) // Missing values are defined as -1
        {
            cout << "Found Missign Value at time " << inputData.at(i)[0] << " s." << endl;
            inputData.at(i)[1] = 0; // Task 2.1
        }
    }

    return inputData;
}

// Function "replaceSpikeArtifacts" to replace spike artifacts in the data with 0
// Parameter: inputData - a reference to a vector of arrays containing the data
// Returns: a vector of arrays with spike artifacts replaced by 0
// Pre-condition: inputData is a vector of arrays, where each array contains two float values
// Post-condition: the second column of each array in inputData has had spike artifacts replaced by 0
// Spike Artifacts are defined as measurement values outside of the physiological -0.5 to 3.0 mV limits
vector<array<float, 2>> replaceSpikeArtifacts(vector<array<float, 2>> inputData)
{
    for (int i = 0; i < inputData.size(); i++)
    {
        if ((inputData.at(i)[1] >= 3.0) || (inputData.at(i)[1] <= -0.5)) // Spike Artifacts are defined as measurementvalues outside of the physiological 0.5 to 3.0 mV limits
        {
            cout << "Found Spike Artifact at time " << inputData.at(i)[0] << " s." << endl;
            inputData.at(i)[1] = 0;
        }
    }

    return inputData;
}

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

// Function "estimatePeaks" to estimate peaks in the second column of data
// Parameter: inputData - a reference to a vector of arrays containing the data
// Returns: a vector of arrays containing the estimated peaks
// Pre-condition: inputData is a vector of arrays, where each array contains two float values
// Post-condition: the function returns a vector of arrays containing the estimated peaks
// The peaks are defined as local maxima in the second column that are greater than 0.5
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

// Function "calculateMeanHeartRate" calculates the mean heart rate from the detected peaks
// Parameter: peaks - a vector of arrays containing the detected peaks
// Parameter: totalTime - the total time of the ECG recording in seconds
// Returns: the mean heart rate in beats per minute (bpm)
// Pre-condition: peaks is a vector of arrays, where each array contains two float values (time and amplitude)
// Post-condition: the function returns the mean heart rate in bpm
// The mean heart rate is calculated as the number of peaks detected multiplied by 60 and divided by the total time of the ECG recording
float calculateMeanHeartRate(const vector<array<float, 2>> &peaks, float totalTime)
{

    return peaks.size() * 60 / totalTime; // Heart rate in bpm
}

// Function "plotData" plots one vector given by the parameter  data using gnuplot
// Parameter: data - a vector of arrays containing the data to plot
// Output: the data is plotted using gnuplot
// Pre-condition: gnuplot is installed and available in the system path
// Post-condition: the data is plotted in a new window by use of gnuplot
void plotData(const vector<array<float, 2>> &data, const vector<array<float, 2>> &peaks)
{
    // Open a pipe to gnuplot
    FILE *gp = _popen("gnuplot -persistent", "w");
    if (!gp)
    {
        perror("popen");
        return;
    }

    // Set up gnuplot
    fprintf(gp, "set title 'ECG Data with Detected Peaks'\n");
    fprintf(gp, "set xlabel 'Time (s)'\n");
    fprintf(gp, "set ylabel 'Amplitude'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "plot '-' with lines lt rgb 'blue' lw 1 title 'ECG Data', '-' with points pt 7 ps 1.5 lt rgb 'red' title 'Peaks'\n");

    // Write the data to gnuplot
    for (const auto &value : data)
    {
        fprintf(gp, "%f %f\n", value[0], value[1]);
    }
    fprintf(gp, "e\n");

    // Write the peaks to gnuplot
    for (const auto &peak : peaks)
    {
        fprintf(gp, "%f %f\n", peak[0], peak[1]);
    }
    fprintf(gp, "e\n");

    fflush(gp);
    _pclose(gp);
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

// Main function
int main()
{
    vector<string> filenames = {"ECG_Data/ECG_data_ABCDE.txt",
                                "ECG_Data/ECG_data_AO9T.txt",
                                "ECG_Data/ECG_data_AX8T.txt",
                                "ECG_Data/ECG_data_BH93.txt",
                                "ECG_Data/ECG_data_CZ76.txt",
                                "ECG_Data/ECG_data_MXI5.txt",
                                "ECG_Data/ECG_data_YJ8U.txt"}; // List of filenames to process
    vector<array<float, 2>> data;
    vector<array<float, 2>> peaks;
    for (const auto &filename : filenames)
    {
        if (!FileExists(filename))
        {
            cerr << "File " << filename << " does not exist." << endl;
            continue; // Skip to the next file if it does not exist
        }

        cout << "Processing file: " << filename << endl;

        // Read data from the file
        data = readData(filename);

        // Replace missing values and spike artifacts
        data = replaceMissingValues(data);
        data = replaceSpikeArtifacts(data);

        // Remove DC component
        removeDC(data);

        float lowCutoffFrequency = 0.5;   // low cutoff frequency in Hz
        float highCutoffFrequency = 40.0; // high cutoff frequency in Hz
        float sampleRate = 500;           // Example sample rate in Hz
        // Apply lowpass filter
        data = lowPassFilter(data, lowCutoffFrequency, sampleRate);

        // Apply highpass filter
        data = highPassFilter(data, highCutoffFrequency, sampleRate);

        // Print the processed data
        printVector(data, 10); // Print first 10 lines of processed data

        // Estimate peaks in the data
        peaks = estimatePeaks(data);

        // Calculate mean heart rate
        float totalTime = data.back()[0]; // Assuming the last time value is the total time
        float meanHeartRate = calculateMeanHeartRate(peaks, totalTime);
        cout << "Mean Heart Rate: " << meanHeartRate << " bpm" << endl;

        // Plot the data and detected peaks using gnuplot
        plotData(data, peaks);

        cout << "Finished processing file: " << filename << endl;
        data.clear();  // Clear data for the next file
        peaks.clear(); // Clear peaks for the next file
    }

    return 0;
}