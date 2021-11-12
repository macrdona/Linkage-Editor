#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

struct records {
	char address[10]; 
	char Name[10]; 
    char recordType[2];
};
typedef struct records ESTAB;

void AddSymbol(ESTAB* externalSymbolTable[], char* address, char* symbol, char* type);
int SymbolExists(ESTAB* externalSymbolTable[], char* symbol);
void AddToTable(ESTAB* externalSymbolTable, char* symbol, char* address, char* type);