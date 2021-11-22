#include "headers.h"

// adding a record to t-table
void AddTRecord(TSTAB* table, TSTAB* record){
    // if this is the first element
    if(strcmp(table->address, "\0") == 0){
        strcpy(table->address, record->address);
        strcpy(table->len, record->len);
        strcpy(table->data, record->data);
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

// adding t-table to table chain
void AddTTable(T_TABLES* main_table, T_TABLES* table){
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