#include "ntddk.h"
#include "string.h"
#include "cmosrw.h"

#define NT_DEVICE_NAME L"\\Device\\cmosrw"
#define DOS_DEVICE_NAME L"\\DosDevices\\cmosrw"

//defined in ./cmos_rw.asm
VOID __cdecl writecmos(CHAR address, CHAR value);
CHAR __cdecl readcmos(CHAR address);

DRIVER_INITIALIZE DriverEntry;
_Dispatch_type_(IRP_MJ_CREATE)
_Dispatch_type_(IRP_MJ_CLOSE)
DRIVER_DISPATCH CreateCloseRoutine;
_Dispatch_type_(IRP_MJ_DEVICE_CONTROL)
DRIVER_DISPATCH DeviceControlRoutine;
DRIVER_UNLOAD UnloadDriverRoutine;


/*
VOID LogIrpInfo(PIRP Irp);

VOID PrintChars(
	_In_reads_(CountChars) PCHAR BufferAddress,
	_In_ size_t CountChars
);

#ifdef ALLOC_PRAGMA
#pragma alloc_text( INIT, DriverEntry )
#pragma alloc_text( PAGE, CreateCloseRoutine )
#pragma alloc_text( PAGE, DeviceControlRoutine )
#pragma alloc_text( PAGE, UnloadDriverRoutine )
#pragma alloc_text( PAGE, LogIrpInfo )
#pragma alloc_text( PAGE, PrintChars )
#endif // ALLOC_PRAGMA
*/


NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
{
	NTSTATUS ntStatus;
	UNICODE_STRING ntDeviceNameString;
	UNICODE_STRING ntWin32NameString;
	PDEVICE_OBJECT deviceObject;

	deviceObject = NULL;

	UNREFERENCED_PARAMETER(RegistryPath);

	RtlInitUnicodeString(&ntDeviceNameString, NT_DEVICE_NAME);

	ntStatus = IoCreateDevice(
		DriverObject,
		0,
		&ntDeviceNameString,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&deviceObject
	);

	if (!NT_SUCCESS(ntStatus))
	{
		//log kdprint here
		return ntStatus;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateCloseRoutine;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = CreateCloseRoutine;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceControlRoutine;
	DriverObject->DriverUnload = UnloadDriverRoutine;

	RtlInitUnicodeString(&ntWin32NameString, DOS_DEVICE_NAME);

	ntStatus = IoCreateSymbolicLink(
		&ntWin32NameString,
		&ntDeviceNameString
	);

	if (!NT_SUCCESS(ntStatus))
	{
		//log kdprint here
		IoDeleteDevice(deviceObject);
	}

	return ntStatus;
}

NTSTATUS CreateCloseRoutine(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	//PAGED_CODE();

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

VOID UnloadDriverRoutine(
	_In_ PDRIVER_OBJECT DriverObject
)
{
	PDEVICE_OBJECT deviceObject = DriverObject->DeviceObject;
	UNICODE_STRING ntWin32NameString;

	//PAGED_CODE();

	RtlInitUnicodeString(&ntWin32NameString, DOS_DEVICE_NAME);

	IoDeleteSymbolicLink(&ntWin32NameString);

	if (deviceObject != NULL)
	{
		IoDeleteDevice(deviceObject);
	}
}

NTSTATUS DeviceControlRoutine(
	PDEVICE_OBJECT DeviceObject,
	PIRP Irp
)
{
	PIO_STACK_LOCATION irpSp;
	NTSTATUS ntStatus = STATUS_SUCCESS;
	ULONG outBufferLength;
	ULONG inBufferLength;
	PCHAR outBuf;
	PCHAR inBuf;
	CHAR arg0, arg1;
	CHAR bcd;
	INT bytesReaded = 0;

	UNREFERENCED_PARAMETER(DeviceObject);

	//PAGED_CODE();

	irpSp = IoGetCurrentIrpStackLocation(Irp);
	outBufferLength = irpSp->Parameters.DeviceIoControl.OutputBufferLength;
	inBufferLength = irpSp->Parameters.DeviceIoControl.InputBufferLength;

	if (!outBufferLength || !inBufferLength)
	{
		ntStatus = STATUS_INVALID_PARAMETER;
		Irp->IoStatus.Status = ntStatus;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return ntStatus;
	}

	inBuf = Irp->AssociatedIrp.SystemBuffer;
	outBuf = Irp->AssociatedIrp.SystemBuffer;

	arg0 = inBuf[0];
	arg1 = inBuf[1];

	memset(outBuf, 0, outBufferLength);

	switch (irpSp->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_READ_CMOS:
		for (CHAR i = arg0; i <= arg1; i++)
		{
			outBuf[i] = readcmos(i);
			bytesReaded++;
		}

		Irp->IoStatus.Information = bytesReaded;
		break;

	case IOCTL_WRITE_CMOS:
		writecmos(arg0, arg1);
		outBuf[0] = DRIVER_SUCCESS;
		Irp->IoStatus.Information = 1;
		break;

	/*
	case IOCTL_WRITE_BCD:
		bcd = readcmos(0xB);
		bcd = arg0 ? bcd | 0x2 : bcd & 0xFD;
		writecmos(0xB, bcd);
		outBuf[0] = DRIVER_SUCCESS;
		Irp->IoStatus.Information = 1;
		break;
		*/

	default:
		ntStatus = STATUS_INVALID_DEVICE_REQUEST;
		//log kdprint here
		break;
	}

	Irp->IoStatus.Status = ntStatus;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return ntStatus;
}

