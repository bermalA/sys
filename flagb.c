#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_FILE_CONTENT_LENGTH 1000
#define MAX_TOTAL_FILE_SIZE 200000000 // 200
#define MAX_INPUT_FILES 32

char *formatFileData(const char *filename, const char *permissions, size_t content_length);

char *formatFileData(const char *filename, const char *permissions, size_t content_length);

void tokenizeAndAppend(const char *content, char **output, size_t *output_size);

void validateInputFiles(int num_files);

void checkFileSizeLimit(char *file_content);

char *processFiles(int num_files, char **file_names);

void createOutputFile(char *all_contents, const char *output_filename);

void CallFlagB(int count, char *argued[]);

int main(int argc, char *argv[])
{
    if (argc < 4 || strcmp(argv[argc - 2], "-o") != 0)
    {
        fprintf(stderr, "Usage: %s <file1> <file2> ... <fileN> -o <outputFile>\n", argv[0]);
        return 1;
    }

    CallFlagB(argc, argv);

    return 0;
}

void tokenizeAndAppend(const char *content, char **output, size_t *output_size)
{
    const char delimiters[] = " \t\n";
    char *copy = strdup(content);
    if (copy == NULL)
    {
        fprintf(stderr, "Error allocating memory for tokenization\n");
        exit(1);
    }

    char *token = strtok(copy, delimiters);
    while (token != NULL)
    {
        size_t new_len = strlen(*output) + strlen(token) + 2;
        if (new_len > *output_size)
        {
            *output_size *= 2;
            *output = realloc(*output, *output_size);
            if (*output == NULL)
            {
                fprintf(stderr, "Error reallocating memory\n");
                free(copy);
                exit(1);
            }
        }

        strcat(*output, token);
        strcat(*output, " ");

        token = strtok(NULL, delimiters);
    }

    free(copy);
}

char *processFiles(int num_files, char **file_names)
{
    size_t output_size = MAX_FILE_CONTENT_LENGTH;
    char *all_contents = malloc(output_size);

    if (all_contents == NULL)
    {
        fprintf(stderr, "Error allocating memory for file contents\n");
        exit(1);
    }

    all_contents[0] = '\0';

    validateInputFiles(num_files);

    for (int i = 0; i < num_files; ++i)
    {
        FILE *file = fopen(file_names[i], "r");

        if (!file)
        {
            fprintf(stderr, "Error opening file: %s\n", file_names[i]);
            continue;
        }

        struct stat file_stat;

        if (stat(file_names[i], &file_stat))
        {
            fprintf(stderr, "Error getting file permissions for: %s\n", file_names[i]);
            fclose(file);
            continue;
        }

        char permissions_str[10];

        snprintf(permissions_str, sizeof(permissions_str), "%o", file_stat.st_mode & 0777);

        while (!feof(file))
        {
            char buffer[MAX_FILE_CONTENT_LENGTH];

            size_t read_size = fread(buffer, 1, sizeof(buffer), file);

            buffer[read_size] = '\0';

            tokenizeAndAppend(buffer, &all_contents, &output_size);
        }

        fclose(file);
    }

    checkFileSizeLimit(all_contents);

    return all_contents;
}

char *formatFileData(const char *filename, const char *permissions, size_t content_length)
{
    size_t needed_space = strlen(filename) + strlen(permissions) + content_length + 50;
    char *formatted_data = malloc(needed_space);

    if (formatted_data == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for formatted data.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(formatted_data, "|%s,%s|\nFile Content:\n", filename, permissions);

    return formatted_data;
}

void createOutputFile(char *all_contents, const char *output_filename)
{
    FILE *output_file = fopen(output_filename, "w");

    if (output_file == NULL)
    {
        fprintf(stderr, "Error opening output file: %s\n", output_filename);
        free(all_contents);
        exit(1);
    }

    fprintf(output_file, "%010lu", strlen(all_contents));

    fprintf(output_file, "%s", all_contents);

    fputc('\0', output_file);

    fclose(output_file);

    if (chmod(output_filename, 0644) == -1)
    {
        fprintf(stderr, "Error setting file permissions for: %s\n", output_filename);
    }

    free(all_contents);
}

void CallFlagB(int count, char *argued[])
{
    int output_option_position = count - 2;

    const char *output_filename = argued[output_option_position + 1];

    char *all_contents = processFiles(output_option_position - 1, &argued[1]);

    createOutputFile(all_contents, output_filename);
}

void validateInputFiles(int num_files)
{
    if (num_files > MAX_INPUT_FILES)
    {
        fprintf(stderr, "Error: Number of input files exceeds the limit (%d)\n", MAX_INPUT_FILES);
        exit(1);
    }
}

void checkFileSizeLimit(char *file_content)
{
    if (strlen(file_content) > MAX_TOTAL_FILE_SIZE)
    {
        fprintf(stderr, "Error: Total size of input files exceeds the limit (%d bytes)\n", MAX_TOTAL_FILE_SIZE);
        free(file_content);
        exit(1);
    }
}
