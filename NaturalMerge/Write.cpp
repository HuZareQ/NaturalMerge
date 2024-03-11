#include "Write.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

Write::Write() {
}


// Implementacja dodawania rekordu rêcznie
int Write::addRecordManually() {
    double record[Tape::MAX_NUMBERS];
    int numberOfNumbers;

    cout << "Podaj ilosc liczb rzeczywistych (do 15): ";
    if (!(cin >> numberOfNumbers) || numberOfNumbers <= 0 || numberOfNumbers > Tape::MAX_NUMBERS) {
        cerr << "Nieprawid³owa liczba." << endl;
        return 1;
    }

    cout << "Podaj " << numberOfNumbers << " liczb rzeczywistych (oddzielone spacjami): ";

    for (int i = 0; i < numberOfNumbers; ++i) {
        if (!(cin >> record[i])) {
            cerr << "Blad podczas wprowadzania danych." << endl;
            return 1;
        }
    }

    Tape.writeBlockToFile("records.bin", record, numberOfNumbers, 1);
}

// Implementacja generowania rekordów
int Write::generateRecords() {
    srand(time(NULL));

    double record[Tape::MAX_NUMBERS];
    int numberOfRecords;
    int numberOfNumbers;
    int maxNumber = 1000;
    cout << "Podaj ilosc rekordow ";
    if (!(cin >> numberOfRecords) || numberOfRecords <= 0) {
        cerr << "Nieprawidlowa liczba." << endl;
        return 1;
    }

    for (int i = 0; i < numberOfRecords; i++) {
        //numberOfNumbers = 2;
        numberOfNumbers = 1 + rand() % (Tape::MAX_NUMBERS);
        for (int k = 0; k < numberOfNumbers; k++) {
            record[k] = (double)rand() / RAND_MAX * maxNumber;
            //record[k] = rand() % 100;
        }
        Tape.writeBlockToFile("records.bin", record, numberOfNumbers, 1);
    }
}

// Implementacja importowania danych z pliku
void Write::importDataFromFile() {
    // Nazwy plików wejœciowego i wyjœciowego
    const char* inputFileName = "dane.txt";
    const char* outputFileName = "records.bin";

    // Otwarcie pliku wejœciowego
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku wejsciowego: " << inputFileName << std::endl;
        return;
    }

    // Otwarcie pliku wyjœciowego w trybie binarnym
    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku wyjsciowego: " << outputFileName << std::endl;
        return;
    }

    // Wczytywanie i zapisywanie danych
    double number;
    while (inputFile >> number) {
        cout << number;
        // Zapisywanie liczby binarnie do pliku wyjœciowego
        outputFile.write(reinterpret_cast<const char*>(&number), sizeof(double));
    }

    // Zamkniêcie plików
    inputFile.close();
    outputFile.close();

    std::cout << "Operacja zapisu do pliku binarnego zakonczona pomyslnie." << std::endl;
}