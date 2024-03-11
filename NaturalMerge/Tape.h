#ifndef TAPE_H
#define TAPE_H

class Tape {
private:
    
    bool ReadRecordFromBlock(const char* filename, double* record, double* block, int& countReadRecords, int& countBlocks, int& readOperations);
    void writeRecordInBlock(const char* filename, double* record, double* block, int& countRecords, bool flag, int& writeOperations);
    void sendIncomplete(double* blockT0, double* blockT1, double* blockT2, int& countRecordsT0, int& countRecordsT1, int& countRecordsT2, int& writeOperations);
public:
    Tape();
    const static int MAX_NUMBERS = 15;
    const static int BLOCK_SIZE = 50;

    bool readRecord(const char* filename, double* record, int& readOperations);
    void writeRecord(const char* filename, double* record, bool flag, int& writeOperations);

    void writeBlockToFile(const char* filename, double* block, int numberOfNumbers, int numberRecordsInBlock);
    bool readBlockFromFile(const char* filename, double* block, int countBlock, int numberRecordsInBlock);

};

#endif 