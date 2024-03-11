#include <iostream>
#include "Tape.h"
#include "Sort.h"
#include "Write.h"
#include "Read.h"

using namespace std;

int main() {
    Tape Tape;
    Sort Sort;
    Write Write;
    Read Read;
    int command;
    int series = 1, phase = 0, writeOperations = 0, readOperations = 0;
    do {
        std::cout << "Podaj numer instrukcji, ktora chcesz wykonac:\n"
            << "1. Dodaj rekord\n"
            << "2. Wygeneruj x rekordow\n"
            << "3. Importuj dane z pliku\n"
            << "4. Wyswietl tasme\n"
            << "5. Sortuj tasme i wyswietl po kazdej fazie\n"
            << "6. Sortuj tasme\n"
            << "7. Wyjscie\n";

        std::cin >> command;

        switch (command) {
        case 1:
            Write.addRecordManually();
            break;

        case 2:
            Write.generateRecords();
            break;

        case 3:
            Write.importDataFromFile();
            break;

        case 4:
            Read.showRecords("records.bin");
            break;

        case 5:
            Read.showRecords("records.bin");
            Sort.sortRecords(series, phase, writeOperations, readOperations, true);
            cout << "Serie: " << series << endl;
            cout << "Fazy: " << phase << endl;
            cout << "Operacje zapisu: " << writeOperations << endl;
            cout << "Operacje odczytu: " << readOperations << endl;
            cout << "Operacje dyskowe: " << writeOperations + readOperations << endl;
            break;

        case 6:
            //Read.showRecords("records.bin");
            Sort.sortRecords(series, phase, writeOperations, readOperations, false);
            cout << "Serie: " << series << endl;
            cout << "Fazy: " << phase << endl;
            cout << "Operacje zapisu: " << writeOperations << endl;
            cout << "Operacje odczytu: " << readOperations << endl;
            cout << "Operacje dyskowe: " << writeOperations + readOperations << endl;
            //Read.showRecords("records.bin");
            break;

        case 7:
            std::cout << "Wyjscie\n";
            break;

        default:
            std::cout << "Niepoprawny numer instrukcji. Sprobuj ponownie.\n";
            break;
        }
    } while (command != 6);

    return 0;
}
