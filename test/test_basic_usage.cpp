#include "test_basic_usage.hpp"

#include "config_utility.hpp"

TestResult testBasicUsage() {
	TestResult results;

	results.expected = 1;

	{
		ConfigUtility config;
		config.LoadConfig("test/configs/basic.cfg");
		if (config["key"] == "value") {
			results.observed++;
		}
	}

	return {1, 1};
}