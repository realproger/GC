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

    union               /* union is needed in order to store data for a specific INT or TWIN */
    {
        int value;

        struct
        {
            struct sObject* head;
            struct sObject* title;

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