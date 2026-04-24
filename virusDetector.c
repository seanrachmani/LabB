/*DESCRIPTION
virusDetector compares the content of the user-selected FILE byte-by-byte with a pre-  defined set of viruses described in a SIGNATURES file. 
The comparison is done according to a naive   algorithm described in task 2.*/

//given struct:
typedef struct virus {
unsigned short SigSize;
unsigned char* VirusName;
unsigned char* Sig;
} virus;

//helper functions
/*this function receives a file pointer and returns a virus* that represents the next virus in the file. 
To read from a file, use fread()
fread reads blocks of data and we can tell it how many bytes to read*/
/*========selfNotes===========
1)cannot have var name the same as struct name
2)any * field require malloc
3)NEED TO REMEMBER TO FREE
4)fread first paramter has to be ptr so & for sigSize which isnt ptr
5)if we reached the end i returned null might need to change later
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

