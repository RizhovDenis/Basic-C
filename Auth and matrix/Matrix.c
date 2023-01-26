#include <stdio.h>  // for printf
#include <stdlib.h> // for random date
#include <time.h>  

void green() // function make color 
{
    printf("\033[0;32m");
}

void bgreen()
{
    printf("\e[1;32m");
}

void hgreen()
{
    printf("\e[0;92m");
}

int main(void)
{

    for (int i = 0; i<100000000; i++)
    {
        if (i % 3 == 0)
        {
            bgreen();
            printf("%d ", rand() % 10000); // rand make random date for 0 - 9999
        }
        else if (i % 2 == 0)
        {
            hgreen();
            printf("%d ", rand() % 10000);
        }
        else
        {
           green();
           printf("%d ", rand() % 10000); 
        }
    }
    return 0;
    
}


