#include "headers.h"

// adding a record to e-table
void AddERecord(ERSTAB* table, ERSTAB* record){
    // if this is the first element
    if(strcmp(table->address, "\0") == 0){
        strcpy(table->address, record->address);
        table->next = NULL;
        return;
    }
    // looking for the last chain
    if (table->next != NULL){
        while (table->next != NULL){
            table = table->next;
        }
    }

    table->next = record;
}

// adding e-table to table chain
void AddETable(E_TABLES* main_table, E_TABLES* table){
    if(main_table->table == NULL){
        main_table->table = table->table;
        main_table->next = NULL;
        return;
    }

    while(main_table->next != NULL){
        main_table = main_table->next;
    }
    main_table->next = table;
}