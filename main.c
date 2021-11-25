#include "headers.h"

void help(){
    printf("%-15s ./Linkage_Editor [relocation] [output_file] [file_to_link] [file_to_link]...\n", "Usage");
    printf("%-15s -h, --help\n%-15s -m, --memory\n%-15s -o, --output\n%-15s -l, --links\n\n\n",
           "Help", "Relocation", "Output File", "Files to Link");
}

int main(int argc, char* argv[]){
    // INIT functions
    
    // Testing input THIS IS A TEMP LOOP REMOVE IN PRODUCTION
    for(int i=0; i<argc; i++){
        printf("Argv[%d] = %s\n", i, argv[i]);
    }
    
    //*Checks that there are more than three parameters.
    //*We need a address where the program will be relocated to, a
    //*name for the file, and at least two
    //*object files that we can link together
	if (argc < 4){
		printf("The number of arguments provided is insufficient.");
		help();
		return EXIT_FAILURE;
	}

    //*If second argument contains a '.',
    //*then a file was provided instead of a filename
    char* check;
    check = strstr(argv[2], ".");
    if(check){
        printf("Filename was not provided");
        help();
        return EXIT_FAILURE;
    }

    // Main start

    //File pointer
    FILE *fptr;

    //store the name of output file
    char file_name[30];
    strcpy(file_name, argv[2]);

    //creating an array of structs
	ESTAB* externalSymbolTable[1024] = {};
    memset(externalSymbolTable, '\0', 1024 * sizeof(struct externalSymbolTable*));

    char* load_point = malloc(20);
    memset(load_point, 0, 20);
    //copying relocation address into variable load point
    strcpy(load_point, argv[1]);

    int decimal_load_point;
    //converts address from hex to dec
    sscanf(load_point, "%X", &decimal_load_point);

    char buffer[1024];
	char bufferFull[1024];

    char* symbol;
	char* address;
    char* type;
    char record_Letter[2] = {"\000"};  // init to null
    char line[1024] = {"\000"};

    //allocating space to store a symbol
	symbol = malloc(1024 * sizeof(char));
	address = malloc(1024 * sizeof(char));
    type = malloc(1024 * sizeof(char));

	//fills a block of memory with null
	memset(symbol, '\0', 1024 * sizeof(char));
	memset(address, '\0', 1024 * sizeof(char));
    memset(type, '\0', 1024 * sizeof(char));

    // T-Tables
    T_TABLES t_tables_base;
    t_tables_base.table = NULL;
    t_tables_base.next = NULL;
    T_TABLES* tTables = NULL;
    tTables = (T_TABLES*) malloc(sizeof (T_TABLES));
    *(tTables) = t_tables_base;

    // M-Tables
    M_TABLES  m_tables_base;
    m_tables_base.table = NULL;
    m_tables_base.next = NULL;
    M_TABLES* mTables = NULL;
    mTables = (M_TABLES*) malloc(sizeof (M_TABLES));
    *(mTables) = m_tables_base;



    // will loop through all of the arvs, with 0 being the program call,
    //      2 being the output file and the rest links (TODO: latter one of them is gonna be a relocation param)
    for(int i = 2; i < argc; i++) {
        if (i == 2) {    // linking file output creation
            fptr = fopen(argv[i], "w"); // opening a linking file for writing

            // checking if file was created/opened
            if (fptr == NULL) {
                printf("Error: Cannot open file - %s \n", argv[i]);
                return EXIT_FAILURE;
            }
            fputs("", fptr);    // making new file
            fclose(fptr);
        }
        else {   // getting all of the files to be linked
            // T-Record Table
            TSTAB t_table_base; // initializer
            memset(t_table_base.address, '\000', 7);
            memset(t_table_base.address_new, '\000', 7);
            memset(t_table_base.len, '\000', 3);
            memset(t_table_base.data, '\000', 61);

            TSTAB *t_table = NULL;
            t_table = (TSTAB *) malloc(sizeof(TSTAB));
            *(t_table) = t_table_base;

            // Table for T-record table
            T_TABLES temp_t_tables_base;
            temp_t_tables_base.table = t_table;
            temp_t_tables_base.next = NULL;

            T_TABLES *temp_t_tables = NULL;
            temp_t_tables = (T_TABLES *) malloc(sizeof(T_TABLES));
            *(temp_t_tables) = temp_t_tables_base;

            // Adding table to tables
            AddTTable(tTables, temp_t_tables);

            // M-Table
            MSTAB  m_table_base;    // initializer
            memset(m_table_base.address, '\000', 7);
            memset(m_table_base.address_new, '\000', 7);
            memset(m_table_base.len, '\000', 3);
            memset(m_table_base.mod_sym, '\000', 2);
            memset(m_table_base.symbol, '\000', 7);
            m_table_base.next = NULL;

            MSTAB* m_table = NULL;
            m_table = (MSTAB *) malloc(sizeof (MSTAB));
            *(m_table) = m_table_base;

            // Table for M-Record table
            M_TABLES  temp_m_table_base;
            temp_m_table_base.table = m_table;
            temp_m_table_base.next = NULL;

            M_TABLES  *temp_m_tables = NULL;
            temp_m_tables = (M_TABLES*) malloc(sizeof (M_TABLES));
            *(temp_m_tables) = temp_m_table_base;

            AddMTable(mTables, temp_m_tables);

            //open file
            fptr = fopen(argv[i], "r");

            //checks if file exists
            if (fptr == NULL) {
                printf("Error: Cannot open file - %s \n", argv[i]);
                return EXIT_FAILURE;
            }

            //reads each line of the file and stores it in buffer
            while (fgets(buffer, 1024, fptr) != NULL) {
                strcpy(bufferFull, buffer);

                // My attempt at this
                char *ptr_save = NULL;
                char *single_line = NULL;
                strcpy(line,
                       buffer);   // writing memory stuff into a char array of equal size, easier to work like this

                single_line = strtok_r(line, "\t\n\r", &ptr_save);
                if (single_line != NULL) {    // if we have a line to work with
                    if (single_line[0] ==
                        ' ') {     // at time there are spaces before the actual text, this gets rid of them
                        single_line = strtok_r(single_line, " ", &ptr_save);
                    }

                    record_Letter[0] = single_line[0]; // getting the first char

                    // TODO: rework the H record
                    if ((strcmp(record_Letter, "H") == 0) && (i == 3)) {
                        
                        char* hexValue = malloc(20);
                        memset(hexValue, '\0', 20);
                        int decimal_address = 0;
                        //Column 2 to 7 has the name of the external symbol
                        strncpy(symbol, buffer + 1, (7 - 1));
                        //Column 8 to 13 has the address of the symbols
                        strncpy(address, buffer + 8, (14 - 8));
                        //converting address into decimal representation
                        sscanf(address, "%X", &decimal_address);
                        //calculating new address based on load_point
                        decimal_address += decimal_load_point;
                        //convert address back to hexadecimal
                        sprintf(hexValue, "%X", decimal_address);
                        //store value in address
                        sprintf(address, "%s", hexValue);
                        //storting record type
                        strcpy(type, "H");
                    
                        //add to table
                        AddToTable(externalSymbolTable, symbol, address, type);
                        free(hexValue);
                    }

                    // M-Record
                    else if (strcmp(record_Letter, "M") == 0){
                        /*  M-Record
                         *  0 = M   single_line[0]
                         *  1-6 = starting address of the modification
                         *  7-8 = length of the modification
                         *  9   = + / -
                         *  10-15 = symbol whose value should be added or subtracted
                         * */
                        MSTAB  temp_base;
                        memset(temp_base.address, '\000', 7);
                        memset(temp_base.address_new, '\000', 7);
                        memset(temp_base.len, '\000', 3);
                        memset(temp_base.mod_sym, '\000', 2);
                        memset(temp_base.symbol, '\000', 7);
                        temp_base.next = NULL;

                        int counter_a = 0;
                        int counter_b = 0;
                        int counter_c = 0;
                        unsigned long max = strlen(single_line);
                        // getting M record information
                        for (int j = 1; j < max; j++) {
                            if (j <= 6) {
                                temp_base.address[counter_a] = single_line[j];
                                counter_a++;
                            } else if (j <= 8) {
                                temp_base.len[counter_b] = single_line[j];
                                counter_b++;
                            } else if (j == 9){
                                temp_base.mod_sym[0] = single_line[j];
                            } else if (j <= 16){
                                temp_base.symbol[counter_c] = single_line[j];
                                counter_c++;
                            }
                        }
                        // address relocation
                        // addr to decimal
                        int new_t_addr = (int) strtol(temp_base.address, NULL, 16);
                        char addr_new[7];
                        memset(addr_new, '\000', 7);
                        // addr + reloc
                        new_t_addr += decimal_load_point;
                        // decimal to hex + hex to record
                        sprintf(addr_new, "%X", new_t_addr);

                        // padding
                        int len_new_add = strlen(addr_new);
                        for(int f = 0; f < (6-len_new_add); f++){
                            temp_base.address_new[f] = '0';
                        }
                        // actual addr
                        for(int f = 0; f < len_new_add; f++){
                            temp_base.address_new[(6-len_new_add) + f] = addr_new[f];
                        }

                        MSTAB *temp = NULL;
                        temp = (MSTAB *) malloc(sizeof(MSTAB));
                        *(temp) = temp_base;

                        // add to table
                        AddMRecord(m_table, temp);
                    }

                    // T-Record
                    else if (strcmp(record_Letter, "T") == 0) {
                        /*  T-record
                         *  0 = T   single_line[0]
                         *  1-6 = address   single_line[1-6]    // START addr + local
                         *  7-8 = length    single_line[7-8]
                         *  9-68 = object code in hex   single_line[9-68]
                         * */
                        TSTAB temp_base; // initializer
//                        memset(temp_base.address, '\000', 6);
                        memset(temp_base.address, '\000', 7);
                        memset(temp_base.address_new, '\000', 7);
//                        memset(temp_base.len, '\000', 2);
                        memset(temp_base.len, '\000', 3);
//                        memset(temp_base.data, '\000', 60);
                        memset(temp_base.data, '\000', 61);

                        int counter_a = 0;
                        int counter_b = 0;
                        int counter_c = 0;
                        unsigned long max = strlen(single_line);
                        // getting T record information
                        for (int j = 1; j < max; j++) {
                            if (j <= 6) {
                                temp_base.address[counter_a] = single_line[j];
                                counter_a++;
                            } else if (j <= 8) {
                                temp_base.len[counter_b] = single_line[j];
                                counter_b++;
                            } else if (j <= 68) {
                                temp_base.data[counter_c] = single_line[j];
                                counter_c++;
                            }
                        }

                        // address relocation
                        // addr to decimal
                        int new_t_addr = (int) strtol(temp_base.address, NULL, 16);
                        char addr_new[7];
                        memset(addr_new, '\000', 7);
                        // addr + reloc
                        new_t_addr += decimal_load_point;
                        // decimal to hex + hex to record
                        sprintf(addr_new, "%X", new_t_addr);

                        // padding
                        int len_new_add = strlen(addr_new);
                        for(int f = 0; f < (6-len_new_add); f++){
                            temp_base.address_new[f] = '0';
                        }
                        // actual addr
                        for(int f = 0; f < len_new_add; f++){
                            temp_base.address_new[(6-len_new_add) + f] = addr_new[f];
                        }

                        TSTAB *temp = NULL;
                        temp = (TSTAB *) malloc(sizeof(TSTAB));
                        *(temp) = temp_base;

                        // add to table
                        AddTRecord(t_table, temp);
                    }

                    else if (strcmp(record_Letter, "D") == 0) {
                        int recordD_length = strlen(buffer) - 1;
                        int nameBeginIndex = 1;
                        int nameEndIndex = 7;
                        int addressBeginIndex = 8;
                        int addressEndIndex = 13;
                        int decimal_address;

                        while (addressBeginIndex < recordD_length) {
                            char *hexValue = malloc(20);
                            memset(hexValue, '\0', 20);
                            decimal_address = 0;
                            //Column 2 to 7 has the name of the external symbol
                            strncpy(symbol, buffer + nameBeginIndex, (nameEndIndex - nameBeginIndex));
                            //Column 8 to 13 has the address of the symbols
                            strncpy(address, buffer + addressBeginIndex, (addressEndIndex - addressBeginIndex));
                            //converting address into decimal representation
                            sscanf(address, "%X", &decimal_address);
                            //calculating new address based on load_point
                            decimal_address += decimal_load_point;
                            //convert addreess back to hexadecimal
                            sprintf(hexValue, "%X", decimal_address);
                            //store value in address
                            sprintf(address, "%s", hexValue);
                            //storting record type
                            strcpy(type, "D");

                            //add to table
                            AddToTable(externalSymbolTable, symbol, address, type);

                            nameBeginIndex += 12;
                            nameEndIndex = nameBeginIndex + 5;
                            addressBeginIndex = nameEndIndex + 1;
                            addressEndIndex = addressBeginIndex + 6;

                            free(hexValue);
                        }

                    }//end else if (strcmp(record_Letter, "D") == 0)
                memset(symbol, '\0', 1024 * sizeof(char));
                memset(address, '\0', 1024 * sizeof(char));
                memset(type, '\0', 1024 * sizeof(char));

                }
            }
        }
    }
    // LOGGER THIS IS TEMP
    // ALL OF THE T-RECORD FIELDS ARE ENLARGED BY 1 BECAUSE I AM PRINTING THEM OUT
    // DO NOT CHANGE SINCE IT WILL BREAK THE LOGGER, I WILL FIX IT LATER ON IN THE PROJECT
    int k = 1; // table counter

    for(int x = 0; externalSymbolTable[x]; x++){
        if (strcmp(externalSymbolTable[x]->recordType, "H") == 0){
            printf("%s%s  [Add zeros]%s[here program legth]\n", externalSymbolTable[x]->recordType, externalSymbolTable[x]->Name, externalSymbolTable[x]->address);
        }
        else{
            if (x < 2){
                printf("%s", externalSymbolTable[x]->recordType);
            }
            printf("%s  [Add zeros]%s", externalSymbolTable[x]->Name, externalSymbolTable[x]->address);
        }
    }
    printf("\n");
    while(tTables != NULL){
        while(tTables->table != NULL){
            printf("T%s%s%s\n", tTables->table->address_new, tTables->table->len, tTables->table->data);
            tTables->table = tTables->table->next;
        }
        tTables = tTables->next;
        k++;
    }
    k = 1;
    while(mTables != NULL){
        while(mTables->table != NULL){
            printf("M%s%s%s%s\n", mTables->table->address, mTables->table->len, mTables->table->mod_sym, mTables->table->symbol);
            mTables->table = mTables->table->next;
        }
        mTables = mTables->next;
        k++;
    }
    

    free(load_point);
    fclose(fptr);
    return EXIT_SUCCESS;
}//end of main