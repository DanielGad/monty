#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monty.h"

#define MAX_LINE_LENGTH 1024

bus_t bus = {NULL, NULL, NULL, 0};

/**
 * read_line - Custom line reading function
 * @file: File pointer
 * @line: Buffer to store the read line
 * @size: Size of the buffer
 * Return: Number of characters read, -1 on failure or end of file
 */
ssize_t read_line(FILE *file, char **line, size_t *size)
{
    if (!line || !size || !file)
        return -1;

    ssize_t read = 0;
    size_t len = 0;
    char *buffer = NULL;

    if (getline(&buffer, &len, file) != -1)
    {
        read = strlen(buffer);
        if (read > 0 && buffer[read - 1] == '\n')
            buffer[read - 1] = '\0';

        *line = malloc((read + 1) * sizeof(char));
        if (*line)
        {
            strcpy(*line, buffer);
            *size = read;
        }
        else
        {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    free(buffer);
    return read;
}

/**
 * main - Monty code interpreter
 * @argc: Number of arguments
 * @argv: Array of arguments
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
    char *content;
    FILE *file;
    size_t size = 0;
    stack_t *stack = NULL;
    unsigned int counter = 0;

    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        exit(EXIT_FAILURE);
    }

    file = fopen(argv[1], "r");
    bus.file = file;

    if (!file)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    while (read_line(file, &content, &size) != -1)
    {
        bus.content = content;
        counter++;
        execute(content, &stack, counter, file);
        free(content);
    }

    free_stack(stack);
    fclose(file);

    return (0);
}
