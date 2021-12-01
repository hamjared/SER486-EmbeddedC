/********************************************************
 * util.c
 *
 * SER486 Project 2
 * Fall 2021
 * Written By:  Jared Ham
 *
 This file implements the util class
 */



/**
update_checksum(unsigned char *data, unsigned int dsize)

 this function updates the checksum value for the vpd data

arguments:
    unsigned char* - buffer containing vpd data
    unsigned int - size of the data

returns:
    nothing

changes:


*/
void update_checksum(unsigned char *data, unsigned int dsize){
    char sum = 0;

     for(int i = 0; i < dsize - 1; i++){
        sum += data[i];
    }

    data[dsize - 1] = 0-sum;
}


/**
update_checksum(unsigned char *data, unsigned int dsize)

 this function checks if checksum is valid

arguments:
    unsigned char* - buffer containing vpd data
    unsigned int - size of the data

returns:
    returns 1 if the sum of all the bytes in the data structure is 0.
Otherwise, returns 0.

changes:


*/
int is_checksum_valid(unsigned char *data, unsigned int dsize){
    char sum = 0;

    for(int i = 0; i < dsize ; i++){
        sum += data[i];
    }

    return sum == 0;
}
