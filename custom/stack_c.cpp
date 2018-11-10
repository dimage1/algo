#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct stack_{
   void          *data;
   size_t         size;
   struct stack_ *prev;
} stack_t;

enum {
   STK_OK = 0,
   STK_WRONG_ARG,
   STK_NOT_ENOUGH_DATA,
   STK_MALLOC_FAILED
};

int    stack_push(      stack_t **stk, const void *data, size_t size);
int    stack_pop (      stack_t **stk,       void *data, size_t size);
int    stack_top (const stack_t  *stk,       void *data, size_t size);
size_t stack_size(const stack_t  *stk);

#define _DBG(lvl, ...) {if (lvl <= debug_level)printf(__VA_ARGS__);}

static size_t debug_level = 3;

int stack_push(stack_t **stk, const void *data, size_t size)
{
    if (!stk || !data || !size)
        return STK_WRONG_ARG;
        
    stack_t *head = (stack_t*)malloc(sizeof(stack_t));
    if (!head)
    {   
        _DBG(1, "!head alloc Failed\n");
        return STK_MALLOC_FAILED;
    }
    _DBG(2, "+head alloc OK\n");
    
    head->data = malloc(size);
    if (!head->data)
    {
        free(head);
        _DBG(1, "!data of size %lu alloc Failed\n", size);
        return STK_MALLOC_FAILED;
    }
    _DBG(1, "+data of size %lu alloc OK\n", size);
    memcpy(head->data, data, size);
    head->prev = *stk;
    head->size = size;
    *stk = head;

    return STK_OK;
}

int stack_top (const stack_t *stk, void *data, size_t size)
{
   if (!stk || !data)
       return STK_WRONG_ARG;
      
   if (stk->size > size)
   {
       _DBG(1, "!top of size - %lu > %lu\n", stk->size, size);
       return STK_NOT_ENOUGH_DATA;
   }
      
   memcpy(data, stk->data, stk->size);
   _DBG(2, "-top of size %lu OK\n", stk->size);
   return STK_OK;
}

int stack_pop (stack_t **stk, void *data, size_t size)
{
   if (!stk)
       return STK_WRONG_ARG;  
  
   int result = stack_top(*stk, data, size);
   if (STK_OK == result) {
       stack_t *tmp = (*stk)->prev;
       free((*stk)->data);
       free((*stk));
       *stk = tmp;
   }
   return result;
}

size_t stack_size (const stack_t *stk) 
{  
    size_t sz = 0;

    const stack_t *tmp = stk;
    while (tmp) {
        sz++;
        tmp = tmp->prev;
        _DBG(3, "~sz++, tmp: %p\n", tmp);
    }
    return sz;
}

/*struct {
   int x;
   int y;
} Point;*/

int main(int argc, char **argv) {
    if (argc > 1)
        debug_level = atoi(argv[1]);

    // int test
    stack_t *stk1 = NULL;

    int i = 0;
    stack_push(&stk1, &(++i), sizeof(i));
    stack_push(&stk1, &(++i), sizeof(i));
    stack_push(&stk1, &(++i), sizeof(i));
    printf(">size: %lu\n", stack_size(stk1)); // 3
    printf("~~~~~~~~~~~~~~~~\n");

    int x = 123;
    stack_pop(&stk1, &x, sizeof(x));
    printf(">pop: %d, %lu more\n", x, stack_size(stk1)); // 3, 2 more
    stack_top(stk1, &x, sizeof(x));
    printf(">top: %d, %lu more\n", x, stack_size(stk1)); // 2, 2 more
    //-------------------------------------------------------------
    // String test
    printf("--------------------------------------------\n");

    stack_t *stk2 = NULL;
    stack_push(&stk2, "Hello", sizeof("Hello"));
    stack_push(&stk2, "world", sizeof("world"));

    char s[256] = {0};
    stack_pop(&stk2, s, sizeof(s));
    printf(">pop: %s, %lu more\n", s, stack_size(stk2)); // Hello, 1 more
    stack_pop(&stk2, s, sizeof(s));
    printf(">pop: %s, %lu more\n", s, stack_size(stk2)); // world, 0 more
    //-------------------------------------------------------------
    // TODO: add Struct test with Point here
    printf("--------------------------------------------\n");

    stack_t *stk3 = NULL;
}
