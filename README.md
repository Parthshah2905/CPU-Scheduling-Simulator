# CPU Scheduling Algorithm Simulator

A C++ console application that simulates and compares three classic CPU scheduling algorithms used in Operating Systems: **FCFS**, **SJF**, and **Round Robin**.

## What it does
- Takes a user-defined list of processes (arrival time and burst time)
- Simulates scheduling using:
  - **First Come First Serve (FCFS)**
  - **Shortest Job First (SJF)**
  - **Round Robin** (with a configurable time quantum)
- Calculates waiting time and turnaround time for each process
- Prints a comparison summary of average waiting/turnaround time across all three algorithms

## Why I built this
CPU scheduling is a core Operating Systems concept. I built this to move from theory to practice — implementing the algorithms myself instead of only reading about them, and comparing their real performance trade-offs (fairness vs. efficiency vs. starvation).

## How to run it
1. Compile: `g++ scheduler.cpp -o scheduler`
2. Run: `scheduler` (or `./scheduler` on Mac/Linux)
3. Enter the number of processes, then each process's arrival time and burst time.

## Concepts used
- Structs, vectors, queues
- Sorting and greedy selection (SJF)
- Queue-based simulation (Round Robin)
- Time complexity and algorithmic trade-offs

## Built by
Parth Shah — B.Tech CS, JECRC University