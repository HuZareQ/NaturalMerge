#include "Read.h"
#include "Sort.h"
#include <iostream>
#include <fstream>

using namespace std;

Read::Read() {
}

// Implementacja wyœwietlania rekordów
void Read::showRecords(const char* filename) {
    Sort Sort;
    ifstream binaryFile(filename, ios::binary | ios::in);
    double record[Tape::MAX_NUMBERS];
    if (!binaryFile.is_open()) {
        cerr << "Nie mozna otworzyc pliku do odczytu." << endl;
        return;
    }

    if (!binaryFile.good()) {
        cerr << "Blad podczas odczytywania z pliku." << endl;
        return;
    }
    cout << "----------------------------------------"<< endl;
    while (binaryFile.read(reinterpret_cast<char*>(record), Tape::MAX_NUMBERS * sizeof(double))) {
        if (!isnan(record[0])) {
            cout << "Wczytany rekord: ";
                for (int i = 0; i < Tape::MAX_NUMBERS; i++) {
                    cout << record[i] << " ";
                }
            cout << endl;
                cout << Sort.calculateSpan(record);
                cout << endl;
        }
    }
    

    binaryFile.close();
}

