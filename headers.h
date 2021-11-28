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
    char address[7];
    char address_new[7];
    char len[3];
    char data[61];
    struct t_records* next;
};
typedef struct t_records TSTAB;

struct t_tables{
    TSTAB* table;
    struct t_tables* next;
};
typedef struct t_tables T_TABLES;

struct m_records{
    char address[7];
    char address_new[7];
    char len[3];
    char mod_sym[2];
    char symbol[7];
    struct m_records* next;
};
typedef struct m_records MSTAB;

struct m_tables{
    MSTAB* table;
    struct m_tables* next;
};
typedef struct m_tables M_TABLES;

struct e_records{
    char address[7];
    struct e_records* next;
};
typedef struct e_records ERSTAB;

struct e_tables{
    ERSTAB* table;
    struct e_tables* next;
};
typedef struct e_tables E_TABLES;

void AddSymbol(ESTAB* externalSymbolTable[], char* address, char* symbol, char* type);
int SymbolExists(ESTAB* externalSymbolTable[], char* symbol);
void AddToTable(ESTAB* externalSymbolTable[], char* symbol, char* address, char* type);

// T-Records
void AddTRecord(TSTAB* table, TSTAB* record);
void AddTTable(T_TABLES* main_table, T_TABLES* table);

// M-Records
void AddMRecord(MSTAB* table, MSTAB* record);
void AddMTable(M_TABLES* main_table, M_TABLES* table);

void appendToSymbol(char* symbol, int length, char* append, int append_front);
int checkMemory(long address);

// E-Records
void AddERecord(ERSTAB* table, ERSTAB* record);
void AddETable(E_TABLES* main_table, E_TABLES* table);