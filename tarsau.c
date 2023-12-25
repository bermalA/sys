#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{

    if (argc < 2 || argv[1][0] != '-')
    {
        fprintf(stderr, "Please enter arguements...");
        return 1;
    }

    switch (argv[1][1])
    {
    case 'a':
        printf("In use: Flag A\n");
        break;

    case 'b':
        printf("In use of flag b\n");

        // Allocate memory for the argument string dynamically
        char *argumentString = malloc(strlen("./flagb") + 1);

        if (argumentString == NULL)
        {
            fprintf(stderr, "Memory allocation failed.\n");
            return 1;
        }

        strcpy(argumentString, "./flagb");

        // Concatenate additional arguments dynamically
        for (int i = 2; i < argc; ++i)
        {
            argumentString = realloc(argumentString, strlen(argumentString) + strlen(argv[i]) + 2);

            if (argumentString == NULL)
            {
                fprintf(stderr, "Memory reallocation failed.\n");
                return 1;
            }

            strcat(argumentString, " ");
            strcat(argumentString, argv[i]);
        }

        // Call the program with the constructed argument string
        system(argumentString);

        // Free the dynamically allocated memory
        free(argumentString);
        break;

    default:
        break;
    }

    return 0;
}
