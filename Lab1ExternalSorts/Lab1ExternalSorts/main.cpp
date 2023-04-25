#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <random>

using std::ifstream;
using std::ofstream;
using std::endl;
using std::cin;
using std::cout;
using std::string;

void StraightMerge(const std::string& inputFileName);
int MergeFiles(FILE& B, FILE& C, FILE& A);
void SplitFiles(const std::string& inputFileName, const std::string& outputFile1, const std::string& outputFile2);
bool CreateFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int maxNumberValue);
bool MergeFiles(const std::string& inputFile1, const std::string& inputFile2, const std::string& outputFile1, const std::string& outputFile2);


bool isFileContainsSortedArray(const string& fileName)
{

    ifstream file;
    file.open(fileName);

    if (!file.is_open())
    {
        cout << "File close" << endl;
    }
    else
    {
        cout << "File open!" << endl;
    }

    int antecedentNumber = -1;
    int currentNumber = -1;


    while (file >> currentNumber)
    {
        if (currentNumber < antecedentNumber)
        {
            return false;
        }

        antecedentNumber = currentNumber;
    }

    return true;
}

void StraightMerge(const string& inputFileName) {
    string fileNames[4] = {"/Users/vladashudegova/files/F1.txt", "/Users/vladashudegova/files/F2.txt", "/Users/vladashudegova/files/F3.txt", "/Users/vladashudegova/files/F4.txt" };


    SplitFiles(inputFileName, fileNames[0], fileNames[1]);

    while (!MergeFiles(fileNames[0], fileNames[1], fileNames[2], fileNames[3]))
    {
        string tmp;

        for (int i = 0; i < 2; i++)
        {
            tmp = fileNames[i + 2];
            fileNames[i + 2] = fileNames[i];
            fileNames[i] = tmp;
        }
    }


}


void SplitFiles(const string& inputFileName, const string& outputFile1, const string& outputFile2){

    ifstream inputFile;
    inputFile.open(inputFileName);

    if (!inputFile.is_open())
    {
        cout << "File close" << endl;
    }
    else
    {
        cout << "File open!" << endl;
    }

    ofstream outputFiles[2];
    outputFiles[0].open(outputFile1);
    outputFiles[1].open(outputFile2);

    int n = 0, currentEl, antecedentEl;
    inputFile >> antecedentEl;
    currentEl = antecedentEl;
    while (inputFile >> antecedentEl)
    {
        outputFiles[n] << antecedentEl << endl;

        if (antecedentEl > currentEl)
        {
            n = 1 - n;
        }

        antecedentEl = currentEl;
    }

    outputFiles[n] << antecedentEl << endl;


    inputFile.close();
    outputFiles[0].close();
    outputFiles[1].close();

}

bool MergeFiles(const string& inputFile1, const string& inputFile2, const string& outputFile1, const string& outputFile2)
{
    ifstream inputFiles[2];
    inputFiles[0].open(inputFile1);
    inputFiles[1].open(inputFile2);

    ofstream outputFiles[2];
    outputFiles[0].open(outputFile1);
    outputFiles[1].open(outputFile2);

    int m = 0;
    int n = 0;
    int currentEl[2], antecedentEl[2];
    inputFiles[0] >> antecedentEl[0];

    if (!(inputFiles[1] >> antecedentEl[1]))
    {
        return 1;
    }

    while (!inputFiles[0].eof() || !inputFiles[1].eof())
    {
        if (antecedentEl[m] > antecedentEl[1 - m] && !inputFiles[1 - m].eof())
        {
            m = 1 - m;
        }

        outputFiles[n] << antecedentEl[m] << endl;
        inputFiles[m] >> currentEl[m];

        if (inputFiles[m].eof() || antecedentEl[m] > currentEl[m])
        {
            if (!inputFiles[1 - m].eof())
            {
                m = 1 - m;
            }
            if (antecedentEl[m] > currentEl[m]) {
                outputFiles[n] << antecedentEl[m] << endl;
                inputFiles[m] >> currentEl[m];
            }

            while (!inputFiles[m].eof() && antecedentEl[m] <= currentEl[m])
            {
                antecedentEl[m] = currentEl[m];
                outputFiles[n] << antecedentEl[m] << endl;
                inputFiles[m] >> currentEl[m];
            }

            antecedentEl[1 - m] = currentEl[1 - m];

            n = 1 - n;
        }

        antecedentEl[m] = currentEl[m];
    }


    for (int i = 0; i < 2; i++)
    {
        inputFiles[i].close();
        outputFiles[i].close();
    }

    return 0;
}

bool CreateFileWithRandomNumbers(const string& fileName, const int numbersCount, const int maxNumberValue)
{
    ofstream fileOfNumber;
    fileOfNumber.open(fileName);

    if (!fileOfNumber.is_open())
    {
        cout << "File close" << endl;
    }
    else
    {
        cout << "File open!" << endl;
    }

    std::random_device randDev;
    std::mt19937 gen(randDev());
    std::uniform_int_distribution<> dis(0, maxNumberValue);

    for (int i = 0; i < numbersCount; ++i)
    {
        const int number = dis(gen);
        fileOfNumber << number << endl;
    }

    fileOfNumber.close();

    return true;
}

int CreateAndSortFile(const string& fileName, const int numbersCount, const int maxNumberValue)
{
    if (!CreateFileWithRandomNumbers(fileName, numbersCount, maxNumberValue))
    {
        return -1;
    }

    StraightMerge(fileName);

    if (!isFileContainsSortedArray(fileName))
    {
        return -2;
    }

    return 1;
}

int main() {
    
    string fileName = "/Users/vladashudegova/files/labFileSort.txt";
    const int numbersCount = 100;
    const int maxNumberValue = 1000000;


    CreateAndSortFile(fileName, numbersCount, maxNumberValue);

    cout << endl;
    
    return 0;
}
