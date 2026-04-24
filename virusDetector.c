#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*File DESCRIPTION
virusDetector compares the content of the user-selected FILE byte-by-byte with a pre-  defined set of viruses described in a SIGNATURES file. 
The comparison is done according to a naive   algorithm described in task 2.*/

//given struct:
typedef struct virus {
unsigned short SigSize;
unsigned char* VirusName;
unsigned char* Sig;
} virus;

//helper function 1:
/*this function receives a file pointer and returns a virus* that represents the next virus in the file. 
To read from a file, use fread()
fread reads blocks of data and we can tell it how many bytes to read
========selfNotes===========
1)cannot have var name the same as struct name
2)any * field require malloc
3)NEED TO REMEMBER TO FREE
4)fread first paramter has to be ptr so & for sigSize which isnt ptr
5)if we reached the end i returned null might need to change later
6)fread(where to put info,single item size in bytes,how many items,where to read from)
*/
virus* readVirus(FILE* file){
    virus* newVir = (virus*)malloc(sizeof(virus));
    size_t firstFread = fread(&(newVir->SigSize),1,2,file);
    if(firstFread != 2){
        free(newVir);
        return NULL;
    }
    newVir->VirusName = (unsigned char*)malloc(16);
    fread(newVir->VirusName,1,16,file);
    newVir->Sig = (unsigned char*)malloc(newVir->SigSize);
    fread(newVir->Sig,1,newVir->SigSize,file);
    return newVir;
}

//helper function 2:
/*this function receives a virus and a pointer to an output file.
The function prints the virus to the given output.
It prints the virus name (in ASCII),
the virus signature length (in decimal),
and the virus signature (in hexadecimal representation).
========selfNotes===========
1)we used hexaPrint code for sig array
2)%s means the function expect ptr, d means fprintf expect num
3)for string we dont need loop bc its a convention that string is null
terminated, therfore it looping over the string automatically
*/
void printVirus(virus* virus, FILE* output){
    fprintf(output,"Virus name: %s\n",virus->VirusName);
    fprintf(output,"Virus signature length: %d\n", virus->SigSize);
    fprintf(output,"Virus signature:\n");
    for(int i=0; i<virus->SigSize ;i++){
        fprintf(output,"%02X ",virus->Sig[i]);
    }
    fprintf(output,"\n");
}


int main(int argc, char **argv){
    /*Open the signatures file, check the magic number.
    If the magic number is incorrect(i.e. different from "VIRL"or "VIRB"),then print an error message, close the file, and return.
    If magic number is OK,
    use readVirus in order to read the viruses one-by-one,
    and use printVirus in order to print the virus 
    (to a file or to the standard output, up to your choice).
    ========selfNotes===========
    1)I opened signaturs-L bc linux reads little endian
    2)fopen rb=read binary
    3)here we free malloc from readVirus
    */
    FILE* sigFile = fopen("signatures-L","rb");
    if(sigFile == NULL){
        printf("can't open file\n");
        return 1;
    }
    char magic[4];
    fread(magic,1,4,sigFile);
    if(strncmp(magic,"VIRL",4) !=0 && strncmp(magic,"VIRB",4) !=0){
        printf("Unsupported File - magic word is incorrect\n");
        fclose(sigFile);
        return 1;
    }
    virus* tmp = readVirus(sigFile);
    while(tmp!=NULL){
        printVirus(tmp, stdout);
        free(tmp->VirusName);
        free(tmp->Sig);
        free(tmp);
        tmp = readVirus(sigFile);
    }
    fclose(sigFile);
    return 0;
}


