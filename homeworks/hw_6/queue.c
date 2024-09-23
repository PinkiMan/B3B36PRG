#include "queue.h"

// TODO - your code



queue_t* create_queue(int capacity){
    queue_t *Queue=malloc(sizeof(queue_t));
    Queue->Buffer_Size=capacity;
    Queue->Head_Index=0;
    Queue->Tail_Index=0;
    Queue->Data_Size=0;
    Queue->Buffer=malloc(capacity*sizeof(void*));
    return Queue;
}


void delete_queue(queue_t *queue){
    free(queue->Buffer);
    free(queue);
}


bool push_to_queue(queue_t *queue, void *data){

    if(queue->Head_Index==queue->Buffer_Size){
        queue->Head_Index=0;
    }
    /*if(queue->Head_Index==queue->Tail_Index && queue->Data_Size!=0){
        return false;
    }*/



    if(queue->Data_Size==queue->Buffer_Size){

        /*for(int i=0;i<queue->Buffer_Size;i++){
            printf("%d ",*((int*)queue->Buffer[i]));
        }
        printf("\n");*/



        void **tmp = malloc(sizeof(void*)*queue->Buffer_Size*2);

        for(int i=0;i<queue->Data_Size;i++){
            tmp[i]=queue->Buffer[(queue->Tail_Index+i)%queue->Buffer_Size];
        }
        //printf("!%d!\n",tmp);

        queue->Buffer_Size*=2;
        free(queue->Buffer);
        queue->Buffer=tmp;

        queue->Tail_Index=0;
        queue->Head_Index=queue->Data_Size;


    }



    queue->Buffer[queue->Head_Index]=data;
    queue->Head_Index++;
    queue->Data_Size++;

    /*for(int i=0;i<queue->Data_Size;i++){
        printf("%d ",*((int*)queue->Buffer[i]));
    }
    printf("\n");*/

    return true;
}


void* pop_from_queue(queue_t *queue){
    if(queue->Head_Index==queue->Tail_Index){
        return NULL;
    }

    if(queue->Tail_Index==queue->Buffer_Size){
        queue->Tail_Index=0;
    }


    if(queue->Data_Size<queue->Buffer_Size/3 && queue->Buffer_Size>=3){
        void **tmp = malloc(sizeof(void*)*queue->Buffer_Size/3);

        for(int i=0;i<queue->Data_Size;i++){
            tmp[i]=queue->Buffer[(queue->Tail_Index+i)%queue->Buffer_Size];
        }


        queue->Buffer_Size/=3;
        free(queue->Buffer);
        queue->Buffer=tmp;

        queue->Tail_Index=0;
        queue->Head_Index=queue->Data_Size;

    }





    queue->Tail_Index++;
    queue->Data_Size--;

    return queue->Buffer[queue->Tail_Index-1];
}


void* get_from_queue(queue_t *queue, int idx){

    if(queue->Data_Size>0 && idx>=0){
        if(queue->Head_Index<queue->Tail_Index){
            if(queue->Tail_Index+idx<queue->Head_Index+queue->Buffer_Size){
                return queue->Buffer[(queue->Tail_Index+idx)%queue->Buffer_Size];
            }
        }

        if(queue->Head_Index>queue->Tail_Index){
            if(queue->Tail_Index+idx<queue->Head_Index){
                return queue->Buffer[queue->Tail_Index+idx];
            }
        }
    }
    return NULL;
}


int get_queue_size(queue_t *queue){
    return queue->Data_Size;
}



