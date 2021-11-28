#include "headers.h"

void help(){
    printf("%-15s ./Linkage_Editor [relocation] [output_file] [file_to_link] [file_to_link]...\n", "Usage");
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
	if (argc < 5){
		printf("The number of arguments provided is insufficient.");
		help();
		return EXIT_FAILURE;
	}

    //*If second argument contains a '.',
    //*then a file was provided instead of a filename
    char* check;
    check = strstr(argv[2], ".");
    if(check != NULL){
        printf("Filename was not provided\n");
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

    char* load_point = malloc(100);
    memset(load_point, '\0', 100);
    //copying relocation address into variable load point
    strcpy(load_point, argv[1]);

    long decimal_load_point;
    //converts address from hex to dec
    sscanf(load_point, "%lX", &decimal_load_point);

    char buffer[1024];
	char bufferFull[1024];

    char* symbol;
	char* address;
    char* type;
    //stores symbol of from H record of the first file 
    char program_symbol[20];
    program_symbol[0] = 0;
    //stores the length of the linked program
    char program_length[20];
    long decimal_program_length = 0;
    long final_program_length = 0;
    //stores address of H record
    char h_record_address[20];
    long decimal_h_record_address = 0;
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
    T_TABLES* tTables = NULL;
    tTables = (T_TABLES*) malloc(sizeof (T_TABLES));
    {
        T_TABLES t_tables_base;
        t_tables_base.table = NULL;
        t_tables_base.next = NULL;
        *(tTables) = t_tables_base;
    }

    // M-Tables
    M_TABLES* mTables = NULL;
    mTables = (M_TABLES*) malloc(sizeof (M_TABLES));
    {
        M_TABLES  m_tables_base;
        m_tables_base.table = NULL;
        m_tables_base.next = NULL;
        *(mTables) = m_tables_base;
    }

    // E-Tables

    E_TABLES* eTables = NULL;
    eTables = (E_TABLES*) malloc(sizeof (E_TABLES));
    {
        E_TABLES  e_tables_base;
        e_tables_base.table = NULL;
        e_tables_base.next = NULL;
        *(eTables) = e_tables_base;
    }



    // will loop through all of the arvs, with 0 being the program call,
    for(int i = 2; i < argc; i++) {
        if (i != 2) {   // getting all of the files to be linked

            TSTAB *t_table = NULL;
            t_table = (TSTAB *) malloc(sizeof(TSTAB));
            {
                // T-Record Table
                TSTAB t_table_base; // initializer
                memset(t_table_base.address, '\000', 7);
                memset(t_table_base.address_new, '\000', 7);
                memset(t_table_base.len, '\000', 3);
                memset(t_table_base.data, '\000', 61);
                *(t_table) = t_table_base;
            }

            T_TABLES *temp_t_tables = NULL;
            temp_t_tables = (T_TABLES *) malloc(sizeof(T_TABLES));
            {
                // Table for T-record table
                T_TABLES temp_t_tables_base;
                temp_t_tables_base.table = t_table;
                temp_t_tables_base.next = NULL;
                *(temp_t_tables) = temp_t_tables_base;
            }

            // Adding table to tables
            AddTTable(tTables, temp_t_tables);

            MSTAB* m_table = NULL;
            m_table = (MSTAB *) malloc(sizeof (MSTAB));
            {
                // M-Table
                MSTAB  m_table_base;    // initializer
                memset(m_table_base.address, '\000', 7);
                memset(m_table_base.address_new, '\000', 7);
                memset(m_table_base.len, '\000', 3);
                memset(m_table_base.mod_sym, '\000', 2);
                memset(m_table_base.symbol, '\000', 7);
                m_table_base.next = NULL;
                *(m_table) = m_table_base;
            }

            M_TABLES  *temp_m_tables = NULL;
            temp_m_tables = (M_TABLES*) malloc(sizeof (M_TABLES));
            {
                // Table for M-Record table
                M_TABLES  temp_m_table_base;
                temp_m_table_base.table = m_table;
                temp_m_table_base.next = NULL;
                *(temp_m_tables) = temp_m_table_base;
            }

            AddMTable(mTables, temp_m_tables);

            ERSTAB* e_table = NULL;
            e_table = (ERSTAB *) malloc(sizeof (ERSTAB));
            {
                // E-Record Table
                ERSTAB  e_table_base;
                memset(e_table_base.address, '\000', 7);
                e_table_base.next = NULL;
                *(e_table) = e_table_base;
            }

            E_TABLES *temp_e_tables = NULL;
            temp_e_tables = (E_TABLES*) malloc(sizeof (E_TABLES));
            {
                // Table for E-Record table
                E_TABLES  temp_e_table_base;
                temp_e_table_base.table = e_table;
                temp_e_table_base.next = NULL;
                *(temp_e_tables) = temp_e_table_base;
            }

            AddETable(eTables, temp_e_tables);


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

                    // H-Record
                    if ((strcmp(record_Letter, "H") == 0)) {
                        memset(program_length, '\0', 10);
                        char* hexValue = malloc(20);
                        memset(hexValue, '\0', 20);
                        long decimal_address = 0;
                        
                        //Column 2 to 7 has the name of the external symbol
                        strncpy(symbol, buffer + 1, (7 - 1));
                        //padding
                        appendToSymbol(symbol, 6, " ", 0);

                        //storing symbol for the M records
                        if (program_symbol[0] == 0){
                            strcpy(program_symbol, symbol);
                        }
                        
                        //Column 8 to 13 has the address of the symbols
                        strncpy(h_record_address, buffer + 7, (14 - 8));
                        //converting address into decimal representation
                        sscanf(h_record_address, "%lX", &decimal_h_record_address);
                        //calculating new address based on load_point 
                        decimal_address = decimal_load_point;
                        //convert address back to hexadecimal
                        sprintf(hexValue, "%lX", decimal_address);
                        //store value in address
                        sprintf(address, "%s", hexValue);
                        //padding
                        appendToSymbol(address, 6, "0", 1);
                        //Column 14 to 19 have the program length
                        strncpy(program_length, buffer + 13, (20 - 14));
                        //converting address into decimal representation
                        sscanf(program_length, "%lX", &decimal_program_length);
                        //storting record type
                        strcpy(type, "H");
                        
                        //checks that addresss do not exceed SIC XE limit
                        int result = checkMemory(decimal_address);
                        if (result == 1){
                            printf("ERROR: Memory limit exceeded");
                            return EXIT_FAILURE;
                        }
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
                        long new_t_addr = (long) strtol(temp_base.address, NULL, 16);
                        char addr_new[7];
                        memset(addr_new, '\000', 7);
                        // addr + reloc
                        new_t_addr = (new_t_addr - decimal_h_record_address) + final_program_length + decimal_load_point;
                        // decimal to hex + hex to record
                        sprintf(addr_new, "%lX", new_t_addr);

                        // padding
                        int len_new_add = strlen(addr_new);
                        for(int f = 0; f < (6-len_new_add); f++){
                            temp_base.address_new[f] = '0';
                        }
                        // actual addr
                        for(int f = 0; f < len_new_add; f++){
                            temp_base.address_new[(6-len_new_add) + f] = addr_new[f];
                        }

                        long check_addr = (long) strtol(temp_base.address_new, NULL, 16);
                        int result = checkMemory(check_addr);
                        if (result == 1){
                            printf("ERROR: Memory limit exceeded\n");
                            return EXIT_FAILURE;
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
                        memset(temp_base.address, '\000', 7);
                        memset(temp_base.address_new, '\000', 7);
                        memset(temp_base.len, '\000', 3);
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
                        long new_t_addr = (long) strtol(temp_base.address, NULL, 16);
                        char addr_new[7];
                        memset(addr_new, '\000', 7);
                        // addr + reloc
                        new_t_addr = (new_t_addr - decimal_h_record_address) + final_program_length + decimal_load_point;
                        // decimal to hex + hex to record
                        sprintf(addr_new, "%lX", new_t_addr);

                        // padding
                        int len_new_add = strlen(addr_new);
                        for(int f = 0; f < (6-len_new_add); f++){
                            temp_base.address_new[f] = '0';
                        }
                        // actual addr
                        for(int f = 0; f < len_new_add; f++){
                            temp_base.address_new[(6-len_new_add) + f] = addr_new[f];
                        }

                        long check_addr = (long) strtol(temp_base.address_new, NULL, 16);
                        int result = checkMemory(check_addr);
                        if (result == 1){
                            printf("ERROR: Memory limit exceeded\n");
                            return EXIT_FAILURE;
                        }

                        TSTAB *temp = NULL;
                        temp = (TSTAB *) malloc(sizeof(TSTAB));
                        *(temp) = temp_base;

                        // add to table
                        AddTRecord(t_table, temp);
                    }

                    // D-Record
                    else if (strcmp(record_Letter, "D") == 0) {
                        int recordD_length = strlen(buffer) - 1;
                        int nameBeginIndex = 1;
                        int nameEndIndex = 7;
                        int addressBeginIndex = 8;
                        int addressEndIndex = 13;
                        long decimal_address;

                        while (addressBeginIndex < recordD_length) {
                            char *hexValue = malloc(20);
                            memset(hexValue, '\0', 20);
                            decimal_address = 0;
                            //Column 2 to 7 has the name of the external symbol
                            strncpy(symbol, buffer + nameBeginIndex, (nameEndIndex - nameBeginIndex));
                            //padding
                            appendToSymbol(symbol, 6, " ", 0);
                            //Column 8 to 13 has the address of the symbols
                            strncpy(address, buffer + addressBeginIndex, (addressEndIndex - addressBeginIndex));
                            //converting address into decimal representation
                            sscanf(address, "%lX", &decimal_address);
                            //calculating new address based on load_point and offsets
                            decimal_address = (decimal_address - decimal_h_record_address) + final_program_length + decimal_load_point;
                            //convert addreess back to hexadecimal
                            sprintf(hexValue, "%lX", decimal_address);
                            //store value in address
                            sprintf(address, "%s", hexValue);
                            //padding
                            appendToSymbol(address, 6, "0", 1);
                            //storting record type
                            strcpy(type, "D");

                            int result = checkMemory(decimal_address);
                            if (result == 1){
                                printf("ERROR: Memory limit exceeded\n");
                                return EXIT_FAILURE;
                            }
                            //add to table
                            AddToTable(externalSymbolTable, symbol, address, type);

                            nameBeginIndex += 12;
                            nameEndIndex = nameBeginIndex + 5;
                            addressBeginIndex = nameEndIndex + 1;
                            addressEndIndex = addressBeginIndex + 6;

                            free(hexValue);
                        }

                    }//end else if (strcmp(record_Letter, "D") == 0)

                    // E-Record
                    else if (strcmp(record_Letter, "E") == 0){
                        /*  E-Record
                         *  [0] = E
                         *  [1-6] = Address (Or nothing)
                         * */

                        ERSTAB temp_base;
                        memset(temp_base.address, '\000', 7);
                        char temp[7];
                        memset(temp, '\000', 7);
                        temp_base.next = NULL;
                        long decimal_address = 0;

                        int counter_a = 0;
                        unsigned long max = strlen(single_line);

                        // getting E-Record info
                        for (int j = 1; j < max; j++) {
                            if (j <= 6) {
                                temp_base.address[counter_a] = single_line[j];
                                counter_a++;
                            }
                        }
                        strcpy(temp, temp_base.address);

                        // padding
                        int len_addr = strlen(temp);
                        for(int f = 0; f < (6-len_addr); f++){
                            temp[f] = '0';
                        }
                        // actual addr
                        for(int f = 0; f < len_addr; f++){
                            temp[(6-len_addr) + f] = temp_base.address[f];
                        }
                        strcpy(temp_base.address, temp);

                        //converting address into decimal representation
                        sscanf(temp_base.address, "%lX", &decimal_address);
                        //calculating new address based on load_point and offsets
                        decimal_address = (decimal_address - decimal_h_record_address) + decimal_load_point;
                        //convert addreess back to hexadecimal
                        sprintf(temp_base.address, "%lX", decimal_address);
                        //padding
                        appendToSymbol(temp_base.address, 6, "0", 1);

                        ERSTAB *temp_ = NULL;
                        temp_ = (ERSTAB *) malloc(sizeof(ERSTAB));
                        *(temp_) = temp_base;

                        // add to table
                        AddERecord(e_table, temp_);
                    }

                memset(symbol, '\0', 1024 * sizeof(char));
                memset(address, '\0', 1024 * sizeof(char));
                memset(type, '\0', 1024 * sizeof(char));

                }
            }
            fclose(fptr);
            fptr = NULL;
        }
        //Before the program opens the next file, it will add the program length of the previous file 
        //to calculate the address offsets. When calculating addresses of the next program, the
        //addresses will be calculated based on where the previous program left off.
        final_program_length += decimal_program_length;
    }

    //convert final program length into hex
    sprintf(program_length, "%lX", final_program_length);
    //padding
    appendToSymbol(program_length, 6, "0", 1);

    //creating and opening file
    FILE* new_file;
    char output[40];
    char extension[5];
    strcpy(extension, ".obj");
    sprintf(output, "%s%s", file_name, extension);
    new_file = fopen(output, "w");

    //copying to file
    for(int x = 0; externalSymbolTable[x]; x++){
        if ((strcmp(externalSymbolTable[x]->recordType, "H") == 0) && (x == 0)){
            fputs(externalSymbolTable[x]->recordType, new_file); 
            fputs(externalSymbolTable[x]->Name, new_file); 
            fputs(externalSymbolTable[x]->address, new_file); 
            fputs(program_length, new_file); 
            fputs("\n", new_file); 
        }
        else if (strcmp(externalSymbolTable[x]->recordType, "D") == 0){
            if (x < 2){
                fputs(externalSymbolTable[x]->recordType, new_file); 
            }
            fputs(externalSymbolTable[x]->Name, new_file); 
            fputs(externalSymbolTable[x]->address, new_file); 
        }
    }
    fputs("\n", new_file);

    // copying to file (T-Record)
    // temp_t and temp are declared inside the loops so that they would be disposed off after loops are over
    while(tTables != NULL){
        T_TABLES* temp_t = NULL;
        while(tTables->table != NULL){
            TSTAB* temp = NULL;
            fputs("T", new_file);
            fputs(tTables->table->address_new, new_file);
            fputs(tTables->table->len, new_file);
            fputs(tTables->table->data, new_file);
            fputs("\n", new_file);

            // cleaning memory
            if(tTables->table->next != NULL){
                temp =  tTables->table->next;
                free(tTables->table);
                tTables->table = NULL;

                tTables->table = temp;
            }
            else{
                free(temp);
                temp = NULL;
                free(tTables->table);
                tTables->table = NULL;
            }
        }
        // cleaning memory
        if(tTables->next != NULL){
            temp_t = tTables->next;
            free(tTables);
            tTables = NULL;

            tTables = temp_t;
        }
        else{
            free(temp_t);
            temp_t = NULL;
            free(tTables);
            tTables = NULL;
        }
    }

    //copying to file   (M-Record)
    while(mTables != NULL){
        M_TABLES* temp_t = NULL;
        while(mTables->table != NULL){
            MSTAB * temp = NULL;
            fputs("M", new_file);
            fputs(mTables->table->address_new, new_file);
            fputs(mTables->table->len, new_file);
            fputs(mTables->table->mod_sym, new_file);
            fputs(program_symbol, new_file);
            fputs("\n", new_file);

            // cleaning memory
            if(mTables->table->next != NULL){
                temp =  mTables->table->next;
                free(mTables->table);
                mTables->table = NULL;

                mTables->table = temp;
            }
            else{
                free(temp);
                temp = NULL;
                free(mTables->table);
                mTables->table = NULL;
            }
        }
        // cleaning memory
        if(mTables->next != NULL){
            temp_t = mTables->next;
            free(mTables);
            mTables = NULL;

            mTables = temp_t;
        }
        else{
            free(temp_t);
            temp_t = NULL;
            free(mTables);
            mTables = NULL;
        }
    }


    //copying to file
    fputs("E", new_file);
    fputs(eTables->table->address, new_file);
    fputs("\n", new_file);

    // cleaning memory
    while(eTables != NULL){
        E_TABLES* temp_t = NULL;
        while(eTables->table != NULL){
            ERSTAB* temp = NULL;

            if(eTables->table->next != NULL){
                temp =  eTables->table->next;
                free(eTables->table);
                eTables->table = NULL;

                eTables->table = temp;
            }
            else{
                free(temp);
                temp = NULL;
                free(eTables->table);
                eTables->table = NULL;
            }
        }
        // cleaning memory
        if(eTables->next != NULL){
            temp_t = eTables->next;
            free(eTables);
            eTables = NULL;

            eTables = temp_t;
        }
        else{
            free(temp_t);
            temp_t = NULL;
            free(eTables);
            eTables = NULL;
        }
    }
    
    //free external symbol table
    for(int i=0; externalSymbolTable[i]; i++){
		free(externalSymbolTable[i]);
        externalSymbolTable[i] = NULL;
	}

    free(type);
    type = NULL;
    free(address);
    address = NULL;
    free(symbol);
    symbol = NULL;

    free(load_point);
    load_point = NULL;

    fclose(new_file);
//    free(new_file);
    new_file = NULL;

    return EXIT_SUCCESS;
}//end of main