#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// This struct represents one process (one task the CPU needs to run)
struct Process {
    int id;              // Process name/number, e.g. P1, P2
    int arrivalTime;     // When the process arrives, ready to run
    int burstTime;       // How long the process needs the CPU for
    int waitingTime;     // How long it waited before getting the CPU
    int turnaroundTime;  // Total time from arrival to finishing
};

// FCFS = First Come First Serve: processes run in the order they arrive
void fcfs(vector<Process>& processes) {
    int n = processes.size();
    int currentTime = 0;

    for (int i = 0; i < n; i++) {
        // If CPU is free before this process arrives, CPU waits
        if (currentTime < processes[i].arrivalTime) {
            currentTime = processes[i].arrivalTime;
        }

        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        currentTime += processes[i].burstTime;
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
    }
}

// SJF = Shortest Job First: among arrived processes, always run the shortest one next
void sjf(vector<Process>& processes) {
    int n = processes.size();
    vector<bool> isDone(n, false);  // tracks which processes have already run
    int completed = 0, currentTime = 0;

    while (completed < n) {
        int idx = -1;
        int minBurst = 1e9;  // a very large number to start comparison

        // Find the process with shortest burst time among those that have arrived and not yet done
        for (int i = 0; i < n; i++) {
            if (!isDone[i] && processes[i].arrivalTime <= currentTime) {
                if (processes[i].burstTime < minBurst) {
                    minBurst = processes[i].burstTime;
                    idx = i;
                }
            }
        }

        // If no process has arrived yet, move time forward
        if (idx == -1) {
            currentTime++;
            continue;
        }

        processes[idx].waitingTime = currentTime - processes[idx].arrivalTime;
        currentTime += processes[idx].burstTime;
        processes[idx].turnaroundTime = processes[idx].waitingTime + processes[idx].burstTime;

        isDone[idx] = true;
        completed++;
    }
}

// Round Robin: each process gets a fixed time slice (quantum) in turn
void roundRobin(vector<Process>& processes, int quantum) {
    int n = processes.size();
    vector<int> remaining(n);
    for (int i = 0; i < n; i++) remaining[i] = processes[i].burstTime;

    queue<int> q;
    int currentTime = 0;
    int completed = 0;
    int nextToArrive = 0; // tracks which process (by arrival order) joins the queue next

    // Start the clock when the first process arrives, and add it to the queue
    if (n > 0) {
        currentTime = processes[0].arrivalTime;
        q.push(0);
        nextToArrive = 1;
    }

    while (completed < n) {
        int idx = q.front();
        q.pop();

        int runTime = min(quantum, remaining[idx]);
        currentTime += runTime;
        remaining[idx] -= runTime;

        // Add any newly arrived processes to the queue before deciding what's next
        while (nextToArrive < n && processes[nextToArrive].arrivalTime <= currentTime) {
            q.push(nextToArrive);
            nextToArrive++;
        }

        if (remaining[idx] > 0) {
            q.push(idx); // not done yet, goes to the back of the line
        } else {
            processes[idx].turnaroundTime = currentTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            completed++;
        }
    }
}

void printResults(vector<Process>& processes) {
    float totalWT = 0, totalTAT = 0;
    int n = processes.size();

    cout << "\nProcess\tArrival\tBurst\tWaiting\tTurnaround\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << processes[i].id << "\t"
             << processes[i].arrivalTime << "\t"
             << processes[i].burstTime << "\t"
             << processes[i].waitingTime << "\t"
             << processes[i].turnaroundTime << "\n";
        totalWT += processes[i].waitingTime;
        totalTAT += processes[i].turnaroundTime;
    }

    cout << "\nAverage Waiting Time: " << (totalWT / n);
    cout << "\nAverage Turnaround Time: " << (totalTAT / n) << "\n";
}

void compareAlgorithms(vector<Process>& fcfsP, vector<Process>& sjfP, vector<Process>& rrP) {
    auto avgWT = [](vector<Process>& p) {
        float total = 0;
        for (auto& proc : p) total += proc.waitingTime;
        return total / p.size();
    };
    auto avgTAT = [](vector<Process>& p) {
        float total = 0;
        for (auto& proc : p) total += proc.turnaroundTime;
        return total / p.size();
    };

    cout << "\n=== Comparison Summary ===\n";
    cout << "Algorithm\tAvg Waiting\tAvg Turnaround\n";
    cout << "FCFS\t\t" << avgWT(fcfsP) << "\t\t" << avgTAT(fcfsP) << "\n";
    cout << "SJF\t\t" << avgWT(sjfP) << "\t\t" << avgTAT(sjfP) << "\n";
    cout << "Round Robin\t" << avgWT(rrP) << "\t\t" << avgTAT(rrP) << "\n";
}

int main() {
    int n;
    cout << "=== CPU Scheduling Algorithm Simulator ===\n";
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> original(n);
    for (int i = 0; i < n; i++) {
        original[i].id = i + 1;
        cout << "\nProcess P" << (i + 1) << ":\n";
        cout << "  Arrival Time: ";
        cin >> original[i].arrivalTime;
        cout << "  Burst Time: ";
        cin >> original[i].burstTime;
        original[i].waitingTime = 0;
        original[i].turnaroundTime = 0;
    }

    vector<Process> forFCFS = original;
    vector<Process> forSJF = original;
    vector<Process> forRR = original;

    cout << "\n=== FCFS Scheduling ===\n";
    fcfs(forFCFS);
    printResults(forFCFS);

    cout << "\n=== SJF Scheduling ===\n";
    sjf(forSJF);
    printResults(forSJF);

    cout << "\n=== Round Robin Scheduling (Quantum = 4) ===\n";
    roundRobin(forRR, 4);
    printResults(forRR);

    compareAlgorithms(forFCFS, forSJF, forRR);

    return 0;
}