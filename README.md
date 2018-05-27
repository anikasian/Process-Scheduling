# Process-Scheduling
Simulation of process and disk scheduling.

## Compiling & Running
Use makefile. Type: `make`

OS COMPILED IN:
- macOS Sierra 10.12
- Ubuntu 16.04

## How it works
Execute the program by typing: `./ProcessSched`. <br>
You are then prompted to type in the memory size, frame size and disk number. The maximum memory size is 4e12. Frame size must be less than memory size.<br>
<br>There are only 3 queues in the simulation and each follows its own scheduling method:<br>
**Queue 0:** RR with single-time quantum. <br>
**Queue 1:** RR with double-time quantum. <br>
**Queue 2:** First come first serve. <br>
### List of Commands
**A** - Creates a new process<br>
**t** - Terminates the current process in the CPU<br>
**m** *[MEMORY-REQUEST-NUMBER]* - Current process in CPU makes a memory request and is assigned to a page #.<br>
**d** *[DISK-NUMBER] [FILE-NAME]* - Removes the current process in the CPU to a disk. If the disk is busy, the process is in a waiting queue.<br>
**D** *[DISK-NUMBER]* - Process is done using the given disk number and goes back into the ready queue or CPU. <br>
**S m** - Displays the frame table. <br>
**S r** - Displays the 3 ready queues and the current process in CPU. <br>
**S i** - Displays the disk numbers and disk waiting queue.
