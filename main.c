#include "headers.h"

int main(int argc, char* argv[]){

    //File pointer
	FILE *fptr;

    /*Checks that there are more than two parameters.
    /*We need a name for the file, and at least two 
    /*object files that we can link together*/
	if (argc < 3){
		printf("The number of arguments provided is insufficent.");
		return 0;
	}

    /*If first argument contains a '.', 
    /*then a file was provided instead of a filename*/
    char* check;
    check = strstr(argv[0], ".");
    if(check){
        printf("Filename was not provided");
        return 0;
    }

    //store the name of output file
    char file_name = argv[0];

    int number_files = argc - 1;
    int count_files = 1;

    //creating an array of structs
	ESTAB* externalSymbolTable[1024] = {};
    memset(externalSymbolTable, '\0', 1024 * sizeof(struct externalSymbolTable*));

    //address where we are loading the program
    int decimal_load_point = 0;

    char* load_point = malloc(20);
    memset(load_point, 0, 20);
    //storing the value 0(in hex) into load_point
    sprintf(load_point, "%X", decimal_load_point);


    char buffer[1024];
	char bufferFull[1024];

    char* symbol;
	char* address;
    char* type;

    //allocating space to store a symbol
	symbol = malloc(1024 * sizeof(char));
	address = malloc(1024 * sizeof(char));
    type = malloc(1024 * sizeof(char));

	//fills a block of memory with null
	memset(symbol, '\0', 1024 * sizeof(char));
	memset(address, '\0', 1024 * sizeof(char));
    memset(type, '\0', 1024 * sizeof(char));

    //length of H record
    int recordH_length; 

    //continue while there are more files
    while(count_files <= number_files){

        //open file
        fptr = fopen(argv[count_files], "r");
        
        //checks if file exists
        if (fptr == NULL){
            printf("Error: Cannot open file - %s \n", argv[count_files]);
            return 0;
        }

        //reads each line of the file and stores it in buffer
	    while (fgets(buffer, 1024, fptr) != NULL){

            strcpy(bufferFull, buffer);

            //getting start address from H record
            if(strcmp(buffer[0], "H") == 0){
                //store record lentgh
                recordH_length = strlen(buffer);
                //Column 2 to 7 has the name of the program
                strncpy(symbol, buffer + 2, (8 - 2));
                //Store load_point in address
                strcpy(address, load_point);
                //storting record type
                strcpy(type, "H");

                //add to table
                AddToTable(externalSymbolTable, symbol, address, type);

            }
            else if(strcmp(buffer[0], "D") == 0){
                int recordD_length = strlen(buffer) - 1;
                int nameBeginIndex = 2;
                int nameEndIndex = 8;
                int addressBeginIndex = 8;
                int addressEndIndex = 13;
                int decimal_address;

                while(addressEndIndex < recordD_length){
                    char* hexValue = malloc(10);
                    memset(hexValue, 0, 10);

                    //Column 2 to 7 has the name of the program
                    strncpy(symbol, buffer + nameBeginIndex, (nameEndIndex - nameBeginIndex));
                    //Column 8 to 13 has the address of the symbols
                    strncpy(address, buffer + addressBeginIndex, (addressEndIndex - addressBeginIndex));
                    //converting address into decimal representation
                    sscanf(address, "%X", &decimal_address); 
                    //calculating new address based on load_point
                    decimal_address += decimal_load_point;
                    //convert addreess back to hexadecimal
                    sprintf(hexValue,"%X", decimal_address);
                    //store value in address
                    sprintf(address, "%s", hexValue);
                    //storting record type
                    strcpy(type, "D");
                    
                    //add to table
                    AddToTable(externalSymbolTable, symbol, address, type);
                    
                    nameBeginIndex += 12;
                    nameEndIndex = nameBeginIndex + 5;
                    addressBeginIndex += nameEndIndex + 1;
                    addressEndIndex = addressBeginIndex + 5;

                    free(hexValue);
                }
                

            }//end else if (strcmp(buffer[0], "D") == 0)


        }//end while (fgets(buffer, 1024, fptr) != NULL)

        count_files++;
    }//end while (count_files <= number_files)
 

    free(load_point);
    fclose(fptr);
}//end of main

    