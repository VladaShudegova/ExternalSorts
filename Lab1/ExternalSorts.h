#ifndef LAB1_EXTERNALSORTS_H
#define LAB1_EXTERNALSORTS_H
#include <fstream>

class ExternalSorts {

    void StraightMerge(const std::string& inputFileName);
    void SplitFiles(const std::string& inputFileName, const std::string& outputFile1, const std::string& outputFile2);
    bool CreateFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int maxNumberValue);
    bool MergeFiles(const std::string& inputFile1, const std::string& inputFile2, const std::string& outputFile1, const std::string& outputFile2);


};

bool CreateFileWithRandomNumbers(const std::string& fileName, const int numbersCount, const int maxNumberValue);


#endif //LAB1_EXTERNALSORTS_H
