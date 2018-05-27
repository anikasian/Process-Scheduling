#include "processsched.h"

int main () {
   string input, commands;
	int frameSize, diskNum;
   long ramMem, MAX = 4000000000000;

   cout << "Size of memory: ";
   // check if ramMem is an int
   while (true) {
      getline(cin, input);
      stringstream ss(input);
      if (ss >> ramMem) {
         // check if ramMem is less than MAX.
         while (ramMem > MAX) {
            cout << "Must be less than 4 billion bytes. Please enter again: ";
            cin.clear();
            cin >> ramMem;
            if (ramMem < MAX)
               break;
         }
         break;
      }
      cin.clear();
      cout << "Invalid number. Please try again: ";
   }

   // clearing input
   cin.clear();

   cout << "Frame size:  ";
   while (true) {
      getline(cin, input);
      stringstream ss(input);
      if (ss >> frameSize) {
         while (frameSize > ramMem) {
            cout << "Frame size must be less than RAM size. Please enter again: ";
            cin.clear();
            cin >> frameSize;
            if(frameSize < ramMem)
               break;
         }
         break;
      }
      cin.clear();
      cout << "Invalid number. Please try again: ";
   }

   cin.clear();

   cout << "Number of hard disks: ";
   while (true) {
      getline(cin, input);
      stringstream ss(input);
      if (ss >> diskNum)
         break;
      cout << "Invalid number. Please try again: ";
      cin.clear();
   }

   // Start simulator.
   ProcessSched simulator(ramMem, frameSize, diskNum);
   

   while(true) {
      cout << "> ";
      while (getline(cin, commands)) {
         cout << "> ";
         int numRequest;
         string fileName, firstRequest;

         string d_num_File("d ");
         string D_num("D ");
         string m_add("m ");      

         // Creates new process.
         if (commands == "A") {
            simulator.addProcess();            
         }

         // The currently running process has spent a time quantum using the CPU.
         else if(commands == "Q") {
            simulator.updateTimeQuantum();
         }

         // Current process in the CPU is terminated.
         else if(commands == "t") {
            simulator.terminateCurrentProcess();
         }

         // Creating a harddisk request.
         else if(commands.find(d_num_File) != string::npos) {
            istringstream ss(commands);
            ss >> firstRequest;
            ss >> numRequest;
            ss >> fileName;
            simulator.hardDiskRequest(numRequest, fileName);
         }

         // The hard disk #number has finished the work for one process.
         else if(commands.find(D_num) != string::npos) {
            istringstream ss(commands);
            ss >> firstRequest;
            ss >> numRequest;

            simulator.finishedHardDisk(numRequest);
         }

         else if(commands.find(m_add) != string::npos) {
            istringstream ss(commands);
            ss >> firstRequest;
            ss >> numRequest;
            simulator.memoryRequest(numRequest);
         }
         else if(commands == "S r") {
            simulator.displayReadyQs();
         }
         else if(commands == "S i") {
            simulator.displayHardDisk();
         }
         else if(commands == "S m") {
            simulator.displayFrameTable();
         }
         else {
            cout << "Wrong Input." << endl;
            break;
         }

      }
   }

   return 0;

}