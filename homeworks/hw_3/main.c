#include <stdio.h>
#include <stdlib.h>


#define WRONG_CHAR 100          //if error with wrong char occures
#define DIFF_LEN 101            //if error with different length of array and key
#define NUM_OF_OFFSETS 52       //number of offsets that can be before cycling



typedef struct{
    int Size;       //size of array
    char *Array;    //array for cipher
    char *Key;      //array for key
    int Index;      //index for cipher
    int Index_Key;  //index for key
}CIPHER;


void Exit_Function(int Error_Value){            //function for exit codes and error codes
    if(Error_Value==WRONG_CHAR){
        fprintf(stderr,"Error: Chybny vstup!\n");
        exit(WRONG_CHAR);
    }else if(Error_Value==DIFF_LEN){
        fprintf(stderr,"Error: Chybna delka vstupu!\n");
        exit(DIFF_LEN);
    }
}


void Free_Function(CIPHER Cipher){              //function that frees memory taken by arrays 
    free(Cipher.Array);
    free(Cipher.Key);
}


CIPHER Get_Input(CIPHER Cipher){                //function to get input data from user
    char Character;
    do{
        Character='0';
        int Input_Error = scanf("%c",&Character);       //scan character from input

        if(((Character>='a') && (Character<='z'))||((Character>='A')&&(Character<='Z'))){
            if(Cipher.Index==Cipher.Size-1){            //if array is too small double it and realocate
                Cipher.Size*=2;
                Cipher.Array=realloc(Cipher.Array,Cipher.Size*sizeof(char));
                Cipher.Key=realloc(Cipher.Key,Cipher.Size*sizeof(char));
            }

            Cipher.Array[Cipher.Index]=Character;       //add new letter from input to array
            Cipher.Index++;
        }else if(Character!='\n' || Input_Error!=1){    //if letter is not valid end the program
            Free_Function(Cipher);
            Exit_Function(WRONG_CHAR);
        }
    }while(Character!='\n');        //exit when input ends


    do{
        Character='0';
        int Input_Error = scanf("%c",&Character);
        
        if(((Character>='a') && (Character<='z'))||((Character>='A')&&(Character<='Z'))){
            if(Cipher.Index_Key==Cipher.Size-1){            //if key is too small double it and realocate
                Cipher.Size*=2;
                Cipher.Key=realloc(Cipher.Key,Cipher.Size*sizeof(char));
            }

            Cipher.Key[Cipher.Index_Key]=Character;         //add new letter from input to array
            Cipher.Index_Key++;
        }else if(Character!='\n' || Input_Error!=1){        //if letter is not valid end the program
            Free_Function(Cipher);
            Exit_Function(WRONG_CHAR);
        }
    }while(Character!='\n');            //exit when input ends

    if(Cipher.Index!=Cipher.Index_Key){             //if array and key are not the same length exit code with error
        Free_Function(Cipher);
        Exit_Function(DIFF_LEN);
    }
    return Cipher;
}


char rotate(char original, int offset){     //Rotates character by length of offset
    char Moved=(char)((int)original+offset);        //move letter by offset

    char Moved_Lower_To_Upper=(char)((int)original+offset-58);      //move letter by offset from lower to upper case
    char Moved_Lower_To_Lower=(char)((int)original+offset-52);      //move letter by offset from lower to lower case

    char Moved_Upper_To_Lower=(char)((int)original+offset+6);       //move letter by offset from upper to lower case
    char Moved_Upper_To_Upper=(char)((int)original+offset-52);      //move letter by offset from upper to upper case

    char new_letter='0';

    if((original>='a')&&(original<='z')){       //rotate letter to new character
        if((Moved>='a')&&(Moved<='z')){
            new_letter=Moved;
        }
        else if((Moved_Lower_To_Upper>='A') && (Moved_Lower_To_Upper<='Z')){        
            new_letter=Moved_Lower_To_Upper;
        }
        else if((Moved_Lower_To_Lower>='a')&&(Moved_Lower_To_Lower<='z')){
            new_letter=Moved_Lower_To_Lower;
        }
    }
    else if((original>='A')&&(original<='Z')){
        if((Moved>='A')&&(Moved<='Z')){
            new_letter=Moved;
        }
        else if((Moved_Upper_To_Lower>='a')&&(Moved_Upper_To_Lower<='z')){
            new_letter=Moved_Upper_To_Lower;
        }
        else if((Moved_Upper_To_Upper>='A')&&(Moved_Upper_To_Upper<='Z')){
            new_letter=Moved_Upper_To_Upper;
        }
    }
    return new_letter;
}


int Get_Best_Offset(CIPHER Cipher){         //Finds the best fitting offset of cipher for key
    int Best_Count=0;
    int Best_Offset=0;
    for(int Offset=0;Offset<=NUM_OF_OFFSETS;Offset++){      //Cycle of every possible offset 
        int Count=0;
        for(int i=0;i<Cipher.Index;i++){        //Cycle of every character in Cipher
            if(rotate(Cipher.Array[i],Offset)==Cipher.Key[i]){      //Checks if rotated character is same as in Key
                Count++;
            }
        }
        if(Count>Best_Count){       //Overwrtites the best offset if found 
            Best_Count=Count;
            Best_Offset=Offset;
        }
    }
    return Best_Offset;
}


int main()
{
    CIPHER Cipher;      //Init of structure for cipher data

    Cipher.Size=2;      //Set number of elements in array
    Cipher.Array=malloc(Cipher.Size*sizeof(char));      //Allocation of Cipher Array
    Cipher.Key=malloc(Cipher.Size*sizeof(char));        //Allocation of Key Array
    Cipher.Index=0;         //Actual_index of Array
    Cipher.Index_Key=0;     //Actual index of Key         

    Cipher=Get_Input(Cipher);

    int Best_Offset=Get_Best_Offset(Cipher);        //get best offset to match key

    for(int i=0;i<Cipher.Index;i++){                //Print Cipher with offset
        printf("%c",rotate(Cipher.Array[i],Best_Offset));
    }
    printf("\n");

    Free_Function(Cipher);      //free alocated memory
    return 0;
}
