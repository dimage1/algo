#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/mman.h> // for mmap, munmap
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

static void help()
{
    printf("bsearch --key <key> <csv_sorted_file>\n");
    printf("example: bsearch --key test input.csv\n");
}

// Sets obtained file size to outSize
// Returns file mapped memory
static char* mmapFile(const char *fileName, size_t *outSize)
{
    *outSize = 0;

    int fd = open (fileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror ("open");
        return NULL;
    }

    struct stat st;
    if (fstat (fd, &st) == -1)
    {
        perror ("fstat");
        return NULL;
    }

    char *buf = mmap (0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (buf == MAP_FAILED)
    {
        close(fd);
        perror ("mmap");
        return NULL;
    }

    if (close (fd) == -1)
    {
        perror ("close");
        return NULL;
    }

    *outSize = st.st_size;
    return buf;
}

static const char* getPrevLine(const char* data, const char* start)
{
    if (data == start)
        return data;

    while (data != start && *data != '\n')
    {
        data--;
    }
    return *data == '\n' ? data + 1 : NULL;
}

static const char* getNextLine(const char* data, size_t size, size_t *nextSize)
{
    while (*data != '\n')
    {
        data++;
        size++;
    }

    *nextSize = size > 0 ? size - 1 : 0;
    return data + 1; // skip \n
}

static const char* printLine(const char *line)
{
    while(*line != '\n')
    {
        putchar(*line);
        line++;
    }
    putchar('\n');
    return line + 1;
}

static void printMatchedLines(const int key, const char* start, const char* data, size_t size)
{
    // do a binary search

    size_t nextSize = 0;
    const char* const nextLine = size <= 2 ? data : getNextLine(data + size/2, size/2, &nextSize);

    int lineKey = atoi(nextLine);
    if (lineKey == key)
    {
        // find the 1st matching line
        // do a backward iteration starting from the found position
        const char* startLine = nextLine;
        if (nextLine != start)
        {
            while (1)
            {
                const char *prevLine = getPrevLine(startLine - 2, start); // skip \n
                if (!prevLine)
                    break;

                int prevLineKey = atoi(prevLine);
                if (prevLineKey != key)
                    break;

                startLine = prevLine;
            }
        }

        // print all the matching lines
        const char* printingLine = startLine;
        while (lineKey == key)
        {
            printingLine = printLine(printingLine);
            lineKey = atoi(printingLine);
        }
    }
    else if (lineKey > key)
    {
        printMatchedLines(key, start, data, size/2);
    }
    else
    {
        printMatchedLines(key, start, data + size/2, size/2);
    }

}

int main (int argc, char **argv)
{
    if (argc < 4 || strcmp(argv[1], "--key"))
    {
         help();
         exit(EXIT_FAILURE);
    }

    // !! assume, key is an integer
    int key = atoi(argv[2]);
    const char *fileName = argv[3];

    size_t fileSize = 0;
    char *buf = mmapFile(fileName, &fileSize);
    if (buf == NULL)
    {
        exit(EXIT_FAILURE);
    }

    printMatchedLines(key, buf, buf, fileSize);

    munmap(buf, fileSize);
    return EXIT_SUCCESS;
}