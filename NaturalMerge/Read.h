#ifndef READ_H
#define READ_H
#include "Tape.h"

class Read {
private:
    Tape Tape;
public:
    Read();
    void showRecords(const char* filename);
};
#endif