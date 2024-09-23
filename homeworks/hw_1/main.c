#include <stdio.h>
#include <stdlib.h>

#define INVALID_INPUT 100
#define OUT_OF_RANGE 101
#define EVEN_NUMBER 102
#define INVALID_FENCE 103


void Roof(int Width){   //Vypise strechu domu
    for(int i=Width/2-1;i>-1;i--){
        for(int j=0;j<Width;j++){
            if(j==i+1){
                printf("X");
                if(i==Width/2-1){
                    j=Width;
                }
            }else if(j==Width-i-2){
                printf("X");
                j=Width;
            }else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

void Line(int Width){   //Vypise patro/podlahu domu
    for(int i=0;i<Width;i++){
        printf("X");
    }
    printf("\n");
}

void Body(int Width, int Height){   //Vypise telo domu
    //Vypis tela doma
    for(int i=0;i<Height-2;i++){
        printf("X");
        for(int j=0;j<Width-2;j++){
            printf(" ");
        }
        printf("X\n");
    }
}

void Body_Fence(int Width, int Height, int Fence){     //Vypsie telo domu s plotem
    for(int i=0;i<Height-2;i++){
        //Vypis tela domu
        printf("X");
        for(int j=0;j<Width-2;j++){
            if((j+i)%2==0){
                printf("o");
            }
            else{
                printf("*");
            }
        }
        printf("X");

        //Vypis plotu
        if(Height-Fence-1==i){
            if(Fence%2==1){
                printf("|");
            }
            for(int i=0; i<Fence/2; i++){
                printf("-|");
            }

        }else if(Height-Fence-1<i){
            if(Fence%2==1){
                printf("|");
            }
            for(int i=0; i<Fence/2; i++){
                printf(" |");
            }
        }
        printf("\n");
    }
}

void Line_Fence(int Width, int Fence){      //Vypise patro/podlahu domu s plotem
    for(int i=0;i<Width;i++){
        printf("X");
    }
    if(Fence%2==1){
        printf("|");
    }
    for(int i=0; i<Fence/2; i++){
        printf("-|");
    }
    printf("\n");
}

int main()
{
    int Height=0;
    int Width=0;

    int Err=scanf("%d %d",&Width,&Height);

    if(Err!=2){
        fprintf(stderr, "Error: Chybny vstup!\n");
        return INVALID_INPUT;
    }

    if((Width<3)||(Width>69)){
        fprintf(stderr, "Error: Vstup mimo interval!\n");
        return OUT_OF_RANGE;
    }

    if((Height<3)||(Height>69)){
        fprintf(stderr, "Error: Vstup mimo interval!\n");
        return OUT_OF_RANGE;
    }

    if(Width%2==0){
        fprintf(stderr, "Error: Sirka neni liche cislo!\n");
        return EVEN_NUMBER;
    }

    if(Width==Height){
        int Fence=0;
        int Err_Fence=scanf("%d",&Fence);

        if(Err_Fence!=1){
            fprintf(stderr, "Error: Chybny vstup!\n");
            return INVALID_INPUT;
        }

        if((Fence>=Height)||(Fence<=0)){
            fprintf(stderr, "Error: Neplatna velikost plotu!\n");
            return INVALID_FENCE;
        }

        Roof(Width);
        Line(Width);
        Body_Fence(Width,Height,Fence);
        Line_Fence(Width, Fence);

    }else{
        Roof(Width);
        Line(Width);
        Body(Width,Height);
        Line(Width);
    }

    return 0;
}

