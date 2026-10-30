#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include "iir1-master\Iir.h" // Include the IIR filter library

using namespace std;

// Class SignalProcessing
// This class contains static methods for signal processing tasks on ECG data
// It includes methods to replace missing values, replace spike artifacts, remove DC offset, and apply filters
class SignalProcessing
{
public:
    // Function "replaceMissingValues" to replace missing values in the data with 0
    // Parameter: inputData - a reference to a vector of arrays containing the data
    // Returns: a vector of arrays with missing values replaced by 0
    // Pre-condition: inputData is a vector of arrays, where each array contains two float values
    // Post-condition: the second column of each array in inputData has had missing values replaced by
    static void replaceMissingValues(vector<array<float, 2>> &inputData)
    {
        for (int i = 0; i < inputData.size(); i++)
        {
            if (inputData.at(i)[1] == -1) // Missing values are defined as -1
            {
                cout << "Found Missign Value at time " << inputData.at(i)[0] << " s." << endl;
                inputData.at(i)[1] = 0; // Task 2.1
            }
        }
    }

    // Function "replaceSpikeArtifacts" to replace spike artifacts in the data with 0
    // Parameter: inputData - a reference to a vector of arrays containing the data
    // Returns: a vector of arrays with spike artifacts replaced by 0
    // Pre-condition: inputData is a vector of arrays, where each array contains two float values
    // Post-condition: the second column of each array in inputData has had spike artifacts replaced by 0
    // Spike Artifacts are defined as measurement values outside of the physiological -0.5 to 3.0 mV limits
    static void replaceSpikeArtifacts(vector<array<float, 2>> &inputData)
    {
        for (int i = 0; i < inputData.size(); i++)
        {
            if ((inputData.at(i)[1] >= 3.0) || (inputData.at(i)[1] <= -0.5)) // Spike Artifacts are defined as measurementvalues outside of the physiological 0.5 to 3.0 mV limits
            {
                cout << "Found Spike Artifact at time " << inputData.at(i)[0] << " s." << endl;
                inputData.at(i)[1] = 0;
            }
        }
    }

    // Function "removeDC" to remove the DC component (mean) from the second column of data
    // Parameter: inputData - a reference to a vector of arrays containing the data
    // Pre-condition: inputData is a vector of arrays, where each array contains two float values
    // Post-condition: the second column of each array in inputData has had the mean subtracted from it
    static vector<array<float, 2>> removeDC(const vector<array<float, 2>> &inputData)
    {
        vector<array<float, 2>> modifiedData = inputData;
        // Calculate the mean of the second column (data values)
        float sum = 0.0;
        for (int i = 0; i < inputData.size(); i++)
        {
            sum += inputData.at(i)[1];
        }
        float mean = sum / inputData.size();

        // Subtract the mean from each value in the second column
        for (array<float, 2> &value : modifiedData)
        {
            value[1] -= mean;
        }

        return modifiedData;
    }

    // Function "lowPassFilter" to apply a lowpass filter to the second column of data
    // Parameter: inputData - a reference to a vector of arrays containing the data
    // Parameter: cutoffFrequency - the cutoff frequency for the lowpass filter
    // Parameter: sampleRate - the sample rate of the data
    // Output: a vector of arrays containing the filtered data
    // Pre-condition: inputData is a vector of arrays, where each array contains two float values
    // Post-condition: the second column of each array in inputData has been filtered using a lowpass filter
    static vector<array<float, 2>> lowPassFilter(const vector<array<float, 2>> &inputData, float cutoffFrequency, float sampleRate)
    {
        // Create a highpass filter with the specified cutoff frequency
        const int filterOrder = 4; // this cannot be passed as a parameter, because the IIR library does not support it
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
    static vector<array<float, 2>> highPassFilter(const vector<array<float, 2>> &inputData, float cutoffFrequency, float sampleRate)
    {
        // Create a lowpass filter with the specified cutoff frequency
        const int filterOrder = 4; // this cannot be passed as a parameter, because the IIR library does not support it
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

    static vector<array<float, 2>> estimatePeaks(vector<array<float, 2>> inputData)
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
};

// Class ECGData
// This class represents ECG data and provides methods to read, clean, filter, and plot the data
// It includes methods to calculate the mean heart rate, clean the data, apply filters, and
// plot the data using gnuplot
class ECGData
{
private:
    string m_filename;
    vector<array<float, 2>> m_data;
    float m_sampleRate;

public:
    // Parameterized constructor for ECGData class
    // Parameter: fileName - the name of the file to read
    // Parameter: sampleRate - the sample rate of the ECG data (default is 500.0 Hz)
    // Pre-condition: The file must exist and be readable
    // Post-condition: The ECG data is read from the file and stored in m_data, and the sample rate is set
    ECGData(string fileName, float sampleRate = 500.0)
    {
        readData(fileName);        // Load the data from the file
        m_sampleRate = sampleRate; // Set the sample rate
    }

    // Function "calculateMeanHeartRate" calculates the mean heart rate from the detected peaks
    // Parameter: peaks - a vector of arrays containing the detected peaks
    // Returns: the mean heart rate in beats per minute (bpm)
    // Pre-condition: peaks is a vector of arrays, where each array contains two float values (time and amplitude)
    // Post-condition: the function returns the mean heart rate in bpm
    // The mean heart rate is calculated as the number of peaks detected multiplied by 60 and divided by the total time of the ECG recording
    float calculateMeanHeartRate(const vector<array<float, 2>> &peaks)
    {
        float totalTime = m_data.back()[0] - m_data.front()[0]; // Calculate the total time of the ECG recording
        return peaks.size() * 60 / totalTime;                   // Heart rate in bpm
    }

    // Function "dataCleaning" cleans the ECG data by replacing missing values and spike artifacts
    // Output: the ECG data is cleaned to remove missing values and spike artifacts
    // Pre-condition: m_data is a vector of arrays containing the ECG data
    // Post-condition: m_data is modified to contain the cleaned ECG data
    // This function uses the SignalProcessing class to replace missing values and spike artifacts
    void dataCleaning()
    {
        SignalProcessing::replaceMissingValues(m_data);  // Replace missing values
        SignalProcessing::replaceSpikeArtifacts(m_data); // Replace spike artifacts
    }

    // Function "applyFilters" applies a series of filters to the ECG data
    // Parameters: lowCutoffFrequency - the cutoff frequency for the low-pass filter (default 0.5 Hz)
    //             highCutoffFrequency - the cutoff frequency for the high-pass filter (default 40 Hz)
    // Output: the ECG data is filtered to remove DC offset and apply low-pass and high-pass filters
    // Pre-condition: m_data is a vector of arrays containing the ECG data
    // Post-condition: m_data is modified to contain the filtered ECG data
    // This function uses the SignalProcessing class to remove DC offset, apply low-pass and high-pass filters
    void applyFilters(float lowCutoffFrequency = 0.5, float highCutoffFrequency = 40)
    {
        m_data = SignalProcessing::removeDC(m_data);                                          // Remove DC offset
        m_data = SignalProcessing::lowPassFilter(m_data, lowCutoffFrequency, m_sampleRate);   // Apply low-pass filter with#
        m_data = SignalProcessing::highPassFilter(m_data, highCutoffFrequency, m_sampleRate); // Apply high-pass filter with cutoff
    }

    // Function "plotData" plots the ECG data with detected peaks using gnuplot
    // Parameter: peaks - a vector of arrays containing the detected peaks
    // Output: the data is plotted using gnuplot
    // Pre-condition: gnuplot is installed and available in the system path
    // Post-condition: the data is plotted in a new window by use of gnuplot
    void plot(const vector<array<float, 2>> &peaks)
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
        for (const auto &value : m_data)
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

    // Function "plotData" plots the ECG data without peaks using gnuplot
    // Parameter: None
    // Output: the data is plotted using gnuplot
    // Pre-condition: gnuplot is installed and available in the system path
    // Post-condition: the data is plotted in a new window by use of gnuplot
    void plot()
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

        // Write the data to gnuplot
        for (const auto &value : m_data)
        {
            fprintf(gp, "%f %f\n", value[0], value[1]);
        }
        fprintf(gp, "e\n");

        fflush(gp);
        _pclose(gp);
    }

    // Function "getData" to return the ECG data
    // Returns: a vector of arrays containing the ECG data
    vector<array<float, 2>> getData()
    {
        return m_data; // Return the ECG data
    }

    // Function "readData" to read data from a file into a vector of arrays
    // Parameter: filename - the name of the file to read
    // Returns: void
    void readData(string filename)
    {

        ifstream inputFile(filename);

        if (!inputFile.is_open() || !inputFile.good())
        {
            throw std::invalid_argument("Could not open file: " + filename);
            return;
        }

        m_filename = filename; // Store the filename

        string line;
        // vector<array<float, 2>> data;

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
            istringstream iss(line);
            iss >> value1 >> value2;

            array<float, 2> valueLine = {value1, value2};
            m_data.push_back(valueLine);
        }
        // m_data = data; // Store the data in the member variable
    }
};

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

int main()
{
    vector<string> filenames = {"ECG_Data/ECG_data_ABCDE.txt",
                                "ECG_Data/ECG_data_AO9T.txt",
                                "ECG_Data/ECG_data_AX8T.txt",
                                "ECG_Data/ECG_data_BH93.txt",
                                "ECG_Data/ECG_data_CZ76.txt",
                                "ECG_Data/ECG_data_MXI5.txt",
                                "ECG_Data/ECG_data_YJ8U.txt"}; // List of filenames to process

    vector<ECGData> ecgDataList; // List to store ECGData objects

    for (string filename : filenames)
    {

        // Check if the file exists
        if (!FileExists(filename))
        {
            cout << "Error: File does not exist." << endl;
            return 1; // Exit if file does not exist
        }

        cout << "Processing file: " << filename << std::endl;
        ECGData ecgData(filename); // Create an ECGData object with the filename
        ecgData.dataCleaning();    // Clean the data (replace missing values and spike artifacts)
        ecgData.applyFilters();    // Filter the data (remove DC offset, apply low-pass and high-pass filters)

        vector<array<float, 2>> peaks = SignalProcessing::estimatePeaks(ecgData.getData());              // Estimate peaks in the ECG data
        float meanHeartRate = ecgData.calculateMeanHeartRate(peaks);                                     // Calculate the mean heart rate
        std::cout << "Mean heart rate for " << filename << ": " << meanHeartRate << " bpm" << std::endl; // Print the mean heart rate

        ecgData.plot(peaks); // Plot the ECG data and detected peaks
        // ecgData.plot(); // Plot the ECG data without peaks

        ecgDataList.push_back(ecgData); // Add the ECGData object to the list
    }

    return 0;
}