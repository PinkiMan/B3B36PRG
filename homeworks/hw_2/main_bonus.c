
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    char Digits[100];
    int Len;
    int Rest;
} LARGE;

void Do_Sieve_Of_Eratosthenes(int *Best)
{
    int Max_Prime=1000000;      //Maximum prime
    bool Primes[Max_Prime];     //Array indexes of primes

    for(int i=0; i<Max_Prime; i++)  //Add odd numbers to prime list
    {
        if(i%2==1)
        {
            Primes[i]=true;
        }
        else
        {
            Primes[i]=false;
        }

    }
    Primes[2]=true;     //Set number 2 as prime


    for(int i=3; i*i<=Max_Prime; i++)       //Sieve of Eratosthenes, removes every power of prime
    {
        if(Primes[i]==true)
        {
            for(int j=i*i; j<Max_Prime; j+=i)
            {
                Primes[j]=false;
            }
        }
    }

    int Count=0;
    for (int i = 2; i <= Max_Prime; i++)        //Adds primes to Array
    {
        if (Primes[i]==true)
        {
            Best[Count]=i;
            Count++;
        }
    }
}


void Input(LARGE *Number)
{
    int Index = 0;
    char tmp = '0';
    int Return=1;

    while(tmp != ' ' && tmp != '\n' && Return == 1)
    {
        Return = scanf("%c", &tmp);
        Number->Digits[Index] = tmp-'0';

        if ((tmp <'0' || tmp > '9') && tmp !=' ' && tmp != '\n')
        {
            fprintf(stderr, "Error: Chybny vstup!\n");
            exit(100);
        }

        Index ++;
    }
    Number->Len = Index;


    if (Return != 1 || (Number->Len == 2 && Number->Digits[0] == 0))
    {
        exit(0);
    }
}


bool Greeater_Same(LARGE Number1, int Number2)
{
    if(Number1.Len <= 10)    //Max int number 10 000 000 000
    {
        int Integer = 0;
        for(int i = 0; i < Number1.Len-1; i++)
        {
            Integer *= 10;
            Integer += Number1.Digits[i];
        }

        if (Integer >= Number2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }

}


LARGE Division(LARGE Number, int Divisor)
{
    int Intiger;
    int Counter;

    LARGE Result;
    int Index = 0;
    int ResultIndex = 0;

    int Rest = 0;
    bool FirstRun = true;


    while(Index < Number.Len -1)    //Main loop
    {
        Intiger = Rest;
        Counter = 0;
        while(Intiger < Divisor && Index < Number.Len -1)   // Get Number to int
        {
            Intiger *= 10;
            Intiger += Number.Digits[Index];
            Index ++;
            Counter ++;
        }

        if (Counter > 1 && FirstRun != true)   //Fill with zeros if first
        {
            for(int i = 0; i < Counter-1; i++)
            {
                Result.Digits[ResultIndex] = 0;
                ResultIndex ++;
            }
        }

        Rest = Intiger % Divisor;

        FirstRun = false;
        Result.Digits[ResultIndex] = Intiger / Divisor;
        ResultIndex ++;
    }
    Result.Len = ResultIndex+1;


    Result.Rest=Rest;
    return Result;
}


void Prime_Factorization(LARGE Number, int* Primes)
{
    printf("Prvociselny rozklad cisla ");
    if (Number.Len == 2 && Number.Digits[0] == 1)
    {
        printf("1 je:\n1\n");
    }
    else
    {
        int ActualPrime = Primes[0];
        int Index = 1;
        bool FirstRun = true;
        int Count;


        for(int i = 0; i < Number.Len-1; i++)
        {
            printf("%d", (int) Number.Digits[i]);
        }

        printf(" je:\n");

        while (Greeater_Same(Number, ActualPrime) == true && ActualPrime != 0)
        {
            if (Division(Number, ActualPrime).Rest == 0)
            {
                Count = 0;
                while (Division(Number, ActualPrime).Rest == 0)
                {
                    Number = Division(Number, ActualPrime);
                    Count ++;
                }

                if (FirstRun==false)
                {
                    printf(" x ");

                }else{
                    FirstRun = false;
                }

                if (Count == 1)
                {
                    printf("%d", ActualPrime);
                }
                else
                {
                    printf("%d^%d", ActualPrime, Count);
                }
            }
            ActualPrime = Primes[Index];
            Index ++;
        }

        printf("\n");
    }
}


int main(int argc, char* argv[])
{
    int Primes[100000] = {0};
    Do_Sieve_Of_Eratosthenes(Primes);

    while(true)
    {
        LARGE Number = {.Digits={0}};
        Input(&Number);
        Prime_Factorization(Number, Primes);
    }
    return 0;
}
