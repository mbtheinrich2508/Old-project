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

class SignalProcessing
{
public:
    static void replaceMissingValues(std::vector<std::array<float, 2>> &inputData) {}

    static void replaceSpikeArtifacts(std::vector<std::array<float, 2>> &inputData) {}

    static std::vector<std::array<float, 2>> removeDC(const std::vector<std::array<float, 2>> &inputData)
    {
        vector<array<float, 2>> modifiedData = inputData;

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

    static std::vector<std::array<float, 2>> lowPassFilter(const std::vector<std::array<float, 2>> &inputData, float cutoffFrequency, float sampleRate)
    {
        vector<array<float, 2>> filteredData;
        return filteredData;
    }

    static std::vector<std::array<float, 2>> highPassFilter(const std::vector<std::array<float, 2>> &inputData, float cutoffFrequency, float sampleRate)
    {
        vector<array<float, 2>> filteredData;
        return filteredData;
    }

    static std::vector<std::array<float, 2>> estimatePeaks(std::vector<std::array<float, 2>> inputData)
    {
        vector<array<float, 2>> peaks;
        return peaks;
    }
};

class ECGData
{
private:
    std::string m_filename;
    std::vector<std::array<float, 2>> m_data;
    float m_sampleRate;

public:
    ECGData(string fileName, float sampleRate = 500.0)
    {
        readData(fileName);        // Load the data from the file
        m_sampleRate = sampleRate; // Set the sample rate
    }

    float calculateMeanHeartRate(const std::vector<std::array<float, 2>> &peaks)
    {
        float totalTime = m_data.back()[0] - m_data.front()[0]; // Calculate the total time of the ECG recording
        return peaks.size() * 60 / totalTime;                   // Heart rate in bpm
    }

    void dataCleaning() {} // Clean the ECG data by replacing missing values and spike artifacts

    void applyFilters(float lowCutoffFrequency = 0.5, float highCutoffFrequency = 40); // Apply a series of filters to the ECG data

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

    void plot(const std::vector<std::array<float, 2>> &peaks) {} // plot including peak

    std::vector<std::array<float, 2>> getData()
    {
        return m_data;
    } // Get the ECG data

    void readData(std::string filename)
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

// global
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
        ecgData.plot();            // Plot the ECG data
    }
}
