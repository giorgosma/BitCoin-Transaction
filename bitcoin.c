#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"
#include "functions.h"

int read_input_parameters(int argc, char **argv){
    if (argc != 12)
        return 0;
    else
        return 1;
}

int main(int argc, char **argv){
    // CHECK THE INPUT PARAMETERS
    if(!read_input_parameters*(argc)){
        printf("ERROR:\n \t WRONG INPUT PARAMETERS\n EXITING...\n");
        return 0;
    }
    // CONVERT INPUT PARAMETERS TO INT
    int size_of_sender_hash_table = atoi(argv[8]),
        size_of_receiver_hash_table = atoi(argv[10]),
        bucket_size_of_bytes = atoi(argv[12]),
        bitcoin_value = atoi(argv[6]);
    // CALL READ_FUNCTION
    read_functions(size_of_sender_hash_table, size_of_receiver_hash_table, bucket_size_of_bytes, bitcoin_value, argv[2], argv[4]);
}