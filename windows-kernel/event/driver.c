#include "ntddk.h"

#define STDCALL __stdcall

/*
 *  Undocument function 
 */
NTSTATUS STDCALL ZwCreateEvent(
	OUT PHANDLE  EventHandle,
	IN ACCESS_MASK  DesiredAccess,
	IN POBJECT_ATTRIBUTES  ObjectAttributes OPTIONAL,
	IN EVENT_TYPE  EventType,
	IN BOOLEAN  InitialState);

/*
 *	forward declarations of functions
 */
NTSTATUS STDCALL DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pusRegPath);

/*
 *	desired sections for functions
 */
#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#endif

/*
 *	COMMENT:
 *		entry function, [INIT]
 *	ARGUMENTS:
 *		standard for DriverEntry
 *	RETURN VALUE:
 *		NTSTATUS
 */
NTSTATUS STDCALL DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pusRegPath)
{
	NTSTATUS Status;
	HANDLE hEvent;
	OBJECT_ATTRIBUTES oa;
	UNICODE_STRING us;
	PVOID pEvent;
	LARGE_INTEGER timeout;

	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(pusRegPath);

	DbgPrint("!!! Hello world (init)! \n");
	__debugbreak();

	/*
	 *	FUTURE: 
	 *		creating device and symbolic link
	 *		initialize driver functions
	 *		making this driver unloadable
	 */
	
	RtlInitUnicodeString(
		&us,
		L"\\BaseNamedObjects\\TestEvent");

	InitializeObjectAttributes(
		&oa,
		&us,  			//ObjectName
		OBJ_CASE_INSENSITIVE, 	//Attributes
		NULL,			//RootDirectory
		NULL);			//SecurityDescriptor

    Status = ZwOpenEvent(&hEvent, EVENT_ALL_ACCESS, &oa);

	if (!NT_SUCCESS(Status))
	{
		DbgPrint("!!! Failed to open event \n");
		return Status;
	}

	Status = ObReferenceObjectByHandle(
		hEvent, 		//Handle
		EVENT_ALL_ACCESS,	//DesiredAccess
		NULL,			//ObjectType
		KernelMode,		//AccessMode
		&pEvent,		//Object
		NULL);			//HandleInformation

	if (!NT_SUCCESS(Status))
	{
		ZwClose(hEvent);
		DbgPrint("!!! Failed to reference event \n");
		return Status;
	}

    DbgPrint("!!! Waiting for an event ... \n");
	timeout.QuadPart = -100000000;
	Status = KeWaitForSingleObject(
		pEvent,  		//Object
		Executive,		//WaitReason
		KernelMode,		//WaitMode
		FALSE,			//Alertable
		&timeout);		//Timeout

	DbgPrint("!!! Return from wait with 0x%08X \n", Status);
	ObDereferenceObject(pEvent);
	ZwClose(hEvent);
	return Status;
}

