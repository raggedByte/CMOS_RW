# CMOS_RW
App for read/write CMOS memory

Note: cmosrw.sys should be in directory with cmos_testapp.exe!

Also note: output showed in hex!

Develop for terminal/cmd.


**Usage example**

```
[ps]$ .\cmos_testapp.exe

Driver is ready. . .
Type "help" for getting help usage

cmosrw> read
Response from driver(50 bytes):
     0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
     ================================================================================================
0  | 43    45    5     7     12    3     4     1     4     21    26    2     50    80    0     0
10 | 40    EF    7B    E6    3     7F    2     FF    FF    FF    30    EF    CB    FE    15    BD
20 | 5F    E7    E2    FF    6B    FA    E3    ED    FB    FB    FF    EF    FA    EF    15    F4
30 | FF    FF    -     -     -     -     -     -     -     -     -     -     -     -     -     -

cmosrw> read 2 5
Response from driver(4 bytes):
     0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
     ================================================================================================
0  | -     -     5     7     12    3     -     -     -     -     -     -     -     -     -     -

cmosrw> write 3 A
OK

cmosrw> read 2 5
Response from driver(4 bytes):
     0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
     ================================================================================================
0  | -     -     6     A     12    3     -     -     -     -     -     -     -     -     -     -

cmosrw> write 3 7
OK

cmosrw> read 2 5
Response from driver(4 bytes):
     0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
     ================================================================================================
0  | -     -     6     7     12    3     -     -     -     -     -     -     -     -     -     -

cmosrw> exit
Exit . . .
```
