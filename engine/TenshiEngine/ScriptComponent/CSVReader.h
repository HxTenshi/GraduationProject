#pragma once
#include "h_standard.h"

static class CSVReader
{ 
private:
	CSVReader();
	~CSVReader();
public:
	static std::vector<std::vector<std::string>> readCSV(std::string fileName);
private:

};