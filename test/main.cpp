//test headers
#include "test_compiles.hpp"
#include "test_basic_usage.hpp"

#include "common.hpp"

#include <functional>
#include <iostream>
#include <vector>

const std::vector<std::function<TestResult()>> testVector = {
	testCompiles,
	testBasicUsage,
};

int main() {
	TestResult results;

	//run each test in sequence, recording the result
	for (auto t : testVector) {
		TestResult res = t();

//		std::cout << res.observed << "/" << res.expected << std::endl;

		results += res;
	}

	//finally
	std::cout <<  results.observed << "/" << results.expected << " tests passed." << std::endl;
	return results.expected - results.observed;
}