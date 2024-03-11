#include "Tape.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

Tape::Tape() {

}

// Implementacja zapisu rekordu do pliku
void Tape::writeBlockToFile(const char* filename, double* block, int numberOfNumbers, int numberRecordsInBlock) {
    
    ofstream binaryFile(filename, ios::binary | ios::app);

    if (!binaryFile.is_open()) {
        cerr << "Nie mozna otworzyc pliku do zapisu." << endl;
        return;
    }

    // Uzupe³nij pozosta³e miejsca nan
    for (int i = numberOfNumbers; i < numberRecordsInBlock * MAX_NUMBERS; i++) {
        block[i] = NAN;
    }

    binaryFile.write(reinterpret_cast<char*>(block), numberRecordsInBlock * MAX_NUMBERS * sizeof(double));

    if (!binaryFile.good()) {
        cerr << "Blad podczas zapisywania do pliku." << endl;
        return;
    }

    //cout << "Rekord zosta³ zapisany do pliku binarnego." << endl;

    binaryFile.close();
}

void Tape::writeRecordInBlock(const char* filename, double* record, double* block, int& countRecords, bool flag, int& writeOperations) {

    for (int i = 0; i < MAX_NUMBERS; i++) {
        block[(countRecords * MAX_NUMBERS) + i] = record[i];
    }
    countRecords++;

    if (countRecords % BLOCK_SIZE == 0) {
        writeBlockToFile(filename, block, Tape::MAX_NUMBERS * Tape::BLOCK_SIZE, Tape::BLOCK_SIZE);
        countRecords = 0;
        writeOperations++;
    }
    
}

void Tape::sendIncomplete(double* blockT0, double* blockT1, double* blockT2, int& countRecordsT0, int& countRecordsT1, int& countRecordsT2, int& writeOperations) {
    if (countRecordsT0 != 0) {
        writeBlockToFile("records.bin", blockT0, countRecordsT0 * Tape::MAX_NUMBERS, Tape::BLOCK_SIZE);
        countRecordsT0 = 0;
    }
    if (countRecordsT1 != 0) {
        writeBlockToFile("t1.bin", blockT1, countRecordsT1 * Tape::MAX_NUMBERS, Tape::BLOCK_SIZE);
        countRecordsT1 = 0;
    }
    if (countRecordsT2 != 0) {
        writeBlockToFile("t2.bin", blockT2, countRecordsT2 * Tape::MAX_NUMBERS, Tape::BLOCK_SIZE);
        countRecordsT2 = 0;
    }
}

void Tape::writeRecord(const char* filename, double* record, bool flag, int& writeOperations) {

    double static blockT0[Tape::BLOCK_SIZE * Tape::MAX_NUMBERS];
    double static blockT1[Tape::BLOCK_SIZE * Tape::MAX_NUMBERS];
    double static blockT2[Tape::BLOCK_SIZE * Tape::MAX_NUMBERS];
    int static countRecordsT1 = 0, countRecordsT2 = 0, countRecordsT0 = 0;
    int static countBlocksT1 = 0, countBlocksT2 = 0, countBlocksT0 = 0;
    if (flag == false) sendIncomplete(blockT0, blockT1, blockT2, countRecordsT0, countRecordsT1, countRecordsT2, writeOperations);
    else {
        if (filename == "t1.bin") {
            writeRecordInBlock(filename, record, blockT1, countRecordsT1, flag, writeOperations);
        }
        else if (filename == "t2.bin") {
            writeRecordInBlock(filename, record, blockT2, countRecordsT2, flag, writeOperations);
        }
        else {
            writeRecordInBlock(filename, record, blockT0, countRecordsT0, flag, writeOperations);
        }
    }
}

// Implementacja odczytu rekordów z pliku
bool Tape::readBlockFromFile(const char* filename, double* block, int countBlock, int numberRecordsInBlock) {
    bool recordExist = false;

    for (int i = 0; i < numberRecordsInBlock * MAX_NUMBERS; i++) {
        block[i] = NAN;
    }

    std::fstream binaryFile(filename, std::ios::binary | std::ios::in | std::ios::out);

    if (!binaryFile.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku do odczytu." << std::endl;
        return false;
    }

    if (!binaryFile.good()) {
        std::cerr << "Blad podczas odczytywania z pliku." << std::endl;
        return false;
    }

    // Ustawienie pozycji wskaŸnika przed odczytem
    binaryFile.seekg(countBlock * numberRecordsInBlock * MAX_NUMBERS * (sizeof(double)), std::ios::beg);

    // Odczytanie bloku
    if(binaryFile.read(reinterpret_cast<char*>(block), numberRecordsInBlock * MAX_NUMBERS * sizeof(double))) recordExist = true;

    
    /*for (int i = 0; i < numberRecordsInBlock * MAX_NUMBERS; i++) {
        cout << block[i] << " ";
    }
    if (isnan(block[0])) {
        recordExist = false;
    }*/


    binaryFile.close();
    return recordExist;
}

bool Tape::ReadRecordFromBlock(const char* filename, double* record, double* block, int& countRecords, int& countBlocks, int& readOperations) {

    if (countRecords == 0) {
        readBlockFromFile(filename, block, countBlocks, BLOCK_SIZE);
        readOperations++;
    }


    for (int i = 0; i < MAX_NUMBERS; i++) {
        record[i] = block[(countRecords * MAX_NUMBERS) + i];
    }
    countRecords++;

    if (countRecords % BLOCK_SIZE == 0) {
        countRecords = 0;
        countBlocks++;
    }

    if (isnan(record[0])) {
        countRecords = 0;
        countBlocks = 0;
        return false;
    }

    return true;
}

bool Tape::readRecord(const char* filename, double* record, int& readOperations) {
    bool exist = true;

    double static blockT0[Tape::BLOCK_SIZE * Tape::MAX_NUMBERS];
    double static blockT1[Tape::BLOCK_SIZE * Tape::MAX_NUMBERS];
    double static blockT2[Tape::BLOCK_SIZE * Tape::MAX_NUMBERS];
    int static countRecordsT1 = 0, countRecordsT2 = 0, countRecordsT0 = 0;
    int static countBlocksT1 = 0, countBlocksT2 = 0, countBlocksT0 = 0;

    if (filename == "t1.bin") {
        exist = ReadRecordFromBlock(filename, record, blockT1, countRecordsT1, countBlocksT1, readOperations);
    }
    else if (filename == "t2.bin") {
        exist = ReadRecordFromBlock(filename, record, blockT2, countRecordsT2, countBlocksT2, readOperations);
    }
    else {
        exist = ReadRecordFromBlock(filename, record, blockT0, countRecordsT0, countBlocksT0, readOperations);
    }
    

    return exist;
}