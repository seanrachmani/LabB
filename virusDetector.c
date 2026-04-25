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

//given struct 1b:
typedef struct link link;
struct link {
link* nextVirus;
virus* vir;
};


/*==============================part1a====================================*/
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
    fprintf(output,"Virus size: %d\n", virus->SigSize);
    fprintf(output,"signature:\n");
    for(int i=0; i<virus->SigSize ;i++){
        fprintf(output,"%02X ",virus->Sig[i]);
        //Start: the following if condition is taken from Gemini:
        if((i+1) % 20 ==0 && (i+1)!=virus->SigSize){
        //End of taken code
            fprintf(output,"\n");
        }   
    }
 
    fprintf(output,"\n\n");
}

/*==============================part1b====================================*/
//function 1:
/*Print the data of every link in list to the given stream.
Each item followed by a newline character.
========selfNotes===========
1)pointers arithmetic wont work since its on the heap and the links might be in different places on the heap
*/
void list_print(link* virus_list, FILE* file){
    while(virus_list != NULL){
        printVirus(virus_list->vir,file);
        virus_list = virus_list->nextVirus;
    }
}


//function 2:
/*Add a new link with the given data to the list(at the end CAN ALSO AT BEGINNING),
and return a pointer to the list(i.e., the first link in the list).
If the list is null - create a new entry and return a pointer to the entry.
========selfNotes===========
1)malloc for new link - REMEMBER TO FREE
2)added to the beginning
3)if virus list is null the code still good bc we did newlink.next = null
*/
link* list_append(link* virus_list, virus* data){
    link* newLink = (link*)malloc(sizeof(link));
    newLink->vir = data;
    newLink->nextVirus = virus_list;
    return newLink;
}


//function 3:
/*Free the memory allocated by the list.
========selfNotes===========
1)when we added virus to the list we transfer ownership
  therefore need to free virus here. 
2)no need for malloc for nextcopy since its only other name for exist memory
*/
void list_free(link* virus_list){
    link* nextCopy;
    while(virus_list != NULL){
        free(virus_list->vir->VirusName);
        free(virus_list->vir->Sig);
        free(virus_list->vir);
        nextCopy = virus_list->nextVirus;
        free(virus_list);
        virus_list = nextCopy;
    }
}


//helper functions for part 2b menu
/*<L>oad signatures
gets an empty LL,load viruses to LL and returns the updated LL
if the magic word is incorrect/ cant open file, return the previously loaded list
========selfNotes===========
1)didnt understand fully:
in load, were changing ptr of the LL,which transfered by "refrence" differnet from main ptr address. 
therefore we need to return LL otherwise the changed only rleeevant for load scope
2)combination of fgets and sscanf help us read input without \n
*/
link* load(link* virList){
    fprintf(stdout,"please enter signature file name\n");
    char fileNameBuff[256];
    char fileName[256];
    fgets(fileNameBuff,sizeof(fileNameBuff),stdin);
    //no & bc string already a ptr:
    sscanf(fileNameBuff,"%s",fileName);
    FILE* sigFile = fopen(fileName,"rb");
    if(sigFile == NULL){
        printf("can't open file\n");
        return virList;
    }
    char magic[4];
    fread(magic,1,4,sigFile);
    if(strncmp(magic,"VIRL",4) !=0 && strncmp(magic,"VIRB",4) !=0){
        printf("Unsupported File - magic word is incorrect\n");
        fclose(sigFile);
        return virList;
    }
    virus* tmp = readVirus(sigFile);
    while(tmp!=NULL){
        virList = list_append(virList,tmp);
        tmp = readVirus(sigFile);
    }
    fclose(sigFile);
    return virList;
}

/*<S>elect file to inspect
gets char[] array, 
and put file to inspect name in it
========selfNotes===========
cant decalre char[] and return it from this scope sisnce its on the stack.
*/
void undefaultSelect(char* fileToInspect){
    fprintf(stdout,"please select file to inspect\n");
    char buffer[256];
    fgets(buffer,sizeof(buffer),stdin);
    //no & bc string already a ptr:
    sscanf(buffer,"%s",fileToInspect);
}

/*<D>etect viruses
gets fileToInspect Name, our saved LL with viruses,
fread contents, activate detect_virus
========selfNotes===========
1)fread(where to put info,single item size in bytes,how many items,where to read from)
*/
void detect(char* fileToInspect,link* virList){
    if(fileToInspect[0] == '\0'){
        fprintf(stdout,"file was not selected\n");
        return;
    }
    FILE* file = fopen(fileToInspect,"rb");
    if(file==NULL){
        fprintf(stdout,"cant read the file\n");
        return;
    }
    char buffer[10000];
    unsigned int size = fread(buffer,1,10000,file);
    detect_virus(buffer,size,virList);
    fclose(file);
}


/*
========selfNotes===========
1)memcmp(ptr1,ptr2,size to compare)
*/
void detect_virus(char* buffer, unsigned int size, link* virus_list){
    size_t startingByte;
    for(int i=0; i<size; i++){
        link* current_list = virus_list;
        while(current_list!=NULL){
            if(memcmp(&buffer[i],current_list->vir->sig,current_list->vir->SigSize) == 0){
                fprintf(stdout,"sarting byte location in the suspected file: %d \n",i);
                fprintf(stdout,"virus name: %s\n",current_list->vir->VirusName);
                fprintf(stdout,"the size of the virus signature: %d",current_list->vir->SigSize);
            }
            current_list = current_list->nextVirus;
        }
    }
}

/*<F>ix file*/
void fix(){
    printf("Not implemented\n");
}





int main(int argc, char **argv){
    /*
    ========selfNotes===========
    1)I opened signaturs-L bc linux reads little endian
    2)fopen rb=read binary
    3)here we free malloc from readVirus
    4)fgets reads line of text, sscanf help store info from string to variables(help us ignores \n)
    */
    char[] fileToInspect = NULL;
    link* virList = NULL;
    char fileToInspect[256];
    while(1){
        fprintf(stdout,"Select operation from the following menu by index:\n"
                        "<L>oad signatures\n<P>rint signatures\n"
                        "<S>elect file to inspect\n<D>etect viruses\n"
                        "<F>ix file\n<Q>uit\n");
        char buffer[1024];
        char choice;
        fgets(buffer,sizeof(buffer),stdin);
        sscanf(buffer,"%c",&choice);
        if(choice=='L'){
            virList = load(virList);
        }
        if(choice=='P'){
            list_print(virList,stdout);
        }
        if(choice=='S'){
            undefaultSelect(fileToInspect);
        }
        if(choice=='D'){
            detect(fileToInspect,virList);
        }
        if(choice=='F'){
            fix();
        }
        if(choice=='Q'){
            list_free(virList);
            exit(0);
        }
    }
return 0;
}


