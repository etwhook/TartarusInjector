#pragma once
#include<windows.h>
#include<TlHelp32.h>
#include "../syscalls/Syscalls.h"
namespace utils {
	DWORD findPID(const char* processName) {
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		Process32First(snapshot, &pe32);
		while (Process32Next(snapshot, &pe32)) {
			LPCSTR procName = (LPCSTR)pe32.szExeFile;
			if (strcmp(processName, procName) == 0) {
				CloseHandle(snapshot);
				return pe32.th32ProcessID;
			}
		}
		CloseHandle(snapshot);
		return -1;
	}

	HANDLE findProcess(const char* processName) {
		DWORD procID = findPID(processName);
		if (procID == -1)
			return INVALID_HANDLE_VALUE;
		HANDLE hProc;
		OBJECT_ATTRIBUTES objAtt = { 0 };
		CLIENT_ID cid = { 0 };
		cid.UniqueProcess = (HANDLE)(procID);
		cid.UniqueThread = 0;

		Sw3NtOpenProcess(
			&hProc,
			PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION | PROCESS_DUP_HANDLE,
			&objAtt,
			&cid
		);
		return hProc;
	}

	bool isValidHandle(HANDLE handle) {
		if (handle == 0 || handle == INVALID_HANDLE_VALUE)
			return false;
		else
			return true;
	}
}