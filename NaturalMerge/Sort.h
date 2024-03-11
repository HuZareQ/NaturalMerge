#ifndef SORT_H
#define SORT_H
#include "Tape.h"
#include "Read.h"

class Sort {
private:
    Tape Tape;
    Read Read;
    bool distribution(int& series, int& phase, int& writeOperations, int& readOperations);
    void merge(int& writeOperations, int& readOperations);
    
    const char* changeFile(const char* file);
    bool nextRecord(const char* fileWrite, const char* fileRead, double* record, double& span, double& prev, int& countRecord, int& writeOperations, int& readOperations);
public:
    Sort();
    void sortRecords(int& series, int& phase, int& writeOperations, int& readOperations, bool showRecords);
    double calculateSpan(double* record);
};

#endif 