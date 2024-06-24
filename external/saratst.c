#include <stdio.h>
#include <string.h>

typedef struct
{
    char name[20];
    int ID;
    char password[20];
} Patient;

void basiclogin(char expectusername[], char expectpassword[])
{
    char inputUser[30];
    char inputPassw[30];

    printf("Input username > ");
    scanf("%s", inputUser);
    printf("\nInput password > ");
    scanf("%s", inputPassw);
    printf("\n");

    if (strcmp(inputUser, expectusername))
    {
        printf("Error: Invalid username\n");
        return;
    }

    if (strcmp(inputPassw, expectpassword))
    {
        printf("Error: Invalid password\n");
        return;
    }

    printf("Login successful!\n");
}

void actualLogin(Patient patients[], int npatients)
{
    char inputUser[30];
    char inputPassw[30];

    printf("Input username > ");
    scanf("%s", inputUser);
    printf("\nInput password > ");
    scanf("%s", inputPassw);
    printf("\n");

    int counter = 0;

    int userFound = 0; // false
    int correctPassword = 0;

    while (counter < npatients)
    {
        // if inputUser == patients[counter].password
        if (!strcmp(inputUser, patients[counter].name))
        {
            userFound = 1;

            // if passwords are equal
            if (!strcmp(inputPassw, patients[counter].password))
            {
                correctPassword = 1;
            }
            break;
        }

        counter++;
    }

    if (!userFound)
    {
        printf("Error: User not found...\n");
        return;
    }

    if (!correctPassword)
    {
        printf("Error: Invalid password...\n");
        return;
    }

    printf("Login successful!\n");
}

int main(int argc, char **argv)
{

    Patient patients[] =
        {
            {.name = "patient1", .ID = 1, .password = "pass1"},
            {.name = "patient2", .ID = 2, .password = "pass2"},
            {.name = "patient3", .ID = 3, .password = "pass3"},
        };

    actualLogin(patients, 3);

    return 0;
}