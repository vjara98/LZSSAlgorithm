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
				for(int c = 0; c < sbs.size()-1; c++) {
					cout << sbs[c].result << ", ";
				}
				cout << sbs[sbs.size() - 1].result;
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
				cout << files[i].get_file_name() << endl;
				files[i].print_test();
				cout << endl << endl;
			}
		}
};


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
		for(int lookAheadBuffer = 1; lookAheadBuffer <= 9; lookAheadBuffer++) {
			for(int searchBuffer = lookAheadBuffer; searchBuffer <= 9; searchBuffer++) {
				string searchBufferLength = std::to_string(pow(2,searchBuffer));
				string lookAheadLength = std::to_string(pow(2,lookAheadBuffer));
				string run_string = "./ValProject " + file_name + " " + searchBufferLength + " " + lookAheadLength;
				//cout << run_string;
				//cout << lookAheadLength << searchBufferLength << "\t\t\t";
				string result = exec(run_string.c_str());
				//cout << result;
				cout << endl;
				TestsRun.add_test(pow(2,lookAheadBuffer), pow(2,searchBuffer), atof(result.c_str()));
				//cout << endl;
				cout << "test done";
			}
			cout << "upping lookahead" << endl;
		}
	}
	TestsRun.print();
	
	//cout << exec("./ValProject");
	//cout << exec("./ValProject");
}