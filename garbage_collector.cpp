/*
**
*/
#include <stdio.h>
#include <stdlib.h>
/*
**
*/
#define STACK_MAX_SIZE 256


typedef enum            /* enum we need to identify the types of the object we are working with */
{
    INT,
    TWIN
}oType;

typedef struct sObject
{
    oType type;         /* struct sObject has this type field which identifies exactly what type of value it is INT OR TWIN */ 
    unsigned char marked;

    union               /* union is needed in order to store data for a specific INT or TWIN */
    {
        int value;

        struct
        {
            struct sObject* head;
            struct sObject* tail;

        };
    };
}Object;

typedef struct     
{
    Object* stack[STACK_MAX_SIZE];
    int stackSize;
}vm;                    /*stack for cirtual machine*/

void push(vm* vm, Object* value)        /* default push operation to add frames to the stack */
{
    vm -> stack[vm -> stackSize++] = value; 
}

Object* pop(vm* vm)                     /* default pop operation to remove frames from the stack */
{
    return vm -> stack[--vm -> stackSize];
}


/* Initializing virtual machine */

/*
Function that returns an initialized stack 
    which is the Kernel of the virtual machine itself.
*/
vm* newVm()
{
    vm* mainVm = (vm*)malloc(sizeof(vm));
    mainVm -> stackSize = 0;
    return mainVm;
}

/*
Function to create objects,
that is, the very place where the memory allocation will take place 
    + the mark of the type bit(INT OR TWIN)
*/
Object* newObject(vm* vm, oType type)
{
    Object* object = (Object*)malloc(sizeof(Object));
    object -> type = type;
    return object;
}

/* now we can custom push any types of objects to this virtual machine */

void pushInt(vm* vm, int intV)
{
    Object* object = newObject(vm, INT);
    object -> value = intV;
    push(vm, object);
}

Object* pushTwin(vm* vm)
{
    Object* object = newObject(vm, TWIN);
    object -> tail = pop(vm);
    object -> head = pop(vm);

    push(vm, object);
    return object;
}

/*its time to mark objects*/
void markAll(vm* vm)
{
    for (int i = 0; i < vm->stackSize; i++)
    {
        mark(vm->stack[i]);
    }
}

/*
in this function, the root of our GC will be concluded
*/
void mark(Object* object)
{
    if (object->marked) return;
    object -> marked = 1;

    if (object->type == TWIN)
    {
        mark(object->head);
        mark(object->tail);
    }
}