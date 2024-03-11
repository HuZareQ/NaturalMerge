#ifndef WRITE_H
#define WRITE_H
#include "Tape.h"

class Write {
private:
    Tape Tape;
public:
    Write();
    int addRecordManually();
    int generateRecords();
    void importDataFromFile();
};
#endif