#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <thread>
#include <unistd.h>
#include <queue>
#include <atomic>
#include <mutex>
#include <string.h>

using namespace std;

static int debugLevel = 1;


#define DEBUG_PRINT(fmt, ...) if(debugLevel > 0) { printf(fmt, __VA_ARGS__); }

class Buffer
{
public:
    Buffer (const char *fileName, size_t maxSize) : m_MaxSize(maxSize), m_FreeSize(maxSize) {
        if (maxSize > 0) {
            m_Data = new uint8_t[maxSize];
        }

        m_Fd = fopen(fileName, "w");
    }

    ~Buffer () {
        if (m_Data) {
            Flush();

            delete []m_Data, m_Data = nullptr;
            m_CurSize = 0;

            fclose(m_Fd);
            m_Fd = nullptr;
        }
    }

    bool IsInit() const  {return m_Fd && m_MaxSize;}
    bool HasData() const {return m_CurSize > 0;}

    void Write (const uint8_t* data, size_t size) {
        if (!IsInit() || size == 0) 
            return;

        // not enough space even in fully free buffer - write all to file right away
        if (size > m_MaxSize) {
            Flush();
            DEBUG_PRINT("flushed %d bytes\n", (int)m_CurSize);
            fwrite(data, size, 1, m_Fd);
        }
        // enough free space in buffer for new data
        else if (size <= m_FreeSize) {
            memcpy(m_Data + m_CurSize, data, size);
            
            m_CurSize += size;
            m_FreeSize -= size;
        }
        // not enough space in current buffer
        else {
            // finish buffer
            size_t written = m_FreeSize;
            memcpy(m_Data + m_CurSize, data, m_FreeSize);
            m_CurSize = m_MaxSize;
            Flush();

            Write(data + written, size - written);
        }

        if (m_FreeSize == 0) {
            Flush();
        }
    }

    bool Flush () {
        bool result = false;

        if (IsInit() && m_CurSize > 0) {
            DEBUG_PRINT("flushed %d bytes\n", (int)m_CurSize);
            result = fwrite(m_Data, m_CurSize, 1, m_Fd) == (int)m_CurSize;
            
            m_CurSize = 0;
            m_FreeSize = m_MaxSize;
            m_Data[0] = 0;

        }
        return result;
    }

private:
    FILE* m_Fd {nullptr};
    const size_t m_MaxSize  {0};
    size_t m_CurSize  {0};
    size_t m_FreeSize {0};

    uint8_t* m_Data {nullptr};

    // add stat counters
};



#define WRITE_TEXT_TO_BUF(buf, text) buf.Write((uint8_t*)text, sizeof(text) - 1);

int main(int argc, char **argv) {

    Buffer buf("test.txt", 4);

    printf("Testing buf\n");
    WRITE_TEXT_TO_BUF(buf, "1");
    WRITE_TEXT_TO_BUF(buf, "1");
    WRITE_TEXT_TO_BUF(buf, "12");
    WRITE_TEXT_TO_BUF(buf, "123");
    WRITE_TEXT_TO_BUF(buf, "1");
    WRITE_TEXT_TO_BUF(buf, "1234");
    WRITE_TEXT_TO_BUF(buf, "1234123123");
    printf("End of buf test");
    buf.Flush();

    Buffer buf2("test.txt", 10);

    printf("\nTesting buf2\n");
    WRITE_TEXT_TO_BUF(buf2, "1234");
    WRITE_TEXT_TO_BUF(buf2, "12");
    WRITE_TEXT_TO_BUF(buf2, "12");
    WRITE_TEXT_TO_BUF(buf2, "12356");
    WRITE_TEXT_TO_BUF(buf2, "12356");
    printf("End of buf test: flushing - ");
    buf2.Flush();

    return 0;
}