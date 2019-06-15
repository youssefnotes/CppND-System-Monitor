#include <string>

using namespace std;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
private:
    string pid;
    string user;
    string cmd;
    string cpu;
    string mem;
    string upTime;

public:
    Process(string pid) {
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        //TODOs:
        //complete for mem
        this->mem = ProcessParser::getSysRamPercent();
        //complete for cmd
        this->cmd = ProcessParser::getCmd(this->pid);
        //complete for upTime
        this->upTime = ProcessParser::getProcUpTime(this->pid);
        //complete for cpu
        this->cpu = ProcessParser::getCpuPercent(this->pid);
    }

    void setPid(int pid);

    string getPid() const;

    string getUser() const;

    string getCmd() const;

    int getCpu() const;

    int getMem() const;

    string getUpTime() const;

    string getProcess();
};

void Process::setPid(int pid) {
    this->pid = pid;
}

string Process::getPid() const {
    return this->pid;
}

string Process::getProcess() {
    if (!ProcessParser::isPidExisting(this->pid))
        return "";
    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);
//TODO: finish the string! this->user + "   "+ mem...cpu...upTime...;
    return (this->pid + "   " + this->user + +"   " + this->cmd + "   " + this->cpu + "   " + this->mem + "   " + this->upTime);
}
