#include "headers.h"

void AddSymbol(ESTAB* externalSymbolTable[], char* address, char* symbol, char* type){
    int index = 0;
    ESTAB* add;

    //allocate memory for the struct
    add = malloc(sizeof(ESTAB));

    strcpy(add->address, address);
    strcpy(add->Name, symbol);
    strcpy(add->recordType, type);

    //checks if the next element is empty
    while(externalSymbolTable[index] != NULL){
        index++;
    }

    //intert the element where there is an empty space
    externalSymbolTable[index] = add;
}

int SymbolExists(ESTAB* externalSymbolTable[], char* symbol){
    int result = 0;
    int index = 0;

    //checks if next element is empty
    while(externalSymbolTable[index] != NULL){

        //compare the strings from current element and symbol to be inserted
        if(strcmp(symbol, externalSymbolTable[index]->Name) == 0){
            result = -1;
            break;
        }
        index++;
    }
    return result;
}

void AddToTable(ESTAB* externalSymbolTable[], char* symbol, char* address, char* type){
    //check for duplicate symbols
    if(SymbolExists(externalSymbolTable, symbol) == 0){
        //if not duplicate, add symbol
        AddSymbol(externalSymbolTable, address, symbol, type);
    }
    else{
        printf("%s is a duplicate symbol!\n", symbol);
//        return 0;    // TODO: do we need this, return type is void?
    }
}