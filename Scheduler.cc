#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits.h>
#include <algorithm>

using namespace std;

struct Process
{
    int processID;
    int arrivalTime;
    int burstDuration;
    int priority;
    int remainingTime;
    int startTime;
    int finishTime;
};
void readProcesses(const string &filename, vector<Process> &processes)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    while (getline(file, line))
    {
        Process process;
        stringstream ss(line);
        ss >> process.processID;
        ss.ignore();
        ss >> process.arrivalTime;
        ss.ignore();
        ss >> process.burstDuration;
        ss.ignore();
        ss >> process.priority;
        process.remainingTime = process.burstDuration;
        process.startTime = -1;
        process.finishTime = -1;
        processes.push_back(process);
    }
    file.close();
}
void FCFS(vector<Process> &processes)
{
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrivalTime < b.arrivalTime;
    });    
    int currentTime = 0;
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    for (auto &process : processes)
    {
        if (currentTime < process.arrivalTime)
        {
            currentTime = process.arrivalTime;
        }
        process.startTime = currentTime;
        process.finishTime = currentTime + process.burstDuration;
        int waitingTime = process.startTime - process.arrivalTime;
        int turnaroundTime = process.finishTime - process.arrivalTime;
        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;
        currentTime += process.burstDuration;
    }
    double averageTurnaroundTime = static_cast<double>(totalTurnaroundTime) / processes.size();
    double averageWaitingTime = static_cast<double>(totalWaitingTime) / processes.size();
    double throughput = static_cast<double>(processes.size()) / currentTime;
    cout << "--- FCFS ---" << endl;
    cout << "Average Turnaround Time: " << averageTurnaroundTime << endl;
    cout << "Average Waiting Time: " << averageWaitingTime << endl;
    cout << "Throughput: " << throughput << endl;
}
void SJF(vector<Process> &processes)
{
    int currentTime = 0;
    size_t completed = 0;
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    while (completed < processes.size())
    {
        int shortestRemainingTime = INT_MAX;
        int shortestIndex = -1;
        for (size_t i = 0; i < processes.size(); ++i)
        {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0)
            {
                if (processes[i].remainingTime < shortestRemainingTime)
                {
                    shortestRemainingTime = processes[i].remainingTime;
                    shortestIndex = i;
                }
            }
        }
        if (shortestIndex == -1)
        {
            ++currentTime;
            continue;
        }
        --processes[shortestIndex].remainingTime;
        ++currentTime;
        if (processes[shortestIndex].remainingTime == 0)
        {
            ++completed;
            processes[shortestIndex].finishTime = currentTime;
            int turnaroundTime = currentTime - processes[shortestIndex].arrivalTime;
            int waitingTime = turnaroundTime - processes[shortestIndex].burstDuration;
            totalTurnaroundTime += turnaroundTime;
            totalWaitingTime += waitingTime;
        }
    }
    double averageTurnaroundTime = static_cast<double>(totalTurnaroundTime) / processes.size();
    double averageWaitingTime = static_cast<double>(totalWaitingTime) / processes.size();
    double throughput = static_cast<double>(processes.size()) / currentTime;
    cout << "--- SJFP ---" << endl;
    cout << "Average Turnaround Time: " << averageTurnaroundTime << endl;
    cout << "Average Waiting Time: " << averageWaitingTime << endl;
    cout << "Throughput: " << throughput << endl;
}
void PriorityScheduling(vector<Process> &processes)
{
    int currentTime = 0;
    size_t completed = 0;
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    while (completed < processes.size())
    {
        int highestPriority = INT_MAX;
        int highestPriorityIndex = -1;
        for (size_t i = 0; i < processes.size(); ++i)
        {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0)
            {
                if (processes[i].priority < highestPriority)
                {
                    highestPriority = processes[i].priority;
                    highestPriorityIndex = i;
                }
            }
        }
        if (highestPriorityIndex == -1)
        {
            int nextArrivalTime = INT_MAX;
            for (const auto &process : processes)
            {
                if (process.arrivalTime > currentTime)
                {
                    nextArrivalTime = min(nextArrivalTime, process.arrivalTime);
                }
            }
            if (nextArrivalTime == INT_MAX)
            {
                break;
            }
            currentTime = nextArrivalTime;
            continue;
        }
        --processes[highestPriorityIndex].remainingTime;
        ++currentTime;
        if (processes[highestPriorityIndex].remainingTime == 0)
        {
            ++completed;
            processes[highestPriorityIndex].finishTime = currentTime;
            int turnaroundTime = currentTime - processes[highestPriorityIndex].arrivalTime;
            int waitingTime = turnaroundTime - processes[highestPriorityIndex].burstDuration;
            totalTurnaroundTime += turnaroundTime;
            totalWaitingTime += waitingTime;
        }
    }
    double averageTurnaroundTime = static_cast<double>(totalTurnaroundTime) / processes.size();
    double averageWaitingTime = static_cast<double>(totalWaitingTime) / processes.size();
    double throughput = static_cast<double>(processes.size()) / currentTime;
    cout << "--- Priority ---" << endl;
    cout << "Average Turnaround Time: " << averageTurnaroundTime << endl;
    cout << "Average Waiting Time: " << averageWaitingTime << endl;
    cout << "Throughput: " << throughput << endl;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <processes file>" << endl;
        return EXIT_FAILURE;
    }
    vector<Process> processes;
    readProcesses(argv[1], processes);
    FCFS(processes);
    for (auto &process : processes)
    {
        process.remainingTime = process.burstDuration;
    }
    SJF(processes);
    for (auto &process : processes)
    {
        process.remainingTime = process.burstDuration;
    }
    PriorityScheduling(processes);
    return EXIT_SUCCESS;
}
