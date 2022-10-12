#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define DEBUG_

#ifdef DEBUG_
#define log printf
#else
#define log(...) 
#endif

#define BDY_BLOCK_SIZE(x) (1 << (x))
#define BDY_OFFSET(b, allocator)      ((uintptr_t)b - (uintptr_t)allocator->memory)
#define BDY_BUDDYOF(x, allocator, i)  (uint8_t*)((BDY_OFFSET(x, allocator) ^ (1 << (i))) + (uintptr_t)allocator->memory)

#define BDY_MIN_ORDER 8 // 256 bytes

//////////////////////////////////////////////////////////////////////////////////////

// Simple memory buddy allocator
// Fragmentations are possible
typedef struct buddy_allocator_
{
    size_t min_order;
    size_t max_order;

    uint8_t* free_blocks[32 + 2]; // MAX_ORDER
    uint8_t* memory;

} buddy_allocator_t;

//////////////////////////////////////////////////////////////////////////////////////
static size_t bdy_get_min_order(buddy_allocator_t *buddy, size_t size)
{
    size_t i = 0;
    while (BDY_BLOCK_SIZE(i) < size + 1) // extra byte for the order storage
    {
        i++;
    }

    return (i < buddy->min_order) ? buddy->min_order : i;
}

static size_t bdy_get_max_order(buddy_allocator_t *buddy, size_t size)
{
    size_t i = 0;
    while (BDY_BLOCK_SIZE(i) < size + 1) // extra byte for the order storage
    {
        i++;
    }

    return i - 1;
}
 
//////////////////////////////////////////////////////////////////////////////////////
buddy_allocator_t *buddy_allocator_create(void *raw_memory, size_t raw_memory_size)
{
    // check minimal possible size (1 min order block)
    if (raw_memory_size < (sizeof(buddy_allocator_t) + (1 << BDY_MIN_ORDER)))
    {
        return NULL;
    }

    buddy_allocator_t *buddy = (buddy_allocator_t *)raw_memory;
    memset(buddy, 0, sizeof(*buddy));

    buddy->min_order = BDY_MIN_ORDER;
    buddy->max_order = bdy_get_max_order(buddy, raw_memory_size - sizeof(buddy_allocator_t));
    buddy->memory = (uint8_t*)raw_memory + sizeof(buddy_allocator_t);
    buddy->free_blocks[buddy->max_order] = buddy->memory;

    log("Created buddy allocator(%p) with min_order=%lu, max_order=%lu, memory=%p\n", 
        buddy, buddy->min_order, buddy->max_order, buddy->memory);

    return buddy;
}
 
void buddy_allocator_destroy(buddy_allocator_t *buddy_allocator)
{
    memset(buddy_allocator, 0, sizeof(buddy_allocator_t));
}

void *buddy_allocator_alloc(buddy_allocator_t *buddy_allocator, size_t size)
{
    size_t order = bdy_get_min_order(buddy_allocator, size);

    // search the proper list
    size_t i = order;
    for (;; i++) 
    {
        if (i > buddy_allocator->max_order)
        {
            log("Unable to allocate %lu bytes from buddy %p\n", size, buddy_allocator);
            return NULL;
        }
        if (buddy_allocator->free_blocks[i])
        {
            break;
        }
    }

    // remove the block out of list
    uint8_t *block = buddy_allocator->free_blocks[i];
    buddy_allocator->free_blocks[i] = *(uint8_t**) buddy_allocator->free_blocks[i];

    // split until order is found
    void *buddy;
    while (i-- > order) 
    {
        buddy = BDY_BUDDYOF(block, buddy_allocator, i);
        buddy_allocator->free_blocks[i] = buddy;
    }

    // store order in prev byte
    *((uint8_t*) (block - 1)) = order;
    log("Allocated %lu bytes from buddy %p: %p\n", size, buddy_allocator, block);
    return block;
}
 
void buddy_allocator_free(buddy_allocator_t *buddy_allocator, void *ptr)
{
    // fetch order in previous byte
    int i = *((uint8_t*) (ptr - 1));
    for (;; i++) 
    {
        // calc buddy
        uint8_t *buddy = BDY_BUDDYOF(ptr, buddy_allocator, i);
        uint8_t **p = &(buddy_allocator->free_blocks[i]);

        // find buddy in list
        while ((*p != NULL) && (*p != buddy))
        {
            p = (uint8_t**) *p;
        }

        // not found, insert into the list
        if (*p != buddy)
        {
            *(uint8_t**)ptr = buddy_allocator->free_blocks[i];
            buddy_allocator->free_blocks[i] = ptr;
            return;
        }
        // found, merged block starts from the lower one
        ptr = ((uint8_t*)ptr < buddy) ? ptr : buddy;
        // remove buddy from the list
        *p = *(uint8_t**) *p;
    }
}

//////////////////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)
{
    // Test
    uint8_t memory[sizeof(buddy_allocator_t) + (1 << 16)];

    buddy_allocator_t *buddy = buddy_allocator_create(memory, sizeof(memory));

    // try to alloca a too big block size
    assert(buddy_allocator_alloc(buddy, 1 << 20) == NULL);
    assert(buddy_allocator_alloc(buddy, 1 << 17) == NULL);

    void *block1 = buddy_allocator_alloc(buddy, (1 << 15) - 1);
    assert(block1 != NULL);

    void *block2 = buddy_allocator_alloc(buddy, (1 << 15) - 64);
    assert(block2 != NULL);

    // no any space should be left
    assert(buddy_allocator_alloc(buddy, 1 << 16) == NULL);
    assert(buddy_allocator_alloc(buddy, 1) == NULL);

    // free all allocated blocks
    buddy_allocator_free(buddy, block1);
    buddy_allocator_free(buddy, block2);

    // try to alloc max possible block size
    void *block3 = buddy_allocator_alloc(buddy, (1 << 16) -1);
    assert(block3 != NULL);
    buddy_allocator_free(buddy, block3);

    buddy_allocator_destroy(buddy);
    assert(buddy->min_order == 0);
    assert(buddy->max_order == 0);
    assert(buddy->memory == NULL);

    return EXIT_SUCCESS;
}
