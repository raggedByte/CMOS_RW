#include "windows.h"
#include "stdio.h"
//#include "cmosrw.h"
#include "commands.h"

#define USER_PROMT TEXT("cmosrw")

VOID printUsage()
{
	printf("Usage: \r\n \
		help - help\r\n \
		read - read 50 byte from cmos memory\r\n \
		read <lborder> <rborder> - read cmos memory on range = [lborder, rborder] (inclusive)\r\n \
		write <address> <byte> - write byte to cmos memory on address\r\n \
		exit - terminate driver and app\r\n");
		//hexconv - turn on\\off hex converter for readed bytes\r\n

	printf("Examples:\r\n \
		read 0 9 - read 10 byte starting at 0 address\r\n \
		write 5 0 - write \"0\" to cmos at 5 address (return code 37 is Successful)\r\n");
}

UINT CommandDispatcher(PCHAR args)
{
	BOOL isInterrupted = FALSE;
	CHAR command[8] = "", inputString[32];
	INT arg0 = 0, arg1 = 0;
	INT scanned = 0;
	UINT commandType;

	while (!isInterrupted)
	{
		memset(command, 0, sizeof(command));
		memset(inputString, 0, sizeof(inputString));

#ifdef UNICODE
		wprintf(L"\r\n" USER_PROMT TEXT("> "));
#elif
		printf("\r\n" USER_PROMT TEXT("> "));
#endif
		gets_s(inputString, sizeof(inputString));

		scanned = sscanf_s(inputString, "%s %d %d", command, (UINT)sizeof(command), &arg0, &arg1);

		switch (scanned)
		{
		case 1:
			if (!strcmp(command, "help"))
			{
				printUsage();
			}
			else if (!strcmp(command, "exit"))
			{
				commandType = COMMAND_EXIT;
				isInterrupted = TRUE;
			}
			else if (!strcmp(command, "read"))
			{
				args[0] = 0;
				args[1] = 49;
				commandType = COMMAND_READ;
				isInterrupted = TRUE;
			}
			/*else if (!strcmp(command, "hexconv"))
			{
				commandType = COMMAND_TURN_CONVERTER;
				isInterrupted = TRUE;
			}*/
			else
			{
				printf("Command not recognized! \r\n");
			}
			break;

		/*case 2:
			if (!strcmp(command, "bcd"))
			{
				commandType = COMMAND_BCD;
				args[0] = arg0 > 0 ? 1 : 0;
				isInterrupted = TRUE;
			}
			else
			{
				printf("Command not recognized! \r\n");
			}
			break;*/

		case 3:
			if (!strcmp(command, "read"))
			{
				commandType = COMMAND_READ;
			}
			else if (!strcmp(command, "write"))
			{
				commandType = COMMAND_WRITE;
			}
			else
			{
				printf("Command not recognized! \r\n");
				break;
			}
			args[0] = arg0;
			args[1] = arg1;
			isInterrupted = TRUE;
			break;

		default:
			printf("Unxpected count of args! Use \"help\"\r\n");
			break;
		}
	}

	return commandType;
}

