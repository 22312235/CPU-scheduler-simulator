<div align="center">

# 🖥️ CPU Scheduler Simulator

### CMPE351 – Operating Systems Project

A CPU Scheduling Simulator implemented in **C** that simulates six classical CPU scheduling algorithms. The program reads process information from an input file, executes multiple scheduling strategies, computes waiting times, and generates formatted results in both the terminal and an output file.

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Linux-green.svg)
![Build](https://img.shields.io/badge/Build-Makefile-orange.svg)
![License](https://img.shields.io/badge/Status-Academic_Project-red.svg)

</div>

---

# 📖 Overview

The **CPU Scheduler Simulator** is designed to demonstrate how different CPU scheduling algorithms affect process execution and waiting times.

The simulator:

- 📥 Reads processes from an input text file
- ⚙️ Simulates six scheduling algorithms
- 📊 Calculates waiting time for every process
- 📈 Computes the average waiting time
- 📄 Writes the results to both the console and an output file

---

# ✨ Features

✅ Process Scheduling Simulation

✅ Linked List Process Management

✅ Dynamic Memory Allocation

✅ Command Line Arguments

✅ File Input & Output

✅ Automatic Waiting Time Calculation

✅ Average Waiting Time Computation

✅ Round Robin Time Quantum Support

---

# 🚀 Scheduling Algorithms

The simulator implements the following CPU scheduling algorithms:

| ID | Algorithm |
|----|-----------|
| 1 | First Come First Serve (FCFS) |
| 2 | Shortest Job First (Non-Preemptive) |
| 3 | Shortest Remaining Time First (Preemptive SJF) |
| 4 | Priority Scheduling (Non-Preemptive) |
| 5 | Priority Scheduling (Preemptive) |
| 6 | Round Robin |

---

# 🛠️ Technologies Used

- C Programming Language
- GCC Compiler
- POSIX APIs
- Linux Environment
- Makefile
- Shell Script Testing

---

# 📂 Project Structure

```
.
├── cmpe351.c          # Main scheduler implementation
├── cmpe351.h          # Header file
├── input.txt          # Sample input
├── output.txt         # Generated output
├── Makefile           # Build automation
├── test.sh            # Automated testing
└── README.md          # Documentation
```

---

# 📥 Input Format

Each line represents one process.

```
BurstTime:ArrivalTime:Priority
```

Example:

```
5:0:3
4:1:2
3:1:1
4:2:2
3:3:1
```

---

# 📤 Output Format

Each scheduling algorithm outputs:

```
AlgorithmID:WaitingTime1:WaitingTime2:...:AverageWaitingTime
```

Example:

```
1:0:4:8:10:13:7.00
```

---

# ⚙️ Compilation

Compile the project using:

```bash
make all
```

---

# ▶️ Running the Program

```bash
./cmpe351 -t 2 -f input.txt -o output.txt
```

### Parameters

| Option | Description |
|---------|-------------|
| `-t` | Time Quantum for Round Robin |
| `-f` | Input file |
| `-o` | Output file |

Example:

```bash
./cmpe351 -t 4 -f input.txt -o output.txt
```

---

# 🧪 Running Tests

Execute:

```bash
make test
```

or

```bash
./test.sh
```

The script automatically verifies:

- Program executes successfully
- Output file is generated
- Correct number of output lines
- Basic scheduler validation

---

# 🧠 Internal Design

The simulator uses a linked list to store processes.

Each process contains:

```c
typedef struct Process {
    int pid;
    int burst;
    int arrival;
    int priority;
    int remaining;
    int waiting;
    int completion;
    struct Process *next;
} Process;
```

---

# 📊 Implemented Algorithms

### 🚀 FCFS

Processes execute in arrival order.

---

### ⚡ Shortest Job First (Non-Preemptive)

The shortest available job executes until completion.

---

### 🔄 Shortest Remaining Time First

The scheduler always executes the process with the shortest remaining execution time.

---

### ⭐ Priority Scheduling (Non-Preemptive)

The highest-priority available process executes until completion.

---

### ⭐ Priority Scheduling (Preemptive)

A newly arrived higher-priority process immediately preempts the current one.

---

### 🔁 Round Robin

Processes receive CPU time based on a configurable time quantum.

---

# 📈 Learning Outcomes

This project demonstrates:

- CPU Scheduling Algorithms
- Operating Systems Concepts
- Dynamic Memory Management
- Linked Lists
- File Processing
- Command-Line Programming
- Process Scheduling Simulation
- Performance Evaluation

---

# 💻 Example Command

```bash
make all

./cmpe351 -t 2 -f input.txt -o output.txt

cat output.txt
```

---

# 📌 Requirements

- Linux / macOS
- GCC Compiler
- POSIX-compliant Operating System

---

# 📚 Course

**CMPE351 – Operating Systems**

CPU Scheduling Simulation Project

---

# 👨‍💻 Author

**Hana Shaimi**

Computer Engineering Student

---

<div align="center">

### ⭐ If you found this project helpful, consider giving it a star!

Made with ❤️ in C

</div>
