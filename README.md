# Introduction to Systems Software
### Project Name: Linkage Loader
Contributors: ```Gabriel Cardona | Iliya Kulbaka | Rohan Josh```

# Project Description 
This project consisted of writting a C program that could link multiple SIC/XE programs and perform relocation based on a given starting address. The program reads the files containing SIC/XE object code and combines them into one file. As its reading each file, it also modifies the addresses in the H, T, D, M and E records. [Iliya explain here a little more about how we are modifying addresses]

### Requirements to run the program
The program requires a minimum of 4 command line arguments:
- Relocation address in hexadecimal
- Name of output file (without any extensions)
- Two or more object files that need to be linked
NOTE: The program will check that there are a minimum of 4 arguments, and it will also check the output file does not have any extensions. 
