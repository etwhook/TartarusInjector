#pragma once
#include<windows.h>
#include "../utils/dbg.h"
#include "structs.h"


class Injector {
public:
	HANDLE targetProc;
	HANDLE currProc;
	HANDLE heap = GetProcessHeap();


	Injector(HANDLE targetProcess) {
		this->targetProc = targetProcess;
		this->currProc = GetCurrentProcess();
	}

	const wchar_t* irTimer = L"IRTimer";
	const wchar_t* tpWk = L"TpWorkerFactory";


	HANDLE hijackProcessHandle(const wchar_t* objName);

	WORKER_FACTORY_BASIC_INFORMATION getWorkerInfo(HANDLE hWorker);

	bool injectIntoProcess(PVOID nWorkerMem);
	PVOID allocateSc(SIZE_T memSize, ACCESS_MASK memProtection);
	PVOID allocateHeap(SIZE_T size);
	bool writeSc(PVOID baseAddr, PVOID memBuffer, SIZE_T memSize);
	void alertFail(BOOL code, const char* err);
	void cleanHeap(PVOID buffer);
	void closeHandle(HANDLE handle); 
};

