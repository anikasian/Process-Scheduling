#include "process.h"


void Process::setProcessProperties(int pagenum, string filename, string queuetype, int quantnum) {
   _pageNum = pagenum;
   _fileName = filename;
   _queueType = queuetype;
   _quantNum = quantnum;
}

void Process::setProcessProperties(int pagenum, int pid, string filename, string queuetype, int quantnum) {
   _pageNum = pagenum;
   _pid = pid;
   _fileName = filename;
   _queueType = queuetype;
   _quantNum = quantnum;
}

void Process::setPID(int pid) {
   _pid = pid;
}

void Process::setTimeStamp(int newTime) { 
   _timeStamp = newTime;
}


void Process::setPageNum(int newNum) {
   _pageNum = newNum;
}


void Process::setFileName(string newfile) {
   _fileName = newfile;
}


void Process::setQueueType(string queuetype) {
   _queueType = queuetype;
}


void Process::setQuantNum(int quantnum) {
   _quantNum = quantnum;
}