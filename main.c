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

    
    /*NEED TO FIND WHAT THIS MEANS
    /*AND HOW TO DO IT
    /*get load-point from operating system
    /*module-addr = load-point*/

    int number_files = argc - 1;
    int count_files = 1;

    //continue while there are more files
    while(count_files <= number_files){

        //open file
        fptr = fopen(argv[count_files], "r");
        
        //checks if file exists
        if (fptr == NULL){
            printf("Error: Cannot open file - %s \n", argv[count_files]);
            return 0;
        }


        count_files++;
    }
 


    fclose(fptr);
}//end of main