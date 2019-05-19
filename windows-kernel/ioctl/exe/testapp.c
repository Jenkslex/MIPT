/*++

Copyright (c) 1990-98  Microsoft Corporation All Rights Reserved

Module Name:

    testapp.c

Abstract:

Environment:

    Win32 console multi-threaded application

--*/
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>
#include "..\sys\sioctl.h"


BOOLEAN
ManageDriver(
    __in LPCTSTR  DriverName,
    __in LPCTSTR  ServiceName,
    __in USHORT   Function
    );

BOOLEAN
SetupDriverName(
    __inout_bcount_full(BufferLength) PCHAR DriverLocation,
    __in ULONG BufferLength
    );

char OutputBuffer[100];
char InputBuffer[100];

struct deviceGetSumArgs {
	ULONG opA;
	ULONG opB;
	
	DWORD method;
	char *inBuf;
	ULONG inSize;
	char *outBuf;
	ULONG outSize;
	HANDLE hDevice;
	
	ULONG opRes;
};

LONG deviceGetSum (struct deviceGetSumArgs *args) {
	BOOL bRc;
	ULONG bytesReturned;
	PLONG ops = (PLONG)args->inBuf;

    memset(args->outBuf, 0, args->outSize);
	ops[0] = args->opA;
	ops[1] = args->opB;
	
	printf("\nCalling DeviceIoControl:\n");

	bRc = DeviceIoControl ( args->hDevice,
                            args->method,
                            args->inBuf,
                            args->inSize,
                            args->outBuf,
                            args->outSize,
                           &bytesReturned,
                           NULL
                           );
    if ( !bRc )
    {
        printf ( "Error in DeviceIoControl : %d", GetLastError());
        return bRc;

    }
	
	ops = (PLONG)args->outBuf;
	args->opRes = ops[0];
	
	printf("	%lu+%lu=%lu\n", args->opA, args->opB, args->opRes);
	return bRc;
}

VOID __cdecl
main(
    __in ULONG argc,
    __in_ecount(argc) PCHAR argv[]
    )
{
    HANDLE hDevice;
    DWORD errNum = 0;
    TCHAR driverLocation[MAX_PATH];
	struct deviceGetSumArgs args;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    //
    // open the device
    //

    if ((hDevice = CreateFile( "\\\\.\\IoctlTest",
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            NULL,
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL)) == INVALID_HANDLE_VALUE) {

        errNum = GetLastError();
        if (errNum != ERROR_FILE_NOT_FOUND) {

            printf("CreateFile failed!  ERROR_FILE_NOT_FOUND = %d\n", errNum);

            return ;
        }

        //
        // The driver is not started yet so let us the install the driver.
        // First setup full path to driver name.
        //
        if (!SetupDriverName(driverLocation, sizeof(driverLocation))) {

            return ;
        }

        if (!ManageDriver(DRIVER_NAME,
                          driverLocation,
                          DRIVER_FUNC_INSTALL
                          )) {

            printf("Unable to install driver. \n");
            //
            // Error - remove driver.
            //

            ManageDriver(DRIVER_NAME,
                         driverLocation,
                         DRIVER_FUNC_REMOVE
                         );

            return;
        }

        hDevice = CreateFile( "\\\\.\\IoctlTest",
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            NULL,
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

        if ( hDevice == INVALID_HANDLE_VALUE ){
            printf ( "Error: CreatFile Failed : %d\n", GetLastError());
            return;
        }

    }

    //
    // Printing Input & Output buffer pointers and size
    //

    printf("InputBuffer Pointer = %p, BufLength = %d\n", InputBuffer,
                        sizeof(InputBuffer));
    printf("OutputBuffer Pointer = %p BufLength = %d\n", OutputBuffer,
                                sizeof(OutputBuffer));
    //
    // Performing METHOD_BUFFERED
    //
	
	args.opA = 11;
	args.opB = 13;
	args.inBuf = InputBuffer;
	args.inSize = 100;
	args.outBuf = OutputBuffer;
	args.outSize = 100;
	args.hDevice = hDevice;
	args.method = (DWORD) IOCTL_SIOCTL_METHOD_BUFFERED;
	deviceGetSum(&args);

    //
    // Performing METHOD_NIETHER
    //
	
	args.opA = 5;
	args.opB = 8;
	args.inBuf = InputBuffer;
	args.inSize = 100;
	args.outBuf = OutputBuffer;
	args.outSize = 100;
	args.hDevice = hDevice;
	args.method = (DWORD) IOCTL_SIOCTL_METHOD_NEITHER;
	deviceGetSum(&args);

    //
    // Performing METHOD_IN_DIRECT
    //

	args.opA = 34;
	args.opB = 21;
	args.inBuf = InputBuffer;
	args.inSize = 100;
	args.outBuf = OutputBuffer;
	args.outSize = 100;
	args.hDevice = hDevice;
	args.method = (DWORD) IOCTL_SIOCTL_METHOD_IN_DIRECT;
	deviceGetSum(&args);

    //
    // Performing METHOD_OUT_DIRECT
    //
	
	args.opA = 23456;
	args.opB = 23451;
	args.inBuf = InputBuffer;
	args.inSize = 100;
	args.outBuf = OutputBuffer;
	args.outSize = 100;
	args.hDevice = hDevice;
	args.method = (DWORD) IOCTL_SIOCTL_METHOD_OUT_DIRECT;
	deviceGetSum(&args);

    CloseHandle ( hDevice );
}


