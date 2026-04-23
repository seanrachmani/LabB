#include <stdio.h>
#include <stdlib.h>

//receives the name of a binary file as a command-line argument
//prints the hexadecimal value of each byte in the file in sequence to the standard output(printf)
//fread function help us read blocks of data to a buffer 

void PrintHex(unsigned char *buffer, int length)
{
    for(int i=0; i<length ;i++){
        printf("%02X ",buffer[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    unsigned char buffer[2048];
    //read binary
    FILE* file = fopen(argv[1],"rb");
    if(file == NULL)
    {
        printf("file is null\n");
    }
    size_t read = fread(buffer,1,2048,file);
    fclose(file);
    //fread returns the number of items read succesfully so var read has it:
    PrintHex(buffer,read);
}