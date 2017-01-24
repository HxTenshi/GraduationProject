
#pragma once
#include "main.h"
#include <string>
#include <vector>


class FuckTenshiEngine :public IDllScriptComponent{
};

namespace Mossan {

	static class CSVScript {
	public:
		CSVScript();
		~CSVScript();
		static void writeCSV(std::string fileName, std::vector<std::vector<std::string>> arrays);
		static std::vector<std::vector<std::string>> readCSV(std::string fileName);
	private:
		//ƒƒ“ƒo•Ï”
	};
}