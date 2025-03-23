#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <fstream>
using namespace std;

// ������̽ṹ�壬����������Ϣ�ͼ���ָ��
struct Process {
    int id, arrival, burst, remaining, priority;
    int completion, waiting, turnaround, response;
    bool started;
};

// ���㲢�����㷨���к������ָ��
void calculateMetricsAndSave(vector<Process>& processes, int totalTime, const string& algorithmName, ofstream& csvFile) {
    double totalWaiting = 0, totalTurnaround = 0, totalResponse = 0;
    int n = processes.size();

    for (auto& p : processes) {
        p.turnaround = p.completion - p.arrival;
        p.waiting = p.turnaround - p.burst;
        totalWaiting += p.waiting;
        totalTurnaround += p.turnaround;
        totalResponse += p.response;
    }

    double jobThroughput = (double)n / totalTime;
    double cpuUtilization = 100.0; // ����CPUʼ������
    double avgTurnaround = totalTurnaround / n;
    double avgResponse = totalResponse / n;
    double avgWaiting = totalWaiting / n;

    // ����̨������
    cout << algorithmName << ":" << endl;
    cout << "Job Throughput: " << jobThroughput << endl;
    cout << "CPU Utilization: " << cpuUtilization << "%" << endl;
    cout << "Average Turnaround Time: " << avgTurnaround << endl;
    cout << "Average Response Time: " << avgResponse << endl;
    cout << "Average Waiting Time: " << avgWaiting << endl;

    // ��������浽CSV�ļ�
    csvFile << algorithmName << "," << jobThroughput << "," << cpuUtilization << "," << avgTurnaround << "," << avgResponse << "," << avgWaiting << endl;
}

// FCFS
void fcfs(vector<Process> processes, ofstream& csvFile) {
    int time = 0;
    for (auto& p : processes) {
        if (time < p.arrival)
            time = p.arrival;
        p.response = time - p.arrival;
        time += p.burst;
        p.completion = time;
    }
    calculateMetricsAndSave(processes, time, "FCFS", csvFile);
}

// SJF
void sjf(vector<Process> processes, ofstream& csvFile) {
    int completed = 0, time = 0;
    int n = processes.size();

    while (completed < n) {
        int idx = -1, minBurst = INT_MAX;
        // ���ҵ�ǰ�����ҵ
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= time && processes[i].remaining > 0 && processes[i].burst < minBurst) {
                minBurst = processes[i].burst;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
        }
        else {
            processes[idx].response = time - processes[idx].arrival;
            time += processes[idx].burst;
            processes[idx].completion = time;
            processes[idx].remaining = 0;
            completed++;
        }
    }
    calculateMetricsAndSave(processes, time, "SJF", csvFile);
}

// SRTF
void srtf(vector<Process> processes, ofstream& csvFile) {
    int completed = 0, time = 0;
    int n = processes.size();

    while (completed < n) {
        int idx = -1, minRemain = INT_MAX;
        // ����ʣ��ʱ����̵Ľ���
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= time && processes[i].remaining > 0 && processes[i].remaining < minRemain) {
                minRemain = processes[i].remaining;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
        }
        else {
            if (!processes[idx].started) {
                processes[idx].response = time - processes[idx].arrival;
                processes[idx].started = true;
            }
            processes[idx].remaining--;
            time++;

            if (processes[idx].remaining == 0) {
                processes[idx].completion = time;
                completed++;
            }
        }
    }
    calculateMetricsAndSave(processes, time, "SRTF", csvFile);
}

// RR
void rr(vector<Process> processes, int quantum, ofstream& csvFile) {
    int time = 0;
    int n = processes.size();
    vector<int> queue;
    vector<bool> visited(n, false);

    queue.push_back(0);
    visited[0] = true;

    while (!queue.empty()) {
        int idx = queue.front();
        queue.erase(queue.begin());

        if (!processes[idx].started) {
            processes[idx].response = time - processes[idx].arrival;
            processes[idx].started = true;
        }

        int runTime = min(processes[idx].remaining, quantum);
        processes[idx].remaining -= runTime;
        time += runTime;

        // ������Ľ��̼������
        for (int i = 0; i < n; i++) {
            if (!visited[i] && processes[i].arrival <= time && processes[i].remaining > 0) {
                queue.push_back(i);
                visited[i] = true;
            }
        }

        if (processes[idx].remaining > 0) {
            queue.push_back(idx);
        }
        else {
            processes[idx].completion = time;
        }
    }
    calculateMetricsAndSave(processes, time, "RR (Q=3)", csvFile);
}

// ���ȼ������㷨
void priorityScheduling(vector<Process> processes, ofstream& csvFile) {
    int completed = 0, time = 0;
    int n = processes.size();

    while (completed < n) {
        int idx = -1, highestPriority = INT_MAX;
        // ����������ȼ��Ľ���
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= time && processes[i].remaining > 0 && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
        }
        else {
            processes[idx].response = time - processes[idx].arrival;
            time += processes[idx].burst;
            processes[idx].completion = time;
            processes[idx].remaining = 0;
            completed++;
        }
    }
    calculateMetricsAndSave(processes, time, "Priority", csvFile);
}

int main() {
    // ��ʼ����������
    vector<Process> processes = {
        {1, 0, 8, 8, 2, 0, 0, 0, 0, false},
        {2, 1, 4, 4, 1, 0, 0, 0, 0, false},
        {3, 2, 9, 9, 3, 0, 0, 0, 0, false},
        {4, 3, 5, 5, 2, 0, 0, 0, 0, false}
    };

    // ��CSV�ļ����ڱ�����
    ofstream csvFile("cpu_metrics.csv");
    csvFile << "Algorithm,Job Throughput,CPU Utilization,Average Turnaround Time,Average Response Time,Average Waiting Time" << endl;

    // �ֱ�ִ�и��㷨����¼���
    fcfs(processes, csvFile);
    for (auto& p : processes) { p.remaining = p.burst; p.started = false; }

    sjf(processes, csvFile);
    for (auto& p : processes) { p.remaining = p.burst; p.started = false; }

    srtf(processes, csvFile);
    for (auto& p : processes) { p.remaining = p.burst; p.started = false; }

    rr(processes, 3, csvFile);
    for (auto& p : processes) { p.remaining = p.burst; p.started = false; }

    priorityScheduling(processes, csvFile);

    csvFile.close();

    cout << "\nʵ�����ѱ��浽 cpu_metrics.csv �ļ��С�" << endl;

    return 0;
}
