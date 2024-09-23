#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>

// TODO - your code

typedef struct Node{
    int value;
    struct Node *next;
} Linked_List;

Linked_List *Head=NULL;

int Size=0;

_Bool push(int entry){

    if(entry>=0){
        Linked_List *new_entry = malloc(sizeof(Linked_List));
        if(Head==NULL){
            Head=new_entry;
            Head->value=entry;
            Head->next=NULL;
        }else{
            Linked_List *Actual;
            Actual=Head;

            while(Actual->next!=NULL){
                Actual=Actual->next;
            }
            new_entry->value = entry;
            new_entry->next=NULL;
            Actual->next=new_entry;
        }

        Size++;
        return true;
    }
    return false;
}

int pop(void){
    if(Size!=0){
        int Val=Head->value;
        Linked_List *New_Head;
        New_Head=Head->next;

        free(Head);
        Head=New_Head;

        Size--;
        if(Size==0){
            Head=NULL;
        }
        return Val;
    }else{
        return -1;
    }

}

_Bool insert(int entry){
    Linked_List *Actual;
    Linked_List *New;

    if(Head==NULL){
            Linked_List *new_entry = malloc(sizeof(Linked_List));
            Head=new_entry;
            Head->value=entry;
            Head->next=NULL;
            Size++;

            return true;
    }else{

        Actual=Head;

        if(Actual->value<entry){
            Linked_List *new_entry = malloc(sizeof(Linked_List));
            new_entry->value=entry;
            new_entry->next=Head;
            Head=new_entry;
            Size++;
            return true;
        }

        _Bool Run=false;

        while(Actual->next!=NULL){
            New=Actual->next;
            if(New->value<=entry){
                Linked_List *new_entry = malloc(sizeof(Linked_List));
                new_entry->value=entry;
                new_entry->next=New;
                Actual->next=new_entry;
                Run=true;
                break;
            }

            Actual=New;
        }
        if(Run==false){
            Linked_List *new_entry = malloc(sizeof(Linked_List));
            new_entry->value=entry;
            new_entry->next=NULL;
            Actual->next=new_entry;
        }


        Size++;
        return true;

    }
}


_Bool erase(int entry){
    Linked_List *Actual;
    Linked_List *New;

    New=Head;
    Actual=Head;
    Linked_List * Last;

    Last=Head;
    _Bool Found=false;

    while(New!=NULL){
        //printf("xd\n");
        New=Actual->next;
        if(Actual->value==entry){
            //printf("Last: V:%d, N-V:%d",Last->value,Last->next->value);
            //printf("2xd\n");
            Found=true;
            if(Actual==Head){
                free(Actual);
                Head=New;
            }else{
                Last->next=New;
                free(Actual);
            }
            Size--;
        }else{
            Last=Actual;
        }

        Actual=New;
    }

    return Found;
}

int getEntry(int idx){
    if(idx>=0 && idx<Size){
        Linked_List *Actual;
        Actual=Head;
        for(int i=0;i<idx;i++){
            Actual=Actual->next;
        }
        return Actual->value;
    }
    return -1;
}

int size(void){
    return Size;
}

void clear(){

    while(Size!=0){
        //printf("%d ",Size);
        pop();
    }
}

