
#include "headers.h"

// adding a record to t-table
void AddMRecord(MSTAB* table, MSTAB* record){
    // if this is the first element
    if(strcmp(table->address, "\0") == 0){
        strcpy(table->address, record->address);
        strcpy(table->address_new, record->address_new);
        strcpy(table->len, record->len);
        strcpy(table->mod_sym, record->mod_sym);
        strcpy(table->symbol, record->symbol);
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
void AddMTable(M_TABLES* main_table, M_TABLES* table){
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