/*********************************************************************************

  Title:        processsched.h
  Author:       Anika Sian
  Created On:   5/3/18
  Description:  The interface file for ProcessSched class.
  Purpose:      Simulates the Multifeedback Queue Scheduling method.
  Build with:   use makefile.

 *********************************************************************************/


#ifndef PROCESSSCHED_H_
#define PROCESSSCHED_H_

#include "process.h"

class ProcessSched {
public:
   /** Constructor for class. Collects the user's input for mem, framesize and number
    *  of disks. Purpose is to set the inputs and create the table.
    * @Pre:  User must give the inputs and they have to be ints.
    * @Post: frameTable size and disk table size has been declared.
    *
    * @Param memory: total number of memory in RAM.
    * @Param pagesize: total number of memory in each frame.
    * @Param disknum: total number of hard disks.
    */
   ProcessSched(long memory, int pagesize, int disknum);



   ////////////////////// FUNCTIONS CALLED ON COMMAND //////////////////////

   /** Add new process to the table and q0. If it is the first process, add it straight to the CPU.
    *
    * @Pre:  None.
    * @Post: Process has been created, added to the table and either added to the CPU or q0.
    */
   void addProcess();

   /** Makes a memory request and assigns the process with a page #.
    *
    * @Pre:  Must be an exisiting process.
    * @Post: Frame table updates with the current CPU process' new page number.
    *
    * @Param requestNum: number requested which will be divided by the frame size.
    */
   void memoryRequest(int requestNum);

   /** Updates the time quantum of the process in the CPU.
    *
    * @Pre:  Must be a process in the CPU.
    * @Post: Either the process stays in the CPU or goes to a lower readyQ.
    */
   void updateTimeQuantum();

   /** Terminate current process in the CPU.
    *
    * @Pre:  Must be process in the CPU.
    * @Post: CPU will either run the next process in the queue or be idle.
    */
   void terminateCurrentProcess();

   /** Process in the CPU makes a hard disk request and must leave the CPU and go to a hard disk.
    *
    * @Pre:  There must be a process in the CPU.
    * @Post: Process is either in its requested hard disk or is in the diskQueue waiting.
    *
    * @Param diskNum: Disk # the process is requesting.
    * @Param filename: filename the process wants to R/W.
    */
   void hardDiskRequest(int diskNum, string filename);

   /** Process is done using the disk and will go back to the queue it belonged to.
    *  Will also look at the waiting queue and put any process requesting the same disk to the diskTable.
    *
    * @Pre:  Process must have been in the Hard disk.
    * @Post: Process is now back in its respective queue and any process in the diskQueue waiting
    *        for the same diskNum will be put into the hard disk.
    *
    * @Param diskNum: Disk # use requested to free up.
    */
   void finishedHardDisk(int diskNum);



   ///////////////////// HELPER FUNCTIONS //////////////////////////

   /** Checks if the CPU is empty.
    *
    * @Pre:  None.
    * @Post: Returns a T/F statement.
    */
   bool isEmpty() const;

   /** Checks if the page# from the "m [MEMORY]" command already exists.
    *
    * @Pre:  None.
    * @Post: Returns true if page exists.
    *
    * @Param pagerequest: Page # the user is requesting.
    */
   bool doesPageExist(int pagerequest) const;

   /** Finds the index of the process with the smallest time stamp.
    *
    * @Pre:  The table must have been full.
    * @Post: Returns the index of frameTable with the smallest time stamp.
    */
   int findIndexOfSmallestTime() const;

   /** Finds the index of the process with the same page name.
    *
    * @Pre:  Page # must exist in the table.
    * @Post: Returns the index of frameTable with the same page #.
    * 
    * @Param pagerequest: Page request from the "m [PAGE-REQUEST]" command.
    */
   int findIndexOfSamePageNum(int pagerequest) const;

   /** Looks through every queue to find the next process to be put into the CPU.
    *
    * @Pre:  User must've terminated the CPU's previous process or the process had a disk request.
    * @Post: CPU pointer is now pointing to the next process from the readyQ or NULL 
    *        if there is no more processes.
    */
   void findNextProcessForCPU();

   /** Checks to see if any Process in the frameTable has a page of -1.
    *
    * @Pre:  None.
    * @Post: Returns true if process of page -1 does exist.
    */
   bool containsNegativeOne() const;

   /** Looks for the index of the first Process with a page# -1 in the frameTable.
    *
    * @Pre:  There must've been a process with a page# -1.
    * @Post: Returns the index of the first process seen with -1.
    */
   int findIndexOfTheFirstNegativeOne() const;

   /** Checks if the hard disk # is currently being used or is free.
    *
    * @Pre:  None.
    * @Post: Returns true if there is a process in that hard disk #.
    *
    * @Param diskNum: Disk# requested by user.
    */
   bool isHardDiskBeingUsed(int diskNum) const;

   /** Checks if the page number of a process already exists in the framTable.
    *
    * @Pre:  None.
    * @Post: Returns true if there is a page number exists.
    *
    * @Param pagerequest: page# requested by user.
    */
   bool pageNumOfProcessExist(int pagerequest) const;

   /** Returns the index of the frametable where the page # exists.
    *
    * @Pre:  The pagenumber existing should exist in the table.
    * @Post: Returns an index of frameTable.
    *
    * @Param pagerequest: page# requested by user.
    */
   int findIndexOfCPUSamePageNum(int pagerequest) const;



   ////////////////////// FUNCTIONS TO DISPLAY DATA ////////////////////

   /** Displays the frameTable and active processes in the table.
    *
    * @Pre:  None.
    * @Post: Displays the processes on the terminal.
    */
   void displayFrameTable();

   /** Displays the diskTable and the processes waiting for a disk.
    *
    * @Pre:  None.
    * @Post: Displays the proceses in disks or waiting on the terminal.
    */
   void displayHardDisk();

   /** Displays all 3 readyQs and the CPU.
    *
    * @Pre:  None.
    * @Post: Displays all processes in the readyQs and the process in the CPU.
    */
   void displayReadyQs();



private: 
   long _ramMem;
   int _frameSize;
   int _diskNum;
   int numOfFrames;

   static int TIME;
   static int PID;

   list<Process*> q0;
   list<Process*> q1;
   list<Process*> q2;
   vector<Process*> frameTable;
   map<int,Process*> diskTable;
   multimap<int,Process*> diskQueue;


   Process * CPU = NULL;
};

#endif // PROCESSSCHED_H_