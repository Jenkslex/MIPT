#include <stdio.h>
#include <strsafe.h>

#include "handler.h"

typedef struct client_t {
	HANDLE g_hChildStd_IN_Rd;
	HANDLE g_hChildStd_IN_Wr;
	HANDLE g_hChildStd_OUT_Rd;
	HANDLE g_hChildStd_OUT_Wr;

	SECURITY_ATTRIBUTES saAttr;

	SOCKET client_conn;
} client;

#define BUFFSIZE 10000

static const char exit_phrase[] = "Terminate RPC";

DWORD WINAPI create_client_handler(LPVOID);
DWORD WINAPI read_from_pipe(LPVOID);

void create_child_process(client* client_info);
int write_to_pipe(client* client_info);
void ErrorExit(PTSTR lpszFunction);

int wrap_create_client_handler(SOCKET client_conn) {
	printf("Attempt to create thread\n");
	DWORD  dwThreadId = 0;
	HANDLE hThread = CreateThread(
		NULL,              // no security attribute 
		0,                 // default stack size 
		create_client_handler,    // thread proc
		client_conn,    // thread parameter 
		0,                 // not suspended 
		&dwThreadId);      // returns thread ID 

	if (hThread == NULL) {
		fprintf(stderr, TEXT("CreateThread failed, GLE=%d.\n"), GetLastError());
		return -1;
	}
	else CloseHandle(hThread);
}

DWORD WINAPI create_client_handler(LPVOID client_conn) {
	printf("Thread created\n");
	client client_info;
	printf("SERVER thread: %d\n", client_conn);

	client_info.client_conn = client_conn;

	// Set the bInheritHandle flag so pipe handles are inherited. 
	client_info.saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	client_info.saAttr.bInheritHandle = TRUE;
	client_info.saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT. 
	if (!CreatePipe(&(client_info.g_hChildStd_OUT_Rd), &(client_info.g_hChildStd_OUT_Wr),
		&(client_info.saAttr), 0))
		ErrorExit(TEXT("StdoutRd CreatePipe"));

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (!SetHandleInformation(client_info.g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
		ErrorExit(TEXT("Stdout SetHandleInformation"));

	// Create a pipe for the child process's STDIN. 
	if (!CreatePipe(&(client_info.g_hChildStd_IN_Rd), &(client_info.g_hChildStd_IN_Wr),
		&(client_info.saAttr), 0))
		ErrorExit(TEXT("Stdin CreatePipe"));

	// Ensure the write handle to the pipe for STDIN is not inherited. 
	if (!SetHandleInformation(client_info.g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
		ErrorExit(TEXT("Stdin SetHandleInformation"));

	create_child_process(&client_info);

	DWORD  dwThreadId = 0;
	HANDLE hThread = CreateThread(
		NULL,              // no security attribute 
		0,                 // default stack size 
		read_from_pipe,    // thread proc
		&client_info,    // thread parameter 
		0,                 // not suspended 
		&dwThreadId);      // returns thread ID 

	if (hThread == NULL) {
		fprintf(stderr, TEXT("CreateThread failed, GLE=%d.\n"), GetLastError());
		return -1;
	}
	else CloseHandle(hThread);

	write_to_pipe(&client_info);

	if (!CloseHandle(client_info.g_hChildStd_IN_Rd))
		ErrorExit(TEXT("StdInWr CloseHandle"));
	if (!CloseHandle(client_info.g_hChildStd_IN_Wr))
		ErrorExit(TEXT("StdInWr CloseHandle"));
	if (!CloseHandle(client_info.g_hChildStd_OUT_Rd))
		ErrorExit(TEXT("StdInWr CloseHandle"));
	if (!CloseHandle(client_info.g_hChildStd_OUT_Wr))
		ErrorExit(TEXT("StdInWr CloseHandle"));
}

void create_child_process(client* client_info) {
	TCHAR szCmdline[] = TEXT("cmd.exe");
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError  = client_info->g_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = client_info->g_hChildStd_OUT_Wr;
	siStartInfo.hStdInput  = client_info->g_hChildStd_IN_Rd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 

	bSuccess = CreateProcess(NULL,
		szCmdline,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

					   // If an error occurs, exit the application. 
	if (!bSuccess)
		ErrorExit(TEXT("CreateProcess"));
	else {
		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
	}
}

DWORD WINAPI read_from_pipe(LPVOID client_in) {
	client* client_info = (client*) client_in;
	DWORD dwRead = 0, dwWritten = 0;
	CHAR chBuf[BUFFSIZE];
	BOOL bSuccess = FALSE;

	while(1) {
		bSuccess = ReadFile(client_info->g_hChildStd_OUT_Rd, chBuf, BUFFSIZE, &dwRead, NULL);
		if (!bSuccess) 
			break;

		printf("%.*s", dwRead, chBuf);

		if (client_info->client_conn != 0 && 
			client_info->client_conn != INVALID_SOCKET) {
			send(client_info->client_conn, chBuf, dwRead, 0);
		}
	}

	closesocket(client_info->client_conn);

	if (client_info->client_conn != INVALID_SOCKET)
		client_info->client_conn = INVALID_SOCKET;

	return 0;
}

int write_to_pipe(client* client_info) {
	DWORD dwRead = 0, dwWritten = 0;
	CHAR chBuf[BUFFSIZE];
	CHAR terminate[] = "exit\r\n";
	BOOL bSuccess = FALSE;

	while(1) {
		int iResult = recv(client_info->client_conn, chBuf, BUFFSIZE, 0);

		if (iResult != SOCKET_ERROR && iResult != 0) {
			bSuccess = WriteFile(client_info->g_hChildStd_IN_Wr, chBuf, iResult, &dwWritten, NULL);

			if (!bSuccess || strncmp(chBuf, exit_phrase, strlen(exit_phrase) - 1) == 0) {
				bSuccess = WriteFile(client_info->g_hChildStd_IN_Wr, terminate, strlen(terminate), &dwWritten, NULL);
				break;
			}
		} else
			break;
	}

	closesocket(client_info->client_conn);
	
	if (client_info->client_conn != INVALID_SOCKET)
		client_info->client_conn = INVALID_SOCKET;

	return 0;
}

void ErrorExit(PTSTR lpszFunction) {
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(1);
}