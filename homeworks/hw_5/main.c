#include <stdio.h>
#include <stdlib.h>

#define MAX_MATRIXES 100
#define WRONG_INPUT 100
#define SUCCES 0


typedef struct{
    int Rows;
    int Columns;
    int *Array;
}MATRIX;


typedef struct{
    void **Array;
    int Size;
}POINTERS;


void Free_Function(POINTERS Pointers){  //Frees all malloc array
    for(int i=0;i<Pointers.Size;i++){
        free(Pointers.Array[i]);
    }
    free(Pointers.Array);
}


void Exit_Function(POINTERS Pointers){  //Exits program
    Free_Function(Pointers);
    fprintf(stderr, "Error: Chybny vstup!\n");
    exit(WRONG_INPUT);
}


MATRIX Input(POINTERS Pointers){    //Gets input from standard input
    MATRIX Matrix;

    if(scanf("%d", &Matrix.Rows)!=1){
        Exit_Function(Pointers);
    }

    if(scanf("%d", &Matrix.Columns)!=1){
        Exit_Function(Pointers);
    }

    Matrix.Array=malloc(Matrix.Rows*Matrix.Columns*sizeof(int));
    if(Matrix.Array==NULL){
        Exit_Function(Pointers);
    }
    Pointers.Array[Pointers.Size]=Matrix.Array;

    for(int i=0;i<Matrix.Rows*Matrix.Columns;i++){
        if(scanf("%d%*c",&Matrix.Array[i])!=1){
            Exit_Function(Pointers);
        }
    }

    return Matrix;
}


MATRIX Add_Matrix(POINTERS Pointers, MATRIX Matrix1,MATRIX Matrix2){        //Adds two matrixes and returns output
    if(Matrix1.Columns!=Matrix2.Columns || Matrix1.Rows!=Matrix2.Rows){
        Exit_Function(Pointers);
    }

    MATRIX Matrix3;
    Matrix3.Columns=Matrix1.Columns;
    Matrix3.Rows=Matrix1.Rows;
    Matrix3.Array=malloc(Matrix3.Rows*Matrix3.Columns*sizeof(int));

    if(Matrix3.Array==NULL){
        Exit_Function(Pointers);
    }

    for(int i=0;i<Matrix3.Rows*Matrix3.Columns;i++){
        Matrix3.Array[i]=Matrix1.Array[i]+Matrix2.Array[i];
    }

    free(Matrix2.Array);
    return Matrix3;
}


MATRIX Sub_Matrix(POINTERS Pointers, MATRIX Matrix1,MATRIX Matrix2){        //Subtracts two matrixes and returns output
    if(Matrix1.Columns!=Matrix2.Columns || Matrix1.Rows!=Matrix2.Rows){
        Exit_Function(Pointers);
    }

    MATRIX Matrix3;
    Matrix3.Columns=Matrix1.Columns;
    Matrix3.Rows=Matrix1.Rows;
    Matrix3.Array=malloc(Matrix3.Rows*Matrix3.Columns*sizeof(int));

    if(Matrix3.Array==NULL){
        Exit_Function(Pointers);
    }

    for(int i=0;i<Matrix3.Rows*Matrix3.Columns;i++){
        Matrix3.Array[i]=Matrix1.Array[i]-Matrix2.Array[i];
    }

    free(Matrix2.Array);
    return Matrix3;
}


MATRIX Mul_Matrix(POINTERS Pointers, MATRIX Matrix1,MATRIX Matrix2){        //Multiply two matrixes and returns output
    if(Matrix1.Columns!=Matrix2.Rows){
        Exit_Function(Pointers);
    }

    MATRIX Matrix3;
    Matrix3.Rows=Matrix1.Rows;
    Matrix3.Columns=Matrix2.Columns;
    Matrix3.Array=malloc(Matrix3.Rows*Matrix3.Columns*sizeof(int));

    if(Matrix3.Array==NULL){
        Exit_Function(Pointers);
    }

    for(int y=0;y<Matrix3.Rows;y++){
        for(int x=0;x<Matrix3.Columns;x++){
            Matrix3.Array[y*Matrix3.Columns+x]=0;
            for(int i=0;i<Matrix1.Columns;i++){
                Matrix3.Array[y*Matrix3.Columns+x]+=Matrix1.Array[y*Matrix1.Columns+i]*Matrix2.Array[i*Matrix2.Columns+x];
            }
        }
    }

    free(Matrix2.Array);
    return Matrix3;
}


void print(int *arr, int rows,int columns){     //Prints input array
    printf("%d %d\n",rows,columns);
    for(int y=0;y<rows;y++){
        for(int x=0;x<columns;x++){
            printf("%d",arr[y*columns+x]);
            if(x<columns-1){
                printf(" ");
            }
        }
        printf("\n");
    }
}


int Is_Mul(char *Signs,int Size){       //Checks if * is in array
    int mul=0;
    for(int i=0;i<Size-1;i++){
        if(Signs[i]=='*'){
            mul=1;
            break;
        }
    }
    return mul;
}





int main()
{
    POINTERS Pointers;
    Pointers.Size=0;
    Pointers.Array=malloc(MAX_MATRIXES*sizeof(int*));

    MATRIX Matrixes[MAX_MATRIXES];
    char Signs[MAX_MATRIXES];

    char sign;

    while(1){       //Get all Inputs
        Matrixes[Pointers.Size]=Input(Pointers);
        sign='a';   //random char elimination
        int err=scanf("%c",&sign);

        Signs[Pointers.Size]=sign;
        Pointers.Size++;

        if(err!=1){
            if(err==-1){
                break;
            }
            Exit_Function(Pointers);
        }
    }


    while(Is_Mul(Signs,Pointers.Size)==1){      //Solve all *
        int mul=0;
        for(int i=0;i<Pointers.Size-1;i++){
            if(Signs[i]=='*'){
                mul=i;
                Signs[i]='0';
                break;
            }
        }

        Matrixes[mul+1]=Mul_Matrix(Pointers,Matrixes[mul],Matrixes[mul+1]);
        Pointers.Array[mul+1]=Matrixes[mul+1].Array;
        Matrixes[mul].Columns=0;
        Matrixes[mul].Rows=0;

    }

    for(int i=0;i<Pointers.Size-1;i++){     //Solve all + and -
        if(Matrixes[i].Columns!=0){
            int n=1;
            while(Matrixes[i+n].Columns==0){
                n++;
            }

            if(Signs[i]=='+'){
                Matrixes[i+n]=Add_Matrix(Pointers,Matrixes[i],Matrixes[i+n]);
                Pointers.Array[i+n]=Matrixes[i+n].Array;
            }
            else if(Signs[i]=='-'){
                Matrixes[i+n]=Sub_Matrix(Pointers,Matrixes[i],Matrixes[i+n]);
                Pointers.Array[i+n]=Matrixes[i+n].Array;
            }
            else if(Signs[i]=='*'){
                Matrixes[i+n]=Mul_Matrix(Pointers,Matrixes[i],Matrixes[i+n]);
                Pointers.Array[i+n]=Matrixes[i+n].Array;
            }
        }
    }

    print(Matrixes[Pointers.Size-1].Array,Matrixes[Pointers.Size-1].Rows,Matrixes[Pointers.Size-1].Columns);        //Print output


    Free_Function(Pointers);    //Free all

    return SUCCES;
}

