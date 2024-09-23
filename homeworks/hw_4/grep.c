#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct{
    char *Pattern;
    bool Color;
    int Pattern_Len;
    FILE *File;
}ARGUMETS;


ARGUMETS Arguments(int argc, char *argv[]){
    ARGUMETS Argument_Obj;
    Argument_Obj.Pattern_Len=0;

    if(argc==3){
        Argument_Obj.Pattern=argv[1];
        while(Argument_Obj.Pattern[Argument_Obj.Pattern_Len]!='\0'){
            Argument_Obj.Pattern_Len++;
        }
        Argument_Obj.File = fopen(argv[2], "r");
        Argument_Obj.Color=false;
    }
    else if(argc==4){
        Argument_Obj.Pattern=argv[2];
        while(Argument_Obj.Pattern[Argument_Obj.Pattern_Len]!='\0'){
            Argument_Obj.Pattern_Len++;
        }
        Argument_Obj.File = fopen(argv[3], "r");
        Argument_Obj.Color=true;
    }
    return Argument_Obj;
}


void Return_Function(bool Printed){
    if(Printed==true){
        exit(0);
    }else{
        exit(1);
    }
}


bool Colorless(ARGUMETS Argument_Obj){
    char Znak='\0';
    char Actual_Line[2048];
    int Line_Index=0;
    bool color[2048];
    for(int i=0;i<2048;i++){
        color[i]=false;
    }
    bool Printing=false;
    bool Last=false;
    bool Printed=false;
    int Same_Char=0;
    int i=0;

    while(Znak!=EOF){
        Znak=fgetc(Argument_Obj.File);
        Actual_Line[Line_Index]=Znak;
        Line_Index++;
        color[Line_Index]=false;



        if(Printing==false){
            if(Znak==Argument_Obj.Pattern[Same_Char]){
                Same_Char++;
                Last=true;
            }
            else{
                Same_Char=0;
                Last=false;
            }

            if(Same_Char==Argument_Obj.Pattern_Len){
                Last=false;
                Printing=true;
                for(i=0;i<Line_Index-1;i++){
                    printf("%c",Actual_Line[i]);
                }
                for(i=Line_Index-Argument_Obj.Pattern_Len-1;i<Line_Index-1;i++){
                    color[i+1]=true;
                }
                Printed=true;
                Same_Char=0;
            }

        }
        if(Printing==true){
            printf("%c",Znak);
        }


        if(Znak=='\n'){
            Same_Char=0;
            Printing=false;
            Line_Index=0;
            for(i=0;i<2048;i++){
                Actual_Line[i]='0';
            }
        }
    }
    return Printed;
}


bool Colorfull(ARGUMETS Argument_Obj){
    char Znak='\0';
    char Actual_Line[2048];
    int Line_Index=0;
    bool color[2048];
    for(int i=0;i<2048;i++){
        color[i]=false;
    }
    bool Printing=false;
    bool Printed=false;
    int Same_Char=0;
    int i=0;


    while(Znak!=EOF){
        Znak=fgetc(Argument_Obj.File);
        Actual_Line[Line_Index]=Znak;
        color[Line_Index]=false;
        Line_Index++;


        if(Znak==Argument_Obj.Pattern[Same_Char]){
            Same_Char++;
        }
        else{
            Same_Char=0;
        }

        if(Same_Char==Argument_Obj.Pattern_Len){
            for(i=Line_Index-Argument_Obj.Pattern_Len-1;i<Line_Index-1;i++){
                color[i+1]=true;
            }
            Printing=true;
        }



        if((Znak=='\n')||(Znak==EOF)){
            if(Printing==true){
                for(i=0;i<Line_Index;i++){
                    if(i>0){
                        if((color[i-1]==false)&&(color[i]==true)){
                            printf("%c[01;31m%c[K", (char)27, (char)27);
                        }

                    }
                    else if(color[0]==true){
                        printf("%c[01;31m%c[K", (char)27, (char)27);
                    }

                    if((color[i-1]==true)&&(color[i]==false)){
                            printf("%c[m%c[K", (char)27, (char)27);
                    }

                    printf("%c",Actual_Line[i]);
                    Printed=true;
                }
            }

            Line_Index=0;
            Printing=false;
        }
    }
    return Printed;
}


int main (int argc, char *argv[]){
    bool Printed=false;


    ARGUMETS Argument_Obj=Arguments(argc,argv);


    if(Argument_Obj.Color==false){
        Printed=Colorless(Argument_Obj);
    }
    else{
        Printed=Colorfull(Argument_Obj);
    }
    fclose(Argument_Obj.File);

    Return_Function(Printed);
}
