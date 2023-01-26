#include <stdio.h> // for printf and scanf
#include <unistd.h> // for sleep
#include <string.h> // for strcmp

int main (void)
{    
    char login_system[]= "test0";
    char password_system[]= "test0";
    char login[24], password[24];

    for (int n = 0; n < 5; n++)
    {
        printf("Enter login:");
        scanf("%s", login);
        printf("Enter password:");
        scanf("%s", password);
   
        if ((strcmp (login_system, login)==0) && (strcmp(password_system, password)==0))
        {
            printf("Welcome ADMIN!\n");
            return 0;
        }
        else if (n == 2 || n == 3)
        {
            printf("User with such credentials was not found \nWaiting 3 secs... \n");
            sleep(3);
        }
        printf("User with such credentials was not found \n");
    }
    printf("Exit 0 \n");

    return 0;
}