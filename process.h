/*********************************************************************************

  Title:        process.h
  Author:       Anika Sian
  Created On:   5/3/18
  Description:  The interface file for Process class.
  Purpose:      Creates a process and assigns the properties of the process.
  Build with:   use makefile.
  
 *********************************************************************************/



#ifndef PROCESS_H_
#define PROCESS_H_

#include <iostream>
#include <list>
#include <sstream>
#include <algorithm>
#include <ctype.h>
#include <vector>
#include <iomanip>
#include <map>
using namespace std;


class Process {
public:

   /** Constructor for Process.
    *
    * @Pre:  None.
    * @Post: Creates an object of type Process.
    */
   Process() {}
   
   ~Process() {}


   ///////////////////////////////// SETTERS ////////////////////////////////////

   /** Sets the properties of a process.
    *
    * @Pre:  Object must have been created.
    * @Post: Assigns all of the private data of the process.
    *
    * @Param pagenum: Page# of the process. All new processes are assigned to Page# 0.
    * @Param filename: Filename the process wants to R/W to.
    * @Param queuetype: The queue the process is currently in.
    * @Param quantnum: The time quantum the process has spent in the CPU.
    */
   void setProcessProperties(int pagenum, string filename, string queuetype, int quantnum);

   /** Sets the properties of a process. This is used when I need to grab properties for a temp.
    *
    * @Pre:  Object must have been created.
    * @Post: Assigns all of the private data of the process.
    *
    * @Param pagenum: Page# of the process. All new processes are assigned to Page# 0.
    * @Param pid: Process ID.
    * @Param filename: Filename the process wants to R/W to.
    * @Param queuetype: The queue the process is currently in.
    * @Param quantnum: The time quantum the process has spent in the CPU.
    */
   void setProcessProperties(int pagenum, int pid, string filename, string queuetype, int quantnum);

   /** Setting the PID of a process.
    *
    * @Pre:  None.
    * @Post: Changes the PID.
    *
    * @Param pid: Newest pid to be changed to.
    */
   void setPID(int pid);
   
   /** Setting the timestamp of a process.
    *
    * @Pre:  None.
    * @Post: Changes the timestamp.
    *
    * @Param newtime: Newest time to be changed to.
    */
   void setTimeStamp(int newtime);

   /** Setting the page# of a process.
    *
    * @Pre:  None.
    * @Post: Changes the page#.
    *
    * @Param newnum: Newest page# to be changed to.
    */
   void setPageNum(int newnum);

   /** Setting the filename the process wants to R/W.
    *
    * @Pre:  None.
    * @Post: Changes the "temp" to the newest filename.
    *
    * @Param newfile: Newest filename to be changed to.
    */
   void setFileName(string newfile);

   /** Setting the queue type (q0, q1, q2) of a process.
    *
    * @Pre:  None.
    * @Post: Changes the queuetype of a process.
    *
    * @Param queuetype: Newest queutype to be changed to.
    */
   void setQueueType(string queuetype);

   /** Setting the time quantum of a process in CPU.
    *
    * @Pre:  None.
    * @Post: Changes the time quantum of the process.
    *
    * @Param quantnum: Newest timequantum to be changed.
    */
   void setQuantNum(int quantnum);



   ///////////////////////////////// GETTERS ////////////////////////////////////

   /** Returns the PID of the process.
    *
    * @Pre:  None.
    * @Post: Returns _pid.
    */
   int getPID() const { return _pid; }

   /** Returns the timestamp of the process.
    *
    * @Pre:  None.
    * @Post: Returns _timeStamp.
    */
   int getTimeStamp() const { return _timeStamp; }

   /** Returns the pagenum of a process.
    *
    * @Pre:  None.
    * @Post: Returns _pageNum.
    */
   int getPageNum() const { return _pageNum; }

   /** Returns the filename of a process.
    *
    * @Pre:  None.
    * @Post: Returns _fileName.
    */
   string getFileName() const { return _fileName; }

   /** Returns the queuetype of a process.
    *
    * @Pre:  None.
    * @Post: Returns _queueType.
    */
   string getQueueType() const { return _queueType; }

   /** Returns the quantumNum of a process.
    *
    * @Pre:  None.
    * @Post: Returns _quantNum.
    */
   int getQuantNum() const { return _quantNum; }

private:
   int _pageNum;
   int _pid;
   int _timeStamp;
   string _fileName;
   string _queueType;
   int _quantNum;
};

#endif // PROCESS_H_ 