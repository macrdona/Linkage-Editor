# Introduction to Systems Software
### Project Name: Linkage Loader
Contributors: ```Gabriel Cardona | Iliya Kulbaka | Rohan Josh```

# Project Description 
This project consisted of writing a C program that could link multiple SIC/XE programs and perform relocation based on a given starting address. The program reads the files containing SIC/XE object code and combines them into one file. As its reading each file, it also modifies the addresses in the H, T, D, M and E records.

# Address Modification Process
- Address Modification follows in two steps:
    - Normalization of the addresses.
    - Reformatting of the addresses and file structure.

### Normalization
After the initial read and log of all of the records (from all of the files), initial load addresses and program lengths (found in H records) are being stored.
Next the initial load address is being subtracted from all of the records to make the initial load address be 0x0.
Subsequently all other records are now positioned at an offset of the 0x0 address.
- This leads to two things:
    - All record addresses are now much easier to relocate.
    - All records of trailing files can be easier appended to the initial file.
    
### Reformatting of the addresses
Every file following the first one will have the program lengths of all of the previous files added to their base record addresses.
All of the modified record types will be appended to their record groups in sequential order.
Any program name dependent record is being modified to use the new program name (set in H record).
Finally the relocation address is being added to all of the record addresses.
- This allows for the following:
    - The sequential execution of all of the records is preserved.
    - The references to other parts of the program are preserved.
    - Relocation to any available memory address becomes a trivial issue.

### Requirements to run the program
The program requires a minimum of 4 command line arguments:
- Relocation address in hexadecimal
- Name of output file (without any extensions)
- Two or more object files that need to be linked
NOTE: The program will check that there are a minimum of 4 arguments, and it will also check the output file does not have any extensions. 
