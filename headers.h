#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

struct d_records {
	char address[10]; 
	char Name[10];
    char recordType[2];
};
typedef struct d_records ESTAB;

struct t_records{
//    char address[6];
    char address[7];
    char address_new[7];
//    char len[2];
    char len[3];
//    char data[60];
    char data[61];
    struct t_records* next;
};
typedef struct t_records TSTAB;

struct t_tables{
    TSTAB* table;
    struct t_tables* next;
};
typedef struct t_tables T_TABLES;


void AddSymbol(ESTAB* externalSymbolTable[], char* address, char* symbol, char* type);
int SymbolExists(ESTAB* externalSymbolTable[], char* symbol);
void AddToTable(ESTAB* externalSymbolTable, char* symbol, char* address, char* type);

void AddTRecord(TSTAB* table, TSTAB* record);
void AddTTable(T_TABLES* main_table, T_TABLES* table);