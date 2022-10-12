#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>

int fnmatch(const char *pattern, const char *string, int flags);

void help()
{
    printf("fnmatch <pattern> <file>\n");
    printf("example: fnmatch --pattern 'test*' input.txt\n");
}

int main (int argc, char **argv)
{
    if (argc < 4 || strncmp(argv[1], "--pattern")) // TODO: use gopt for arg parsing
    {
        help();
        exit(EXIT_FAILURE);
    }

    const char *pattern = argv[2]; // skip --pattern 
    const char *input = argv[3];

    FILE *f = fopen(input, "r");
    if (f == NULL) 
    {
        exit(EXIT_FAILURE);
    }

    char * line = NULL;
    size_t len = 0;
    while (getline(&line, &len, f) != -1) 
    {
        if (fnmatch(pattern, line, 0) == 0) 
        {
            printf("%s", line);
        }
    }

    fclose(f);
    return EXIT_SUCCESS;
}