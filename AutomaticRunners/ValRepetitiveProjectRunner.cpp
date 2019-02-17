#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <math.h>
#include <vector>
#include <map>
#include <stdlib.h> 
#include <sstream>

using namespace std;

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

class Test {
	public:
		int look_ahead_size;
		int search_buffer_size;
		double result;
		Test(){}
		Test(int las, int sbs, double r) {
			look_ahead_size = las;
			search_buffer_size = sbs;
			result = r;
		}
};

//bool TestComparison (Test a, Test b) {return a.look_ahead_size < b.look_ahead_size; }
class FileTests {
	std::string file_name;
	//search buffer = key
	std::map<int, std::vector<Test>> tests;
	//td::vector<Test> tests;
	public: 
		FileTests(){}
		FileTests(std::string name) {
			file_name = name;
		}
		void add_test(int las, int sbs, double r) {
			Test test(las, sbs, r);
			//pow(2,1.0/sbs);
			tests[sbs].push_back(test);
		}
		std::string get_file_name() {
			return file_name;
		}
		void print_test() {
			for(int i = 9; i > 0; i--) {
				auto sbs = tests[pow(2, i)];
				for(int c = 0; c < sbs.size(); c++) {
					cout << sbs[c].result << ", ";
				}
				cout << endl;
			}
		}
};
class Tests {
	std::vector<FileTests> files;
	public: 
		Tests() {}
		void add_file(std::string file_name) {
			FileTests new_file(file_name);
			files.push_back(new_file);
		}
		void add_test(int las, int sbs, double r) {
			files[files.size() - 1].add_test(las, sbs, r);
		}
		void print() {
			for(int i = 0; i < files.size(); i++) {
				cout << files[i].get_file_name() << "," << endl;
				files[i].print_test();
			}
		}
};

string run_test(string file_name, int searchBufferLength, int lookAheadLength) {
	string searchBuffer = std::to_string(searchBufferLength);
	string lookAhead = std::to_string(lookAheadLength);
	
	string run_string = "./ValProject " + file_name + " " + searchBuffer + " " + lookAhead;
	
	string result = exec(run_string.c_str());
	
	return result;
	//atof(result.c_str());
}

double run_test_until_correct(string file_name, int searchBufferLength, int lookAheadLength) {
	bool correct = false;
	double result = 0;
	int runs = 0;
	while(correct == false && runs < 30) {
		runs++;
		istringstream output(run_test(file_name, searchBufferLength, lookAheadLength));
		output >> correct;
		output >> result;
	}
	return result;
}


int main(int argc, char *argv[]) {
	vector<string> file_names;
	file_names.push_back("bib");
	file_names.push_back("book1");
	file_names.push_back("book2");
	file_names.push_back("geo");
	file_names.push_back("news");
	file_names.push_back("obj1");
	file_names.push_back("obj2");
	file_names.push_back("paper1");
	file_names.push_back("paper2");
	file_names.push_back("paper3");
	file_names.push_back("paper4");
	file_names.push_back("paper5");
	file_names.push_back("paper6");
	file_names.push_back("pic");
	file_names.push_back("progc");
	file_names.push_back("progl");
	file_names.push_back("progp");
	file_names.push_back("trans");
	
	Tests TestsRun;
	
	for(int fileIndex = 0; fileIndex < file_names.size(); fileIndex++) {
		std::string file_name = file_names[fileIndex];
		TestsRun.add_file(file_name);
		cout << "Starting " << file_name << endl;
		for(int lookAheadBuffer = 1; lookAheadBuffer <= 9; lookAheadBuffer++) {
			for(int searchBuffer = lookAheadBuffer; searchBuffer <= 9; searchBuffer++) {
				int searchBufferLength = pow(2,searchBuffer);
				int lookAheadLength = pow(2,lookAheadBuffer);
				
				double result = run_test_until_correct(file_name, searchBufferLength, lookAheadLength);
				
				TestsRun.add_test(lookAheadLength, searchBufferLength, result);
				//cout << endl;
				cout << "Test Done" << endl;
			}
			cout << "Upping lookahead" << endl;
		}
	}
	TestsRun.print();
	
	//cout << exec("./ValProject");
	//cout << exec("./ValProject");
}