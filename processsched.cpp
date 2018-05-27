#include "processsched.h"

// Initializing the static private data.
int ProcessSched::TIME = 0;
int ProcessSched::PID = 1;


ProcessSched::ProcessSched(long memory, int pagesize, int disknum) {
   _ramMem = memory;
   _frameSize = pagesize;
   _diskNum = disknum;
   numOfFrames = memory/pagesize;
}


void ProcessSched::addProcess() {
   // CPU is null but there are processes currently in the disk and there are still terminated processes in the frame table.
   if (isEmpty() && containsNegativeOne() && !diskTable.empty()) {
      int index = findIndexOfTheFirstNegativeOne();

      Process * newProcess = new Process;
      newProcess->setProcessProperties(0, "temp", "q0", 0);
      newProcess->setPID(++PID);
      newProcess->setTimeStamp(++TIME);

      CPU = newProcess;  

      frameTable[index] = newProcess;
   }
   else if (isEmpty() &&  !diskTable.empty()) {
      Process * newProcess = new Process;
      newProcess->setProcessProperties(0, "temp", "q0", 0);
      newProcess->setPID(++PID);
      newProcess->setTimeStamp(++TIME);

      CPU = newProcess;

      frameTable.push_back(newProcess);
   }
   // CPU is empty and there are no processes waiting in the readyQ.
   else if (isEmpty()) {
      // If CPU is empty, the new process will automatically go to the CPU.
      Process * newProcess = new Process;

      newProcess->setProcessProperties(0, "temp", "q0", 0);
      newProcess->setPID(PID);
      newProcess->setTimeStamp(++TIME);

      CPU = newProcess;

      frameTable.push_back(newProcess);
   }
   // Frame table is full, must replace the LRU process with a new one.
   else if(frameTable.size() == numOfFrames) {
      /*
         All terminated processes' pageNum is -1 to indicate they have been terminated.
         Although they are terminated, they are still on the frame table but "invisible".
         Will replace the first terminated(processes with -1 pageNum) with the newest process.
      */
      if (containsNegativeOne()) {
         int index = findIndexOfTheFirstNegativeOne();

         Process * newProcess = new Process;
         newProcess->setProcessProperties(0, "temp", "q0", 0);
         newProcess->setPID(++PID);
         newProcess->setTimeStamp(++TIME);

         frameTable[index] = newProcess;

         // Check if the process is a higher priority than the CPU's current process.
         if (CPU->getQueueType() == "q1") {
            q1.push_front(CPU);
            CPU = newProcess;
         }
         else if (CPU->getQueueType() == "q2") {
            q2.push_front(CPU);
            CPU = newProcess;
         }
         else
            q0.push_back(newProcess);
      
      }
      // if the table doesn't contain terminated processes, it is filled with active processes.
      else {
         int index = findIndexOfSmallestTime();

         Process * newProcess = new Process;
         newProcess->setProcessProperties(0, "temp", "q0", 0);
         newProcess->setPID(++PID);
         newProcess->setTimeStamp(++TIME);

         frameTable[index] = newProcess;
        
        // Check if the process is a higher priority than the CPU's current process.
         if (CPU->getQueueType() == "q1") {
            q1.push_front(CPU);
            CPU = newProcess;
         }
         else if (CPU->getQueueType() == "q2") {
            q2.push_front(CPU);
            CPU = newProcess;
         }
         else
            q0.push_back(newProcess);

      }
   }
   // Frame table still has space. Add to the end of the table.
   else {
      Process * newProcess = new Process;
      newProcess->setProcessProperties(0, "temp", "q0", 0);
      newProcess->setPID(++PID);
      newProcess->setTimeStamp(++TIME);

      frameTable.push_back(newProcess);

      // Check if the process is a higher priority than the CPU's current process.
         if (CPU->getQueueType() == "q1") {
            q1.push_front(CPU);
            CPU = newProcess;
         }
         else if (CPU->getQueueType() == "q2") {
            q2.push_front(CPU);
            CPU = newProcess;
         }
         else
            q0.push_back(newProcess);
   }
}


void ProcessSched::memoryRequest(int requestNum) {
   // If CPU has no processes, no memory requests can be made. Return.
   if(CPU == NULL) {
      cout << "CPU is currently idle." << endl << "> ";
      return;
   }

   // Finding the page number.
   int memLoc = requestNum/_frameSize;

   // Frame table is full. Must swap out page number.
   if(frameTable.size() == numOfFrames) {
      if(containsNegativeOne()) {
         int index = findIndexOfTheFirstNegativeOne();

         Process * temp = new Process;
         temp->setProcessProperties(memLoc, CPU->getPID(), CPU->getFileName(), CPU->getQueueType(), CPU->getQuantNum());
         temp->setTimeStamp(++TIME);

         frameTable[index] = temp;
      }
      else if (doesPageExist(memLoc)) {
         int index = findIndexOfSamePageNum(memLoc);
         frameTable.at(index)->setTimeStamp(++TIME);
      }
      else {
         int index = findIndexOfSmallestTime();

         Process * temp = new Process;
         temp->setProcessProperties(memLoc, CPU->getPID(), CPU->getFileName(), CPU->getQueueType(), CPU->getQuantNum());
         temp->setTimeStamp(++TIME);

         frameTable[index] = temp;
      }
   }
   // If page is not full, add it end of the frameTable.
   else {
      if (doesPageExist(memLoc)) {
         int index = findIndexOfSamePageNum(memLoc);
         frameTable.at(index)->setTimeStamp(++TIME);
      }
      else {
         if(containsNegativeOne()) {
            int index = findIndexOfTheFirstNegativeOne();

            Process * temp = new Process;
            temp->setProcessProperties(memLoc, CPU->getPID(), CPU->getFileName(), CPU->getQueueType(), CPU->getQuantNum());
            temp->setTimeStamp(++TIME);

            frameTable[index] = temp;         
         }
         else {
            Process * temp = new Process;
            temp->setProcessProperties(memLoc, CPU->getPID(), CPU->getFileName(), CPU->getQueueType(), CPU->getQuantNum());
            temp->setTimeStamp(++TIME);

            frameTable.push_back(temp);
         }
      }
   }
}

bool ProcessSched::pageNumOfProcessExist(int pagerequest) const {
   int pageNum = CPU->getPageNum();
   for (int i = 0; i < frameTable.size(); i++) {
      if((frameTable.at(i)->getPID() == CPU->getPID()) && (pagerequest = pageNum))
         return true;
   }
   return false;
}

int ProcessSched::findIndexOfCPUSamePageNum(int pagerequest) const {
   int index = 0;
   int pageNum = CPU->getPageNum();
   for (int i = 0; i < frameTable.size(); i++) {
      if((frameTable.at(i)->getPID() == CPU->getPID()) && (pagerequest = pageNum)) {
         index = i;
         break;
      }
   }
   
   return index;
}

void ProcessSched::updateTimeQuantum() {
   int curr = CPU->getQuantNum();
   string queue = CPU->getQueueType();

   // If CPU's current process was from q0 w/ single time quantum.
   if (queue == "q0") {
      CPU->setQuantNum(0);
      CPU->setQueueType("q1");
      q1.push_back(CPU);
      findNextProcessForCPU();
   }
   // If CPU's current process was from q1 w/ double time quantum.
   else if (queue == "q1") {
      if(curr == 0)
         CPU->setQuantNum(1);
      else if(curr == 1) {
         CPU->setQuantNum(0);
         CPU->setQueueType("q2");
         q2.push_back(CPU);
         findNextProcessForCPU();
      }
   }
   // If Process was from q2, it's FCFS. It'll keep going till a termination is requested.
}


void ProcessSched::terminateCurrentProcess() {
   if(CPU == NULL) {
      cout << "CPU is idle." << endl;
      return;
   }

   int curr_pid = CPU->getPID();

   // Removing all of the memory associated with the process.
   for (int i = 0; i < frameTable.size(); i++) {
      if (frameTable.at(i)->getPID() ==  curr_pid) {
         frameTable.at(i)->setPageNum(-1);
         frameTable.at(i)->setTimeStamp(-1);
      }
   }

   findNextProcessForCPU();
}


void ProcessSched::hardDiskRequest(int diskNum, string filename) {
   if (CPU == NULL) {
      cout << "CPU is idle." << endl;
      return;
   }

   // int numOfProcesses = q0.size() + q1.size() + q2.size();

   // Setting file name for the Process.
   CPU->setFileName(filename);

   // If there is no more hard disks available, put it in a waiting queue.
   if (diskTable.size() == _diskNum) {
      diskQueue.insert(make_pair(diskNum, CPU));
      findNextProcessForCPU();
   }
   else if(diskNum > _diskNum) {
      cout << "There are only " << _diskNum << " disks. \n> ";
   }
   // There are available hard disks.
   else {
      // If the current disk# is being used, add it to the queue.
      if (isHardDiskBeingUsed(diskNum)) {
         diskQueue.insert(make_pair(diskNum, CPU));
         findNextProcessForCPU();
      }
      else {
         diskTable.insert(make_pair(diskNum, CPU));
         findNextProcessForCPU();
      }
   }
}


void ProcessSched::finishedHardDisk(int diskNum) {
   map<int, Process*>::iterator alto = diskTable.find(diskNum);

   // User entered a disk larger than the number of assigned disks.
   if(diskNum > _diskNum - 1) {
      cout << "There are only " << _diskNum << " disks. \n> ";
      return;
   }

   // Disk has not been used.
   if(alto == diskTable.end()) {
      cout << "Disk is already available.\n> ";
      return;
   }


   // Setting filename back to temp which means there is no file associated with the process.
   alto->second->setFileName("temp");

   // Returning the Process back to its respective queue.
   if(alto->second->getQueueType() == "q0") {
      alto->second->setQuantNum(0);
      if(CPU->getQueueType() == "q0") {
         q0.push_front(CPU);
         CPU = alto->second;
      }
      if(CPU->getQueueType() == "q1") {
         q1.push_front(CPU);
         CPU = alto->second;
      }
      if(CPU->getQueueType() == "q2") {
         q2.push_front(CPU);
         CPU = alto->second;
      }
   }
   else if(alto->second->getQueueType() == "q1") {
      alto->second->setQuantNum(0);
      if(CPU->getQueueType() == "q0") {
         q0.push_front(CPU);
         CPU = alto->second;
      }
      if(CPU->getQueueType() == "q1") {
         q1.push_front(CPU);
         CPU = alto->second;
      }
      if(CPU->getQueueType() == "q2") {
         q2.push_front(CPU);
         CPU = alto->second;
      }
   }
   else if(alto->second->getQueueType() == "q2") {
      alto->second->setQuantNum(0);
      if(CPU->getQueueType() == "q0") {
         q0.push_front(CPU);
         CPU = alto->second;
      }
      if(CPU->getQueueType() == "q1") {
         q1.push_front(CPU);
         CPU = alto->second;
      }
      if(CPU->getQueueType() == "q2") {
         q2.push_front(CPU);
         CPU = alto->second;
      }
   }


   // Erase Process from the disk table.
   diskTable.erase(alto);

   // Checking if any waiting processes in the diskqueue is able to go to its requested disk.
   if(!diskQueue.empty()) {
      for(multimap<int,Process*>::iterator iter = diskQueue.begin(); iter != diskQueue.end();) {
         if(!isHardDiskBeingUsed(iter->first)) {
            diskTable.insert(make_pair(iter->first, iter->second));
            // Make iter point to the next item after the erased one.
            iter = diskQueue.erase(iter);
         }
         else
            // If the disk is being used, move forward.
            ++iter;
      }
   }
}


bool ProcessSched::isEmpty() const {
   if (CPU == NULL)
      return true;
   else
      return false;
}


bool ProcessSched::doesPageExist(int pagerequest) const {
   for (int i = 0; i < frameTable.size(); ++i) {
      if(pagerequest == frameTable.at(i)->getPageNum())
         return true;
   }
   return false;

}


int ProcessSched::findIndexOfSmallestTime() const {
   vector<int>temp;

   for (int i = 0; i < frameTable.size(); ++i) {
      // Ignoring dead/terminated process whose page#s have been turned to -1
      if(frameTable.at(i)->getPageNum() != -1)
         temp.push_back(frameTable.at(i)->getTimeStamp());
   }

   int min_pos = distance(temp.begin(),min_element(temp.begin(),temp.end()));

   return min_pos;
}


int ProcessSched::findIndexOfSamePageNum(int pagerequest) const {
   int index = 0;

   for (int i = 0; i < frameTable.size(); ++i) {
      // Ignoring dead/terminated process whose page#s have been turned to -1
      if(frameTable.at(i)->getPageNum() != -1)
         if(pagerequest == frameTable.at(i)->getPageNum()) {
            index = i;
            break;
         }
   }

   return index;
}


void ProcessSched::findNextProcessForCPU() {
   if(!q0.empty()) {
      CPU = q0.front();
      q0.pop_front();
   }
   else if(!q1.empty()) {
      CPU = q1.front();
      q1.pop_front();
   }
   else if(!q2.empty()) {
      CPU = q2.front();
      q2.pop_front();
   }
   else {
      CPU = NULL;
   }
}

bool ProcessSched::containsNegativeOne() const {
   for (int i = 0; i < frameTable.size(); ++i) {
      if(frameTable.at(i)->getPageNum() == -1)
         return true;
   }
   return false;
}

int ProcessSched::findIndexOfTheFirstNegativeOne() const {
   int index = -1;
   for (int i = 0; i < frameTable.size(); ++i) {
      if(frameTable.at(i)->getPageNum() == -1) {
         index = i;
         break;
      }
   }

   return index;
}

bool ProcessSched::isHardDiskBeingUsed(int diskNum) const {
   map<int,Process*>::const_iterator it = diskTable.find(diskNum);

   // If it returns diskTable.end(), it is not available.
   if(it != diskTable.end())
      return true;
   else
      return false;
}


void ProcessSched::displayFrameTable() {
   cout << endl;
   cout << "+–––––––––––+––––––––––+–––––––––––+–––––––––––+" << endl;
   cout << "|  Frame #  |  Page #  |    PID    | Timestamp |" << endl;
   cout << "+–––––––––––+––––––––––+–––––––––––+–––––––––––+" << endl;

   if (frameTable.empty()) {
      for (int i = 0; i < numOfFrames; i++) {
         cout << "|" << setw(11) <<  i << "| " << setw(10) 
                 << "|" << setw(12) << "|" << setw(12) <<  "|" 
                 << endl;
      }
   }
   else {
      for (int i = 0; i < frameTable.size(); ++i) {
         if (frameTable.at(i)->getPageNum() != -1) {
            cout << "| " << left << setw(10) <<  i << "| " << left << setw(9) << frameTable.at(i)->getPageNum()
                 << "| " << left << setw(10) << frameTable.at(i)->getPID()
                 << "| " << left << setw(10) << frameTable.at(i)->getTimeStamp() << "|" 
                 << endl;
         }
      }
   }
   cout << "+–––––––––––+––––––––––+–––––––––––+–––––––––––+" << endl;
   cout << "> ";
}


void ProcessSched::displayHardDisk() {
   cout << endl;
   cout << "+––––––––––+–––––––+––––––––––––––+" << endl;
   cout << "|  Disk #  |  PID  |   Filename   |" << endl;
   cout << "+––––––––––+–––––––+––––––––––––––+" << endl;


   if (diskTable.empty())
      cout << "|–––– No processes using a disk ––|" << endl;
   else {
      for (map<int,Process*>::iterator it = diskTable.begin(); it != diskTable.end(); ++it) {
         if (it->second != NULL)
            cout << "| " << left << setw(9) << (*it).first 
                 << "| " << left << setw(6) << (*it).second->getPID() 
                 << "| " << left << setw(13) << (*it).second->getFileName() << "|" << endl;
      }
   }
   cout << "+––––––––––+–––––––+––––––––––––––+" << endl;

   if(diskQueue.empty())
      cout << " No waiting processes for disk." << endl;
   else {
      cout << " Waiting processes:" << endl;
      for (multimap<int,Process*>::iterator it = diskQueue.begin(); it != diskQueue.end(); ++it) {
         if (it->second != NULL)
            cout << "  P" << (*it).second->getPID() << " with file \'" << (*it).second->getFileName() << "\' is waiting for Disk#" << (*it).first << ".\n";
      }
   }

   cout << "> ";
}


void ProcessSched::displayReadyQs() {
   if (CPU == NULL)  {
      cout << endl;
      cout << "CPU is idle." << endl;
   }
   else {
      cout << endl;
      cout << "   CPU" << endl;
      cout << "+––––––––+" << endl;
      cout << "| " << left  << CPU->getPID() << " (" <<  CPU->getQueueType() <<  ")" << setw(4) << " |" << endl;
      cout << "+––––––––+" << endl;
   }

   // Reverse for display purposes. Want to queue to pop -----> instead of the usual <-----.
   q0.reverse();
   q1.reverse();
   q2.reverse();
   cout << " Queue 0: ";
   for(list<Process*>::iterator it = q0.begin(); it != q0.end(); it++) 
      cout << "[ P" << (*it)->getPID() << " ]-->";

   cout << endl;
   cout << " Queue 1: ";
   for(list<Process*>::iterator it = q1.begin(); it != q1.end(); it++)
      cout << "[ P" << (*it)->getPID() << " ]-->";

   cout << endl;
   cout <<  " Queue 2: ";
   for(list<Process*>::iterator it = q2.begin(); it != q2.end(); it++)
      cout << "[ P" << (*it)->getPID() << " ]-->";

   // Reversing back to original.
   q0.reverse();
   q1.reverse();
   q2.reverse();
   cout << endl;
   cout <<  "> ";
}





