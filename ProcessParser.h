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

    static float getSysIdleCpuTime(vector<string> values) {
//        return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
        return 20.0;
    }

    static float getSysActiveCpuTime(vector<string> values) {
        return (stof(values[S_USER]) +
                stof(values[S_NICE]) +
                stof(values[S_SYSTEM]) +
                stof(values[S_IRQ]) +
                stof(values[S_SOFTIRQ]) +
                stof(values[S_STEAL]) +
                stof(values[S_GUEST]) +
                stof(values[S_GUEST_NICE]));

//        return (10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0);
    }
};

// TODO: Define all of the above functions below:
string ProcessParser::getCmd(string pid) {
    ifstream stream;
    Util::getStream(Path::basePath() + pid + Path::cmdPath(), stream);
    string line;
    getline(stream, line);
    return line;
}

vector<string> ProcessParser::getPidList() {
    std::vector<string> container;
    DIR *dir;
    if (!(dir = opendir("/proc"))) {
        throw std::runtime_error(std::strerror(errno));
    }
    while (dirent *dirp = readdir(dir)) {
        if (dirp->d_type != DT_DIR) {
            continue;
        }

        if (all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name), [](char c) { return std::isdigit(c); })) {
            container.push_back(dirp->d_name);
        }
    }
    if (closedir(dir))
        throw std::runtime_error(std::strerror(errno));
    return container;
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
    ifstream process;
    Util::getStream(Path::basePath() + pid + "/" + Path::statPath(), process);
    std::string line;
    float result;
    getline(process, line);
    std::istringstream buf(line);
    std::istream_iterator<string> beg(buf), end;
    std::vector<std::string> values(beg, end);
    float utime = stof(ProcessParser::getProcUpTime(pid));
    float stime = stof(values[14]);
    float cutime = stof(values[15]);
    float cstime = stof(values[16]);
    float starttime = stof(values[21]);
    float uptime = ProcessParser::getSysUpTime();
    float freq = sysconf(_SC_CLK_TCK);
    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime / freq);
    result = 100.0 * ((total_time / freq) / seconds);
    return std::to_string(result);
}

long int ProcessParser::getSysUpTime() {
    std::string line;
    ifstream process;
    Util::getStream(Path::basePath() + Path::upTimePath(), process);
    getline(process, line);
    std::istringstream buf(line);
    std::istream_iterator<string> beg(buf), end;
    std::vector<string> values(beg, end);
    return stoi(values[0]);
}

std::string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string> values2) {
    // TODO the values1, values2 causes segmentation error and dump, when passed to these functions. lets investigate it during review
//    float activeTime = ProcessParser::getSysActiveCpuTime(values2) - ProcessParser::getSysActiveCpuTime(values1);
//    float idleTime = ProcessParser::getSysIdleCpuTime(values2) - ProcessParser::getSysIdleCpuTime(values1);
//    float totalTime = activeTime + idleTime;
//    float result = 100.0 * (activeTime / totalTime);
//    return to_string(result);
    return "";
}

bool ProcessParser::isPidExisting(string pid) {
    return false;
}

string ProcessParser::getOSName() {
    std::string line;
    string name = "PRETTY_NAME=";
    ifstream stream;
    Util::getStream("/etc/os-release", stream);
    while (getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            std::size_t found = line.find("=");
            found++;
            std::string result = line.substr(found);
            result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
            return result;
        }
    }

    return "";
}

std::string ProcessParser::getProcUpTime(string pid) {
    string line;
    string value;
    float result;
    ifstream stream;
    Util::getStream((Path::basePath() + pid + "/" + Path::statPath()), stream);
    getline(stream, line);
    string str = line;
    istringstream buf(str);
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end); // done!
    // Using sysconf to get clock ticks of the host machine
    return to_string(float(stof(values[13]) / sysconf(_SC_CLK_TCK)));
}

string ProcessParser::getProcUser(string pid) {
    std::string line;
    std::string name = "Uid:";
    std::string result = "";
    ifstream process;
    Util::getStream((Path::basePath() + pid + Path::statusPath()), process);
    while (getline(process, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            result = values[1];
            break;
        }
    }
    ifstream stream;
    Util::getStream("/etc/passwd", stream);
    name = ("x:" + result);
    while (std::getline(stream, line)) {
        if (line.find(name) != std::string::npos) {
            result = line.substr(0, line.find(":"));
            return result;
        }
    }
    return "";
}

vector<string> ProcessParser::getSysCpuPercent(string coreNumber) {
    string line;
    string key{"core" + coreNumber};

    ifstream stream;
    Util::getStream(Path::basePath() + Path::statPath(), stream);

    while (getline(stream, line)) {
        if (line.compare(0, key.size(), key) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> line_content(beg, end);
            return line_content;
        }
    }
    return vector<string>();
}

float ProcessParser::getSysRamPercent() {
    std::string memory_available = "MemAvailable:";
    std::string memory_free = "MemFree:";
    std::string buffers = "Buffers:";
    float total_memory = 0.0;
    float free_memory = 0.0;
    float buffer = 0.0;

    std::string line;
    ifstream stream;
    Util::getStream(Path::basePath() + Path::memInfoPath(), stream);
    while (getline(stream, line)) {
        if (line.compare(0, memory_available.size(), memory_available) == 0) {
            istringstream buf{line};
            istream_iterator<string> beg(buf), end;
            std::vector<string> content(beg, end);
            total_memory = stof(content[1]);
        }
        if (line.compare(0, memory_free.size(), memory_free) == 0) {
            istringstream buf{line};
            istream_iterator<string> beg(buf), end;
            std::vector<string> content(beg, end);
            free_memory = stof(content[1]);
        }
        if (line.compare(0, buffers.size(), buffers) == 0) {
            istringstream buf{line};
            istream_iterator<string> beg(buf), end;
            std::vector<string> content(beg, end);
            buffer = stof(content[1]);
        }
    }
    return 100.0 * (free_memory / (total_memory - buffer));
}

string ProcessParser::getSysKernelVersion() {
    std::string line;
    ifstream stream;
    Util::getStream(Path::basePath() + Path::versionPath(), stream);
    getline(stream, line);
    istringstream buff(line);
    istream_iterator<string> beg(buff), end;
    std::vector<string> content(beg, end);
    return content[0] + " " + content[1] + " " + content[2];
}

int ProcessParser::getNumberOfCores() {
    string cpu_core = "cpu cores";
    ifstream stream;
    Util::getStream(Path::basePath() + "cpuinfo", stream);
    string line;

    while (getline(stream, line)) {
        if (line.compare(0, cpu_core.size(), cpu_core) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> content(beg, end);
            return stoi(content[3]);
        }
    }
    return 0;
}

int ProcessParser::getTotalThreads() {
    std::vector<string> pid_list{ProcessParser::getPidList()};
    string name = "Threads:";
    int result = 0;
    for (int i = 0; i < pid_list.size(); ++i) {
        ifstream stream;
        Util::getStream(Path::basePath() + pid_list[i] + Path::statusPath(), stream);
        std::string line;
        while (getline(stream, line)) {
            if (line.compare(0, name.size(), name) == 0) {
                istringstream buf(line);
                istream_iterator<string> beg(buf), end;
                vector<string> values(beg, end);
                result += stoi(values[1]);
                break;
            }
        }
    }
    return result;
}

int ProcessParser::getTotalNumberOfProcesses() {
    return 0;
}

int ProcessParser::getNumberOfRunningProcesses() {
    ifstream stream;
    int result = 0;
    string name = "procs_running";
    Util::getStream(Path::basePath() + Path::statPath(), stream);
    std::string line;
    while (getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buff(line);
            istream_iterator<string> beg(buff), end;
            std::vector<string> content(beg, end);
            result += stoi(content[1]);
            break;
        }
    }
    return result;
}


