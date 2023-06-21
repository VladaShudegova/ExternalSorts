#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <math.h>

const std::string FILES_DIRECTORY_PATH = "./";
const int DESTINATION_FILES_COUNT = 2;
const int SOURCE_FILES_COUNT = 2;

void throwFileOpeningError(const std::string& fileName) {
    throw  "Can't open file \"" + fileName + "\"!";
}

void copyFileToOther(const std::string& sourceFileName, const std::string& destinationFileName) {
    std::ifstream source(sourceFileName);
    if (!source.is_open())
        throwFileOpeningError(sourceFileName);
    std::ofstream destination(destinationFileName);

    if (!destination.is_open())
        throwFileOpeningError(destinationFileName);

    int n;
    while (source >> n)
        destination << n << '\n';

    source.close();
    destination.close();
}

bool isFileContainsSortedArray(const std::string& fileName)
{
    std::ifstream file;
    file.open(fileName);

    if (!file.is_open())
    {
        throwFileOpeningError(fileName);
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

bool isEmptyFile(const std::string& fileName) {
    std::ifstream inputfile;
    inputfile.open(fileName);
    if (!inputfile.is_open())
    {
        throwFileOpeningError(fileName);
    }
    int value;
    if (inputfile >> value) {
        inputfile.close();
        return false;
    }
    else {
        inputfile.close();
        return true;
    }

}

bool createFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int maxNumberValue)
{
    std::ofstream fileOfNumber;
    fileOfNumber.open(fileName);

    if (!fileOfNumber.is_open())
    {
        throwFileOpeningError(fileName);
    }

    std::random_device randDev;
    std::mt19937 gen(randDev());
    std::uniform_int_distribution<> dis(0, maxNumberValue);

    for (int i = 0; i < numbersCount; ++i)
    {
        const int number = dis(gen);
        fileOfNumber << number << std::endl;
    }

    fileOfNumber.close();

    return true;
}

void split(const std::string& mainFileName, const std::vector<std::string>& destinationFileNames) {
    if(destinationFileNames.size() != DESTINATION_FILES_COUNT)
        throw "Count of destination files for split is not equal to allowed value";
    
    std::ifstream source(mainFileName);
    if (!source.is_open())
        throwFileOpeningError(mainFileName);

    std::vector<std::ofstream> destinations;
    for (int i = 0; i < DESTINATION_FILES_COUNT; i++) {
        destinations.push_back(std::ofstream(destinationFileNames[i]));
        if (!destinations[i].is_open())
            throwFileOpeningError(destinationFileNames[i]);
    }

    int destinationIndex = 0;
    int leftNumber;
    int rightNumber;
    source >> leftNumber;
    while (!source.eof()) {
        destinations[destinationIndex] << leftNumber << '\n';
        source >> rightNumber;

        while (!source.eof() && leftNumber < rightNumber) {
            leftNumber = rightNumber;
            destinations[destinationIndex] << leftNumber << '\n';
            source >> rightNumber;
        }

        leftNumber = rightNumber;
        destinationIndex = 1 - destinationIndex;
    }

    source.close();
    for (auto& destination : destinations)
        destination.close();
}

void merge(const std::vector<std::string>& sourceFileNames, const std::vector<std::string>& destinationFileNames) {
    if (sourceFileNames.size() != SOURCE_FILES_COUNT) 
        throw "Count of source files for split is not equal to allowed value.";
    if(destinationFileNames.size() != DESTINATION_FILES_COUNT)
        throw "Count of destination files for split is not equal to allowed value.";

    std::vector<std::ifstream> sources;
    std::vector<std::ofstream> destinations;
    
    for (int i = 0; i < DESTINATION_FILES_COUNT; i++) {
        sources.push_back(std::ifstream(sourceFileNames[i]));
        if (!sources[i].is_open())
            throwFileOpeningError(sourceFileNames[i]);
        
        destinations.push_back(std::ofstream(destinationFileNames[i]));
        if (!destinations[i].is_open())
            throwFileOpeningError(destinationFileNames[i]);
    }

    int sourceIndex = 0;
    int destinationIndex = 0;
    int leftNumbers[SOURCE_FILES_COUNT];
    int rightNumbers[SOURCE_FILES_COUNT];
    sources[sourceIndex] >> leftNumbers[sourceIndex];
    sources[1 - sourceIndex] >> leftNumbers[1 - sourceIndex];

    while (!sources[0].eof() && !sources[1].eof()) {
        if (leftNumbers[sourceIndex] > leftNumbers[1 - sourceIndex])
            sourceIndex = 1 - sourceIndex;
        destinations[destinationIndex] << leftNumbers[sourceIndex] << '\n';
        sources[sourceIndex] >> rightNumbers[sourceIndex];
        
        if (sources[sourceIndex].eof() or leftNumbers[sourceIndex] > rightNumbers[sourceIndex]) {
            sourceIndex = 1 - sourceIndex;
            destinations[destinationIndex] << leftNumbers[sourceIndex] << '\n';
            sources[sourceIndex] >> rightNumbers[sourceIndex];
            
            while (!sources[sourceIndex].eof() && leftNumbers[sourceIndex] <= rightNumbers[sourceIndex]) {
                leftNumbers[sourceIndex] = rightNumbers[sourceIndex];
                destinations[destinationIndex] << leftNumbers[sourceIndex] << '\n';
                sources[sourceIndex] >> rightNumbers[sourceIndex];
            }

            leftNumbers[1 - sourceIndex] = rightNumbers[1 - sourceIndex];
            destinationIndex = 1 - destinationIndex;
        }
        leftNumbers[sourceIndex] = rightNumbers[sourceIndex];
    }

    for (int i = 0; i < SOURCE_FILES_COUNT; i++) {
        while (!sources[i].eof()) {
            destinations[destinationIndex] << leftNumbers[i] << '\n';
            sources[i] >> rightNumbers[i];
            if (leftNumbers[i] > rightNumbers[i])
                destinationIndex = 1 - destinationIndex;
            leftNumbers[i] = rightNumbers[i];
        }
    }

    for (int i = 0; i < DESTINATION_FILES_COUNT; i++) {
        sources[i].close();
        destinations[i].close();
    }
}


void naturallySort(const std::string & fileName) {
    if (isEmptyFile(fileName))
        return;
    if (isFileContainsSortedArray(fileName))
        return;

    std::vector<std::string> ABFilesNames = { 
        FILES_DIRECTORY_PATH + "fileA.txt", 
        FILES_DIRECTORY_PATH + "fileB.txt" 
    };
    std::vector<std::string> CDFilesNames = { 
        FILES_DIRECTORY_PATH + "fileC.txt", 
        FILES_DIRECTORY_PATH + "fileD.txt" 
    };

    split(fileName, ABFilesNames);
    
    while (true) {
        if (isEmptyFile(ABFilesNames[1])) {
            copyFileToOther(ABFilesNames[0], fileName);
            return;
        }

        merge(ABFilesNames, CDFilesNames);

        if (isEmptyFile(CDFilesNames[1])) {
            copyFileToOther(CDFilesNames[0], fileName);
            return;
        }

        merge(CDFilesNames, ABFilesNames);
    }
}

int createAndSortFile(const std::string& fileName, const int numbersCount, const int maxNumberValue) {
    if (!createFileWithRandomNumbers(fileName, numbersCount, maxNumberValue))
        return -1;

    naturallySort(fileName);

    if (!isFileContainsSortedArray(fileName))
        return -2;

    return 1;
}

int main() {

    int numbersCount = 10;
    int maxNumberValue = 100;
    for (int i = 1; i < 6; i++) {
        int result;
        switch (createAndSortFile(FILES_DIRECTORY_PATH + "mainfile.txt", pow(numbersCount, i), 100 * pow(numbersCount, i)))
        {
        case -1:
            std::cout << "ERROR: TEST FAILED. ERROR IN CREATING FILE\n";

        case -2:
            std::cout << "INFO: TEST " << i << " FAILED. FILE DOES NOT CONTAIN SORTED ARRAY\n";

        case 1:
            std::cout << "INFO: TEST " << i << " PASSED\n";

        }
    }

    return 0;
}