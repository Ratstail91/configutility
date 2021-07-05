#include "config_utility.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>

//utility functions
static inline bool striequal(std::string const& a, std::string const& b) {
	return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b){ return tolower(a) == tolower(b); });
}

static inline bool strnequal(std::string const& a, std::string const& b, size_t n) {
	return a.compare(0, n, b);
}

static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

//class methods
void ConfigUtility::LoadConfig(std::string const& fname, bool skipMissingFile, int argc, char* argv[]) {
	//clear the stored configuration
//	table.clear();

	//use the default file
	if (argc < 2) {
		table = ReadFile(fname, skipMissingFile);
		return;
	}

	//some variables to use
	table_t conglomerate;
	table_t commandLine;

	//reading EACH specified file from the command line
	for (int i = 1; i < argc; i++) {
		//read from a specified config file
		if (strnequal(std::string(argv[i]), "--config=", 9)) {
			//older specified files take precedence
			table_t tmp = ReadFile(std::string(argv[i] + 9), skipMissingFile);
			conglomerate.insert(tmp.begin(), tmp.end());
			continue;
		}

		//set some specific values
		char key[256], val[256];
		if (strnequal(std::string(argv[i]), "-", 1)) {
			//zero the variables
			memset(key, 0, 256);
			memset(key, 0, 256);

			//read the key-value pair
			if (sscanf(argv[i], "-%[^=]=%[^\\0]", key, val) != 2) {
				std::ostringstream os;
				os << "Failed to read a command line config argument (expected format -key=val):\n";
				os << "\targv[" << i << "]: " << argv[i] << "\n";
				os << "\tkey: " << key << "\n";
				os << "\tval: " << val << "\n";
				throw(std::runtime_error( os.str() ));
			}
			commandLine.try_emplace(key, val);
		}
	}

	//finally, construct the final config table
	table.insert(commandLine.begin(), commandLine.end());
	table.insert(conglomerate.begin(), conglomerate.end());
}

ConfigUtility::table_t ConfigUtility::ReadFile(std::string const& fname, bool skipMissingFile) {
	//read in and return this file's data
	table_t retTable;
	std::ifstream is(fname);

	if (!is) {
		if (skipMissingFile) {
			return {}; //empty table
		}
		std::ostringstream os;
		os << "Failed to open a config file: " << fname;
		throw(std::runtime_error( os.str() ));
	}

	std::string key, val;

	while(!is.eof()) { //forever
		//eat whitespace
		if (isspace(is.peek())) {
			is.ignore();
			continue;
		}

		//skip comment lines
		if (is.peek() == '#') {
			while(!is.eof() && is.peek() != '\n') {
				is.ignore();
			}
			continue;
		}

		//read in the pair
		getline(is, key,'=');
		getline(is, val);

		//trim
		trim(key);
		trim(val);

		//disallow empty/wiped pairs
		if (key.size() == 0 || val.size() == 0) {
			continue;
		}

		//save the pair
		retTable.try_emplace(key, val);
	}

	is.close();

	//recursively load in any subordinate config files
	auto next = retTable.find("_config.next");
	if (next != retTable.end()) {
		table_t subTable = ReadFile(next->second, skipMissingFile);
		retTable.insert(subTable.begin(), subTable.end());
	}

	return retTable;
}

//-------------------------
//Convert to a type
//-------------------------

std::string const& ConfigUtility::String(std::string const& key) {
	auto iter = table.find(key);
	if (iter == table.end()) {
		return table["_config.empty"]; //reserved "keyword"
	}
	return iter->second;
}

int ConfigUtility::Integer(std::string const& key) const {
	auto iter = table.find(key);
	if (iter == table.end()) {
		return 0;
	}
	return std::stoi(iter->second);
}

float ConfigUtility::Float(std::string const& key) const {
	auto iter = table.find(key);
	if (iter == table.end()) {
		return 0.0f;
	}
	return std::stof(iter->second);
}

bool ConfigUtility::Boolean(std::string const& key) const {
	auto iter = table.find(key);
	if (iter == table.end()) {
		return false;
	}
	return striequal(iter->second, "true");
}
