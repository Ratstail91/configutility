#pragma once

#include <map>
#include <string>

class ConfigUtility {
public:
	void LoadConfig(std::string const& fname, bool skipMissingFile = false, int argc = 0, char* argv[] = nullptr);

	//convert to a type
	std::string const& String(std::string const&);
	int Integer(std::string const&) const;
	float Float(std::string const&) const;
	bool Boolean(std::string const&) const;

	//shorthand
	inline std::string const& operator[](std::string const& key) { return String(key); }
	inline int Int(std::string const& key) const { return Integer(key); }
	inline bool Bool(std::string const& key) const { return Boolean(key); }

private:
	using table_t = std::map<std::string const, std::string const>;

	table_t ReadFile(std::string const& fname, bool skipMissingFile);

	table_t table;
};