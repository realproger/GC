/*
**
*/
#include <stdio.h>
#include <stdlib.h>
/*
**
*/
#define STACK_MAX_SIZE 256
#define IGCT 8


typedef enum            /* enum we need to identify the types of the object we are working with */
{
    INT,
    TWIN
}oType;

typedef struct sObject
{
    oType type;         /* struct sObject has this type field which identifies exactly what type of value it is INT OR TWIN */ 
    unsigned char marked;

    struct sObject* next;

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

    Object* firstObject;

    int numObjects;
    int maxObjects;
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
    mainVm -> firstObject = NULL;
    mainVm -> numObjects = 0;
    mainVm -> maxObjects = IGCT;

    return mainVm;
}

/*
Function to create objects,
that is, the very place where the memory allocation will take place 
    + the mark of the type bit(INT OR TWIN)
*/
Object* newObject(vm* vm, oType type)
{
    if (vm->numObjects == vm->maxObjects) gc(vm);
    Object* object = (Object*)malloc(sizeof(Object));
    object -> type = type;

    vm -> maxObjects++;
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

/*
It remains to go through these objects and delete those objects that are not marked
Linked lists needed
*/

void marksweep(vm* vm)          /* searching and removing of unmarked objects */
{
    Object** object = &vm -> firstObject;
    while(*object)
    {
        if (!(*object) -> marked)
        {
            Object* unreached = *object;
            *object = unreached -> next;
            free(unreached);

            vm -> numObjects--;
        }
        else
        {
            (*object) -> marked = 0;
            object = &(*object) -> next;
        }
    }
}

void gc(vm* vm)
{
    int numObjects = vm -> numObjects;

    markAll(vm);
    marksweep(vm);

    vm -> maxObjects = vm -> numObjects * 2;
    printf("Удалено(deleted) %d объектов, %d осталось(remained).\n", numObjects - vm->numObjects, vm->numObjects);
}

void printObj(Object* object)
{
    switch(object->type)
    {
        case INT:
        printf("%d", object->value);
        break;

        case TWIN:
        printf("(");
        printObj(object->head);
        printf(", ");
        printObj(object->tail);
        printf(")");
        break;

    }
}