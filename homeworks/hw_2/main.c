#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ERROR_INPUT 100
#define VALID_END 0

#define NUM_OF_PRIMES 100000


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

void Prime_Factors(long long int Number, int *Primes)     //Prints prime factors
{
    int First_Print=1;

    printf("Prvociselny rozklad cisla %lld je:\n", Number);
    if(Number==1)    //Prints 1 if Number for factorisation is 1
    {
        printf("1");
    }
    else        //Prints prime factors of Number
    {
        for(int i=0; Primes[i]<=Number; i++)
        {
            int Count=0;
            while(Number%Primes[i]==0)
            {
                Number=Number/Primes[i];
                Count++;
            }

            if(Count>0)
            {
                if(First_Print==0)
                {
                    printf(" x ");
                }

                if(Count>1)
                {
                    printf("%d^%d",Primes[i],Count);
                    First_Print=0;
                }
                else if(Count==1)
                {
                    printf("%d",Primes[i]);
                    First_Print=0;
                }
            }
        }
    }
    printf("\n");
}



int main()
{
    int Primes[NUM_OF_PRIMES];

    Do_Sieve_Of_Eratosthenes(Primes);   //Init primes with Sieve

    long long int Input_Number=-1;      //Set to -1 to be sure, not random number from memory

    do
    {
        Input_Number=-1;    //Reset number to neutral

        int Input_Validation=scanf("%lld",&Input_Number);   //get input

        if(Input_Number<0||Input_Validation!=1)     //Handle error with no input or invalid input
        {
            fprintf(stderr,"Error: Chybny vstup!\n");
            return ERROR_INPUT;

        }
        else if(Input_Number>=1)        //Do prime factorisation
        {
            Prime_Factors(Input_Number,Primes);
        }

    }
    while(Input_Number!=0);

    return VALID_END;
}

