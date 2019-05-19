// driver1-kicker.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <windows.h>
#include <stdio.h>


int main()
{
	auto hEvent = CreateEvent(NULL, FALSE, FALSE, L"Global\\TestEvent");
	if (!hEvent)
	{
		wprintf(L"CreateEvent failed with 0x%x\n", GetLastError());
		getchar();
		return 1;
	}

	wprintf(L"Event created. Press ENTER to continue ...\n");
	getchar();
	
	auto err = SetEvent(hEvent);
	if (!err)
	{
		wprintf(L"SetEvent failed with 0x%x\n", GetLastError());
		getchar();
		return 1;
	}

	wprintf(L"Event successfully created!\n");
	getchar();
	return 0;
}
