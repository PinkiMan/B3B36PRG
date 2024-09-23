#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char *Data;
    int Len;
} My_String;

My_String Input()
{
    int Size = 16;
    int i = 1;
    char *Mem = malloc(Size * sizeof(char));
    char a = getchar();
    while (a != 10)
    {
        if (a < 'A' || (a > 'Z' && a < 'a') || a > 'z')
        {
            fprintf(stderr, "Error: Chybny vstup!\n");
            exit(100);
        }
        else if (Size == i)
        {
            Size *= 2;
            char *res = realloc(Mem, Size * sizeof(char));
            if (Mem == NULL){
                exit(1);
            }
            else{
                Mem = res;
            }
        }
        Mem[i -1] = a;
        i ++;
        a = getchar();
    }
    My_String ret;
    ret.Data = Mem;
    ret.Len = i;
    return ret;
}


int LD_Distance(My_String Edit, My_String Solution)
{
    int * matrix = malloc((Edit.Len)*(Solution.Len)*sizeof(int));
    for(int k =0; k < (Edit.Len)*(Solution.Len); k++){
        matrix[k]=0;
    }
    int i = 0;
    int j = 0;
    for(int k =0; k < (Edit.Len)*(Solution.Len); k++){
        i++;
        if (i >= Edit.Len){
            i = 0;
            j++;
        }
        if (i==0 && k+1 < Edit.Len*Solution.Len){
            matrix[k+1] = j;
        }
    }
    for (i = 0; i < Edit.Len; i++){
        matrix[i] = i;
    }
    matrix[0] = 0;

    i = 0;
    j = 0;
    int k;
    for(k =0; k < (Edit.Len*Solution.Len); k++){
        i = k%Edit.Len;
        j = k/Edit.Len;
        if (i > 0 && i < Edit.Len && j > 0 && j <Solution.Len){
            int subs = 1;
            if (Edit.Data[i-1] == Solution.Data[j-1])
            {
                subs = 0;
            }

            matrix[k]=matrix[i-1+j*(Edit.Len)] + 1;
            if(matrix[i+(j-1)*(Edit.Len)] + 1<matrix[k]){
                matrix[k]=matrix[i+(j-1)*(Edit.Len)] + 1;
            }
            if(matrix[i-1+(j-1)*(Edit.Len)] + subs<matrix[k]){
                matrix[k]=matrix[i-1+(j-1)*(Edit.Len)] + subs;
            }
        }
    }
    int res = matrix[Edit.Len*Solution.Len-1];
    free(matrix);
    return res;
}

My_String Copy_Str(My_String text)
{
    My_String ret;
    ret.Len = text.Len;
    ret.Data = malloc(text.Len * sizeof(char));
    for (int x = 0; x < text.Len; x++)
    {
        ret.Data[x] = text.Data[x];
    }
    return ret;
}



void Print_Data(My_String *Data){
    for(int i=0;i<Data->Len-1;i++){
        printf("%c", Data->Data[i]);
    }
    printf("\n");
}



int main(int argc, char *argv[])
{
    My_String Cipher = Input();
    My_String Solution = Input();

    My_String Edit = Copy_Str(Cipher);
    My_String Best = Copy_Str(Solution);

    int Min_Count = Cipher.Len;

    for (int Roll = 0; Roll < 52; Roll++)
    {
        char Character;
        for (int j = 0; j < Edit.Len - 1; j++)
        {

            Character = Edit.Data[j] + 1;
            if (Character == 'z'+1)
            {
                Character = 'A';
            }
            if (Character == 'Z'+1)
            {
                Character = 'a';
            }
            Edit.Data[j] = Character;
        }


        int count = LD_Distance(Edit, Solution);
        if (count < Min_Count)
        {
            Min_Count = count;
            free(Best.Data);
            Best = Copy_Str(Edit);
        }

    }

    Print_Data(&Best);

    free(Edit.Data);
    free(Cipher.Data);
    free(Solution.Data);
    free(Best.Data);
    return 0;
}
