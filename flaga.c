#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_TOKEN_LENGTH 50
#define MAX_FOLDER_PATH_LENGTH 100

void createFolder(const char *folder_path)
{
#ifdef _WIN32
    _mkdir(folder_path);
#else
    mkdir(folder_path, 0777);
#endif
}

void writeTokenToFile(const char *token, int index, const char *folder_path)
{
    char file_path[MAX_FOLDER_PATH_LENGTH + MAX_TOKEN_LENGTH + 10];
    snprintf(file_path, sizeof(file_path), "%s/f%d.txt", folder_path, index);

    FILE *file = fopen(file_path, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening file for writing: %s\n", file_path);
        exit(1);
    }

    fprintf(file, "%s", token);

    fclose(file);
}

void processOutputFile(const char *output_filename, const char *folder_path)
{
    FILE *output_file = fopen(output_filename, "r");
    if (output_file == NULL)
    {
        fprintf(stderr, "Error opening output file: %s\n", output_filename);
        exit(1);
    }

    char size_str[11];
    fread(size_str, 1, 10, output_file);
    size_str[10] = '\0';
    size_t content_size = atoi(size_str);

    char *content = malloc(content_size + 1);
    if (content == NULL)
    {
        fprintf(stderr, "Error allocating memory for content\n");
        fclose(output_file);
        exit(1);
    }

    fread(content, 1, content_size, output_file);
    content[content_size] = '\0';

    fclose(output_file);

    const char delimiters[] = "\n";
    char *token = strtok(content, delimiters);
    int index = 1;
    while (token != NULL)
    {
        writeTokenToFile(token, index, folder_path);
        token = strtok(NULL, delimiters);
        index++;
    }

    free(content);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <outputFile> <folderName>\n", argv[0]);
        return 1;
    }

    const char *output_filename = argv[1];
    const char *folder_path = argv[2];

    createFolder(folder_path);
    processOutputFile(output_filename, folder_path);

    return 0;
}
