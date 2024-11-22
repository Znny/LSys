//
// Created by Ryanc on 11/1/2024.
//
#ifndef STACK_H
#define STACK_H

#include <stdio.h>

#define STACK_ALLOCATION_FAILURE 0

template<typename ElementType>
class Stack
{
public:
    Stack()
    {
        Elements = (ElementType*) malloc(ExpansionSize * NumAllocations * sizeof(ElementType));

        if (Elements == nullptr)
        {
            //this should realistically never happen, so exiting the application is appropriate
            fprintf(stderr, "%s:%d: DynamicSet::DynamicSet: error constructing dyset, initial allocation failed. exiting.\n", __FILE__, __LINE__);
            exit(STACK_ALLOCATION_FAILURE);
        }
        MaxElements = ExpansionSize;
    }

    ~Stack()
    {
        if(Elements != nullptr)
        {
            free(Elements);
            Elements = nullptr;
        }
    }

    void Push(const ElementType& NewElement)
    {
        if(NumElements == NumAllocations * ExpansionSize)
        {
            Expand();
        }

        Elements[NumElements++] = NewElement;
    };

    ElementType Pop()
    {
        return Elements[--NumElements];
    };

private:

    void Expand()
    {
        NumAllocations++;
        ElementType* reallocdat = (ElementType*) realloc(Elements, ExpansionSize * NumAllocations * sizeof(ElementType));
        if (reallocdat != nullptr)
        {
            Elements = reallocdat;
            MaxElements = NumAllocations * ExpansionSize;
        }
        else
        {
            //this should realistically never happen, so exiting the application is appropriate
            fprintf(stderr, "%s:%d: DynamicSet::Expand: error re-allocating element array. exiting\n", __FILE__, __LINE__);
            exit(STACK_ALLOCATION_FAILURE);
        }
    }

    ElementType* Elements = nullptr;
    int MaxElements = 0;
    int NumElements = 0;
    int NumAllocations = 1;
    const int ExpansionSize = 1024;
};



#endif //STACK_H
