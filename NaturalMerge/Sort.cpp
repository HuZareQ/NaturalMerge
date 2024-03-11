#include "Sort.h"
#include <iostream>
#include <fstream>

using namespace std;

Sort::Sort() {

}

void Sort::sortRecords(int& series, int& phase, int& writeOperations, int& readOperations, bool showRecords) {
	bool sorted = false;
	int count = 0;
	while (!distribution(series, phase, writeOperations, readOperations)) {
		merge(writeOperations, readOperations);
		phase++;
		if(showRecords) Read.showRecords("records.bin");
	}

}

bool Sort::distribution(int& series, int& phase, int& writeOperations, int& readOperations) {
	double record[Tape::MAX_NUMBERS];
	//int countRecords = 0;

	double prev = 0;
	double current = 0;
	const char* file = "t1.bin";
	bool sorted = true;
	int countRecords = 0;

	std::ofstream fileStream("t1.bin", std::ios::trunc);
	fileStream.close();
	std::ofstream fileStream2("t2.bin", std::ios::trunc);
	fileStream2.close();

	while (Tape.readRecord("records.bin", record, readOperations)) {
		countRecords++;
		current = calculateSpan(record);
		if (prev > current && current >= 0) {
			file = changeFile(file);
			sorted = false;
			if(phase == 0)series++;
		}
		prev = current;
		Tape.writeRecord(file, record, true, writeOperations);
		//Tape.writeBlockToFile(file, record, Tape::MAX_NUMBERS, 1);
	}
	Tape.writeRecord(file, record, false, writeOperations);
	return sorted;
}

void Sort::merge(int& writeOperations, int& readOperations) {

	//int countRecords = 0, countRecordsT1 = 0, countRecordsT2 = 0;
	double recordT1[Tape::MAX_NUMBERS], recordT2[Tape::MAX_NUMBERS];
	double prevT1 = 0, prevT2 = 0, spanT1 = 0, spanT2 = 0;
	int countRecordsT1 = 0, countRecordsT2 = 0;
	bool T1exist = true, T2exist = true;

	std::ofstream fileStream("records.bin", std::ios::trunc);
	fileStream.close();

	// Odczytaj pierwsze rekordy z obu taœm
	T1exist = Tape.readRecord("t1.bin", recordT1, readOperations);
	T1exist = Tape.readRecord("t2.bin", recordT2, writeOperations);
	//Tape.readRecord("t1.bin", recordT1);
	//Tape.readRecord("t2.bin", recordT2);

	spanT1 = calculateSpan(recordT1);
	spanT2 = calculateSpan(recordT2);

	while (T1exist || T2exist) {
		if (prevT1 <= spanT1 && prevT2 <= spanT2 && T1exist && T2exist) {
			// Porównaj i zapisz mniejszy rekord
			if (spanT1 <= spanT2) {
				T1exist = nextRecord("records.bin", "t1.bin", recordT1, spanT1, prevT1, countRecordsT1, writeOperations, readOperations);
			}
			else {
				T2exist = nextRecord("records.bin", "t2.bin", recordT2, spanT2, prevT2, countRecordsT2, writeOperations, readOperations);
			}
		}
		else if ((prevT1 <= spanT1 || !T2exist) && T1exist) {
			// Zapisz pozosta³e rekordy z taœmy 1
			T1exist = nextRecord("records.bin", "t1.bin", recordT1, spanT1, prevT1, countRecordsT1, writeOperations, readOperations);
		}
		else if ((prevT2 <= spanT2 || !T1exist) && T2exist) {
			// Zapisz pozosta³e rekordy z taœmy 2
			T2exist = nextRecord("records.bin", "t2.bin", recordT2, spanT2, prevT2, countRecordsT2, writeOperations, readOperations);
		}
		else if (!(prevT1 <= spanT1) && !(prevT2 <= spanT2)) {
			prevT1 = 0;
			prevT2 = 0;
		}

		/*if (spanT1 < 0 && spanT2 < 0) {
			Tape.writeRecord("records.bin", recordT2, false);
			break;
		}*/
	}
	Tape.writeRecord("records.bin", recordT1, false, writeOperations);
}

bool Sort::nextRecord(const char* fileWrite, const char* fileRead, double* record, double& span, double& prev, int& countRecords, int& writeOperations, int& readOperations) {

	bool exist = false;
	//Tape.writeBlockToFile(fileWrite, record, Tape::MAX_NUMBERS, 1);
	Tape.writeRecord(fileWrite, record, true, writeOperations);
	countRecords++;
	if (Tape.readRecord(fileRead, record, readOperations)) exist = true;
	else return false;

	
	prev = span;
	span = calculateSpan(record);

	return exist;
}

const char* Sort::changeFile(const char* file) {
	if (file == "t1.bin") {
		file = "t2.bin";
	}
	else if (file == "t2.bin") {
		file = "t1.bin";
	}
	else {
		cerr << "Nieznany plik aktualny." << std::endl;
	}
	return file;
}

double Sort::calculateSpan(double* record) {

	double min = 1000000, max = 0;

	for (int i = 0; i < Tape::MAX_NUMBERS; i++) {

		if (record[i] > max)

			max = record[i];

		if (record[i] < min)

			min = record[i];

	}

	double wynik = max - min;
	return wynik;
}