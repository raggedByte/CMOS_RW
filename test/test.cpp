#include "stdio.h"
#include "windows.h"

VOID BINtoBCD(PCHAR array, INT arrayLength)
{
    for (INT i = 0; i < arrayLength; i++)
    {
        CHAR digitL = array[i] & 0x0F, digitH = (array[i] & 0xF0) >> 4;
        array[i] = digitH * 10 + digitL;
    }
}
VOID PrintArrayInColumns(PCHAR array, UINT arrayLength, UINT countColumns, CHAR separator = '\t')
{
    UINT countRows = arrayLength / countColumns + ((arrayLength % countColumns) > 0 ? 1 : 0);
    
    for (INT i = 0; i < countRows; i++)
    {
        for (INT k = 0; k < countColumns; k++)
        {
            UINT item = k + i * countColumns;

            if (item >= arrayLength) 
                continue;

            printf("(%2dh): %d", item, array[item]);

            if (k < countColumns)
                putchar(separator);
        }
        printf("\r\n");
    }
}


int main()
{
    char array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    PrintArrayInColumns(array, sizeof(array), 2);

    BINtoBCD(array, sizeof(array));

	return 0;
}