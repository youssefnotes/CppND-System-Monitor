#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "constants.h"
#include "util.h"

using namespace std;

class ProcessParser {
private:
    std::ifstream stream;
public:
    static string getCmd(string pid);

    static vector<string> getPidList();

    static std::string getVmSize(string pid);

    static std::string getCpuPercent(string pid);

    static long int getSysUpTime();

    static std::string getProcUpTime(string pid);

    static string getProcUser(string pid);

    static vector<string> getSysCpuPercent(string coreNumber = "");

    static float getSysRamPercent();

    static string getSysKernelVersion();

    static int getNumberOfCores();

    static int getTotalThreads();

    static int getTotalNumberOfProcesses();

    static int getNumberOfRunningProcesses();

    static string getOSName();

    static std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2);

    static bool isPidExisting(string pid);
};
// TODO: Define all of the above functions below:
string ProcessParser::getCmd(string pid) {
    return std::__cxx11::string();
}

vector<string> ProcessParser::getPidList() {
    return vector<string>();
}

std::string ProcessParser::getVmSize(string pid) {
    ifstream process;
    Util::getStream(Path::basePath() + pid + Path::statusPath(), process);
    std::string line;
    const std::string vm_size = "VmSize:";
    float result;
    while (getline(process, line)) {
//        if (line.find("VmSize:") != std::string::npos) {
//        return line.substr(10, 15);
//        }
        if (line.compare(0, vm_size.size(), vm_size) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            result = (stof(values[1]) / float(1024 * 1024));
            break;
        }
    }
    return to_string(result);
}

std::string ProcessParser::getCpuPercent(string pid) {
    return std::__cxx11::string();
}

long int ProcessParser::getSysUpTime() {
    return 0;
}

std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2) {
    return std::__cxx11::string();
}

bool ProcessParser::isPidExisting(string pid) {
    return false;
}

string ProcessParser::getOSName() {
    return std::__cxx11::string();
}

std::string ProcessParser::getProcUpTime(string pid) {
    return std::__cxx11::string();
}

string ProcessParser::getProcUser(string pid) {
    return std::__cxx11::string();
}

vector<string> ProcessParser::getSysCpuPercent(string coreNumber) {
    return vector<string>();
}

float ProcessParser::getSysRamPercent() {
    return 0;
}

string ProcessParser::getSysKernelVersion() {
    return std::__cxx11::string();
}

int ProcessParser::getNumberOfCores() {
    return 0;
}

int ProcessParser::getTotalThreads() {
    return 0;
}

int ProcessParser::getTotalNumberOfProcesses() {
    return 0;
}

int ProcessParser::getNumberOfRunningProcesses() {
    return 0;
}


