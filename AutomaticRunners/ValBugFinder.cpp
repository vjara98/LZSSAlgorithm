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
using namespace std;
int main(int argc, char *argv[]) {
	int lookAheadBuffer = 512;
	int searchBuffer = 512;
	string file_name = "obj1";
	
	string searchBufferLength = std::to_string(searchBuffer);
	string lookAheadLength = std::to_string(lookAheadBuffer);
	
	string run_string = "./ValProject " + file_name + " " + searchBufferLength + " " + lookAheadLength;
	
	vector<int> runs_before_break;
	
	
	
	//for(int i = 0; i < 50; i++) {
		int runs = 0;
		string resultStr = exec(run_string.c_str());
		//cout << resultStr;
		
		double goal_result = atof(resultStr.c_str());
		double result = atof(resultStr.c_str());
		
		while(goal_result == result) {
			resultStr = exec(run_string.c_str());
			result = atof(resultStr.c_str());
			
			runs++;
			cout << runs << endl;
			if(goal_result != result) {
				cout << goal_result << "\t" << result << endl;
			}
		}
		cout << "shit";
		//runs_before_break.push_back(runs);
	//}
	/*for(int i = 0; i < runs_before_break.size(); i++) {
		cout << runs_before_break[i] << ", ";
	}*/
	//cout << runs;
}