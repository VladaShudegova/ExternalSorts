#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <vector>
#include <random>

using std::ifstream;
using std::ofstream;
using std::endl;
using std::cin;
using std::cerr;
using std::cout;
using std::string;
using std::vector;

const string path = "/Users/vladashudegova/dev/repos/ExternalSorts/ExternalSorts/files/";

bool isFileContainsSortedArray(const string& fileName)
{

    ifstream file;
    file.open(fileName);

    if (!file.is_open())
    {
        cerr << "Can't open file \"" << fileName << "\"!" << endl;
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




void splitFiles(const string& inputFileName, const string& outputFile1, const string& outputFile2){

    ifstream inputFile;
    inputFile.open(inputFileName);

    if (!inputFile.is_open())
    {
        cerr << "Can't open file \"" << inputFileName << "\"!" << endl;
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

bool mergeFiles(const string& inputFile1, const string& inputFile2, const string& outputFile1, const string& outputFile2)
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
        return true;
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

    return false;
}

bool isEmpty(const string& fileName){
    ifstream inputfile;
    inputfile.open(fileName);
    if (!inputfile.is_open())
    {
        cerr << "Can't open file \"" << fileName << "\"!" << endl;
    }
    int value;
    if(inputfile >> value){
        inputfile.close();
        return false;
    }
    else{
        inputfile.close();
        return true;
    }
    
}

string sortedArray(const vector<string> &fileNames){
    for(int i = 0; i < fileNames.size(); i++){
        if(isEmpty(fileNames[i])){
            i++;
        }else{
            return fileNames[i];
        }
        //isEmpty(fileNames[i]) ? i++ : return fileNames[i];
        
        
    }
    return "errorFile";
}

string straightMerge(const string& inputFileName) {
    vector<string> FileNames = { path+"labF1.txt", path+"labF2.txt", path+"labF3.txt", path+"labF4.txt" };

  
        splitFiles(inputFileName, FileNames[0], FileNames[1]);

        while (!mergeFiles(FileNames[0], FileNames[1], FileNames[2], FileNames[3]))
        {
            string tmp;

            for (int i = 0; i < 2; i++)
            {
                tmp = FileNames[i + 2];
                FileNames[i + 2] = FileNames[i];
                FileNames[i] = tmp;
            }
        }
    return sortedArray(FileNames);
   
}

bool createFileWithRandomNumbers(const string& fileName, const int numbersCount, const int maxNumberValue)
{
    ofstream fileOfNumber;
    fileOfNumber.open(fileName);

    if (!fileOfNumber.is_open())
    {
        cerr << "Can't open file \"" << fileName << "\"!" << endl;
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

int createAndSortFile(const string& fileName, const int numbersCount, const int maxNumberValue)
{
    if (!createFileWithRandomNumbers(fileName, numbersCount, maxNumberValue))
    {
        return -1;
    }

    string checkSort = straightMerge(fileName);

    cout<<checkSort<<endl;
    if (!isFileContainsSortedArray(checkSort))
    {
        return -2;
    }

    return 1;
}

int main() {
    
    string fileName = path+"fileSort.txt";
//    string fileName = "/Users/vladashudegova/dev/repos/ExternalSorts/ExternalSorts/files/file.txt";
       const int numbersCount = 100;
       const int maxNumberValue = 100000;

       for (int i = 0; i < 10; i++) {
           switch (createAndSortFile(fileName, numbersCount, maxNumberValue)) {
               case 1:
                   std::cout << "Test passed." << std::endl;
               break;

               case -1:
                   std::cout << "Test failed: can't create file." << std::endl;
               break;

               case -2:
                   std::cout << "Test failed: file isn't sorted." << std::endl;
               break;
           }
       }
    
    return 0;
}
