#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>
#include "cmosrw.h"
#include "commands.h"

#define COLUMNS 16

VOID HexConvertor(PCHAR array, INT arrayLength);
UINT CommandDispatcher(PCHAR args);

BOOLEAN ManageDriver(
    _In_ LPCTSTR  DriverName,
    _In_ LPCTSTR  ServiceName,
    _In_ USHORT   Function
);

BOOLEAN SetupDriverName(
    _Inout_updates_bytes_all_(BufferLength) PTCHAR DriverLocation,
    _In_ ULONG BufferLength
);

UCHAR OutputBuffer[100];
UCHAR InputBuffer[100];

VOID __cdecl main(
    _In_ ULONG argc,
    _In_reads_(argc) PTCHAR argv[]
)
{
    HANDLE hDevice;
    BOOL bRc;
    //BOOL bHexConverter = FALSE;
    ULONG bytesReturned;
    DWORD errNum = 0;
    TCHAR driverLocation[MAX_PATH];
    UINT command = 0;
    DWORD ioctl;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    if ((hDevice = CreateFile(DRIVER_PATH,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL)) == INVALID_HANDLE_VALUE) {

        errNum = GetLastError();

        if (errNum != ERROR_FILE_NOT_FOUND) {

            printf("CreateFile failed : %d\n", errNum);

            return;
        }

        if (!SetupDriverName(driverLocation, sizeof(driverLocation))) {
            return;
        }

        if (!ManageDriver(DRIVER_NAME,
            driverLocation,
            DRIVER_FUNC_INSTALL
        )) {

            printf("Unable to install driver.\n");

            ManageDriver(DRIVER_NAME,
                driverLocation,
                DRIVER_FUNC_REMOVE
            );

            return;
        }

        hDevice = CreateFile(DRIVER_PATH,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (hDevice == INVALID_HANDLE_VALUE) {
            printf("Error: CreatFile Failed : %d\n", GetLastError());
            return;
        }

    }

    printf("Driver is ready. . . \r\n\
Type \"help\" for getting help usage\r\n");
    //HexConverter = % d\r\n\
    
    while (command != COMMAND_EXIT) {

        command = CommandDispatcher(InputBuffer);

        if (command == COMMAND_EXIT)
            break;

        /*if (command == COMMAND_TURN_CONVERTER)
        {
            bHexConverter = !bHexConverter;
            printf("HexConverter = %d\r\n", bHexConverter);
            continue;
        }*/

        bRc = FALSE;
        memset(OutputBuffer, 0, sizeof(OutputBuffer));

        ioctl =
            command == COMMAND_READ ? IOCTL_READ_CMOS : IOCTL_WRITE_CMOS;

        bRc = DeviceIoControl(hDevice,
            ioctl,
            &InputBuffer,
            2,
            &OutputBuffer,
            sizeof(OutputBuffer),
            &bytesReturned,
            NULL
        );

        if (!bRc)
        {
            printf("Error in DeviceIoControl : %d\r\n", GetLastError());
            break;
        }

        /*if (command == COMMAND_READ && bHexConverter)
            HexConvertor(OutputBuffer, bytesReturned);*/

        printf("Response from driver(%d bytes): \r\n", bytesReturned);
        for (ULONG i = 0; i < COLUMNS; i++)
        {
            printf("%-5X ", i);
        }
        printf("\r\n");
        for (ULONG i = 0; i < COLUMNS; i++)
        {
            printf("======");
        }
        printf("\r\n");
        for (ULONG i = 0; i < bytesReturned; i++)
        {
            printf("%-5X ", OutputBuffer[i]);
            if ((i+1) % COLUMNS == 0)
                printf("\r\n");
        }
        printf("\r\n");

        if ((command == COMMAND_READ && bytesReturned != (InputBuffer[1] - InputBuffer[0] + 1)) ||
            (command == COMMAND_WRITE && (DWORD)OutputBuffer[0] != (DWORD)DRIVER_SUCCESS))
        {
            printf("Received unxpected result! \r\n");
            break;
        }
        
        printf("Successful!\r\n");
    }

    printf("Exit . . .\r\n");

    CloseHandle(hDevice);

    ManageDriver(DRIVER_NAME,
        driverLocation,
        DRIVER_FUNC_REMOVE
    );
}



VOID HexConvertor(PCHAR array, INT arrayLength)
{
    for (INT i = 0; i < arrayLength; i++)
    {
        CHAR digitL = array[i] & 0x0F, digitH = (array[i] & 0xF0) >> 4;
        array[i] = digitH * 10 + digitL;
    }
}