#pragma warning(disable : 4996)

#include <Windows.h>
#include <TlHelp32.h>
#include <io.h>
#include <stdio.h> 

DWORD FindProcessId(const WCHAR* processname);
int InjectDllToTargetProcess(int pid, const WCHAR* dllPath);
int FileExist(const WCHAR* name);


int main() {
	WCHAR process_name[32] = TEXT("mspaint.exe");
	WCHAR dll_name[32] = TEXT("newSampleDll.dll");
	WCHAR path[MAX_PATH];

	//printf("Enter process name: ");
	//scanf("%ls", process_name);

	DWORD pID = FindProcessId(process_name);

	printf("Waiting %ls for start...\n", process_name);
	while (pID == 0) {
		Sleep(50);
		pID = FindProcessId(process_name);
	}
	printf("%ls found (pid = %d)\n", process_name, pID);

	while (!FileExist(path)) {
		//printf("Enter DLL name: ");
		//scanf("%ls", dll_name);

		GetFullPathName(dll_name, MAX_PATH, path, NULL);
		if (FileExist(path)) {
			printf("DLL found!\n");
			printf("Its path: %ls\n", path);
		} else {
			printf("DLL not found\n");
		}
	}

	if (InjectDllToTargetProcess(pID, path) == 0) {
		printf("DLL successfully injected!\n");
	} else {
		printf("Something went wrong while injectng attempt\n");
	}

	system("pause");
	return 0;
}

DWORD FindProcessId(const WCHAR* processname) {
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	DWORD result = NULL;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Error: invalid snapshot\n");
		return result;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) {
		fprintf(stderr, "Error: the list of processes is empty\n");
		CloseHandle(hProcessSnap);
		return result;
	}

	do {
		if (0 == wcscmp(processname, pe32.szExeFile)) {
			result = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return result;
}

int InjectDllToTargetProcess(int pid, const WCHAR* dllPath) {
	long dllPathSize = sizeof(dllPath[0]) * wcslen(dllPath);

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hProc == NULL) {
		fprintf(stderr, "Error: Failed to open target process with pid %d\n", pid);
		return 1;
	}

	LPVOID addrAlloc = VirtualAllocEx(hProc, NULL, dllPathSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (addrAlloc == NULL) {
		fprintf(stderr, "Error: Failed to allocate memory in target process with pid %d", pid);
		return 1;
	}

	int writeToTargetProcessMemSuccess = WriteProcessMemory(hProc, addrAlloc, dllPath, dllPathSize, 0);
	if (writeToTargetProcessMemSuccess == NULL) {
		fprintf(stderr, "ERROR: Failed to write into target process memory\n");
		return 1;
	}

	LPTHREAD_START_ROUTINE addrLoadLibrary = GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW");
	if (addrLoadLibrary == NULL) {
		fprintf(stderr, "Error: Failed to get process address\n");
		return 1;
	}

	DWORD lpThreadIdNeseccary;
	HANDLE remoteThread = CreateRemoteThread(hProc, NULL, NULL, addrLoadLibrary, addrAlloc, 0, &lpThreadIdNeseccary);
	if (remoteThread == NULL) {
		fprintf(stderr, "Error: Failed to create remote thread\n");
		return 1;
	}

	if ((hProc != NULL) && (addrAlloc != NULL) &&
		(writeToTargetProcessMemSuccess != ERROR_INVALID_HANDLE) &&
		(remoteThread != NULL)) {
		CloseHandle(hProc);
		return 0;
	}

	return 1;
}

int FileExist(const WCHAR* name) {
	return _waccess(name, 0) != -1;
}