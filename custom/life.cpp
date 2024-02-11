#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>

#ifdef WIN32
#include <windows.h>
#elif defined __linux__
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

static constexpr size_t TABLE_MAX_SIZE = 10000;
static constexpr size_t TABLE_DEFAULT_X_SIZE = 7;
static constexpr size_t TABLE_DEFAULT_Y_SIZE = 20;

#define TABLE_DEFAULT_CONFIG "1,1 2,3 2,4 2,5 3,3 3,4 3,7"

struct LifeTable
{
    bool data[2][TABLE_MAX_SIZE][TABLE_MAX_SIZE];

    int curr{0};

    size_t sizeX{0};
    size_t sizeY{0};
};
static LifeTable table;

static void printTable(const LifeTable &table)
{
    for (size_t i = 0; i < table.sizeX; i++)
    {
        for (size_t j = 0; j < table.sizeY; j++)
        {
            printf("%c", table.data[table.curr][j][i] ? 'o' : '.');
        }
        printf("\n");
    }
    printf("\n");
}

static void initTable(LifeTable &table, const std::string &config)
{
    table.sizeX = std::min(table.sizeX, TABLE_MAX_SIZE);
    table.sizeY = std::min(table.sizeY, TABLE_MAX_SIZE);

    // 0,1 0,2 0,3 0,4
    std::stringstream ss;
    ss.str(config);

    while (ss.rdbuf()->in_avail() > 2) // x,y
    {
        int x, y;
        char c;
        ss >> x >> c >> y;
        table.data[table.curr][x][y] = true;
    }
}

// life step
// - Any live cell with fewer than two live neighbors dies, as if by underpopulation.
// + Any live cell with two or three live neighbors lives on to the next generation.
// - Any live cell with more than three live neighbors dies, as if by overpopulation.
// + Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
static void doOneStep(LifeTable &table)
{
    const int next = (table.curr + 1) % 2;
    const auto &data = table.data[table.curr];

    const auto &szX = table.sizeX;
    const auto &szY = table.sizeY;

    for (size_t i = 0; i < table.sizeX; i++)
    {
        for (size_t j = 0; j < table.sizeY; j++)
        {
            int sum = 0;
            // 1 2 3
            // 4 x 5
            // 6 7 8
            if ((i > 0) && (j > 0) && data[j - 1][i - 1]) sum++;
            if (j > 0 && data[i][j - 1]) sum++;
            if ((i < szX - 1) && j > 0 && data[j - 1][i + 1]) sum++;

            if (i > 0 && data[j][i - 1]) sum++;
            if ((i < szX - 1) && data[i + 1][j]) sum++;

            if ((i > 0) && (j < szY - 1) && data[j + 1][i - 1]) sum++;
            if ((j < szY - 1) && data[j + 1][i]) sum++;
            if ((i < szX - 1) && (j < szY - 1) && data[j + 1][i + 1]) sum++;

            const bool alive = data[j][i];
            if (sum < 2 || sum > 3)
            {
                table.data[next][j][i] = false;
            }
            else if (alive || sum == 3)
            {
                table.data[next][j][i] = true;
            }
        }
    }
    table.curr = next;

}

// Example: ./life 6 16 "0,0 0,1 0,2 1,2 2,2 3,2 4,2 10,1 8,2 6,6 5,5 5,4 5,3 4,1 4,2 8,1 8,2 8,3 15,1 15,2 15,3 15,4 14,2 14,3 14,4"
int main(int argc, char **argv)
{
    size_t sizeX = TABLE_DEFAULT_X_SIZE;
    size_t sizeY = TABLE_DEFAULT_Y_SIZE;
    std::string config {TABLE_DEFAULT_CONFIG};

    // prepare args
    if (argc > 1) 
    {
        sizeX = static_cast<size_t>(atoi(argv[1]));
    }
    if (argc > 2) 
    {
        sizeY = static_cast<size_t>(atoi(argv[2]));
    }
    if (argc > 3) 
    {
        config.assign(argv[3]);
    }
    // init table
    table.sizeX = sizeX;
    table.sizeY = sizeY;
    initTable(table, config);
    printTable(table);

    while (1)
    {
        doOneStep(table);
        printTable(table);
        sleep(1);
    }

    return 0;
}
