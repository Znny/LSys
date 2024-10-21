#pragma once

#include <stdio.h>
#include <stdlib.h>

/***
* @file dyset.h
*     a simple templated dynamic set of elements which allows adding and removing elements, to be used when
*     the order of elements does not matter, opting for adding and remove speed over order consistency
*/

#define DYSET_ALLOCATION_FAILURE 0xDEADBEEF
#define DYSET_INVALID_INDEX 0xBAADF00D

template <typename ElementType>
class DynamicSet
{
public:

    /***
    * @brief Default constructor, initialize variables and allocate initial storage for elements
    */
    DynamicSet() : MaxElements(0), NumElements(0), NumAllocations(1), Data(nullptr)
    {
        Data = (ElementType*) malloc(ExpansionSize * NumAllocations * sizeof(ElementType));

        if (Data == nullptr)
        {
            //this should realistically never happen, so exiting the application is appropriate
            fprintf(stderr, "%s:%d: DynamicSet::DynamicSet: error constructing dyset, initial allocation failed. exiting.\n", __FILE__, __LINE__);
            exit(DYSET_ALLOCATION_FAILURE);
        }
        MaxElements = ExpansionSize;
    };

    /***
    * @brief Destruct object, free used data if necessary
    */
    ~DynamicSet()
    {
        if (Data != nullptr)
        {
            free(Data);
            Data = nullptr;
        }
    }

    /***
    * @brief Remove the element at the specified index
    * @param i The index of the element to remove.
    * @return The element at the given index (if valid).
    *
    * @warning This function exits the program if the index is out of bounds, preferring crashing early and
    *          loudly over breaking silently with potential side-effects
    */
    ElementType RemoveIndex(int i)
    {
        if (i >= 0 && i < NumElements)
        {
            ElementType Returned = Data[i];
            Data[i] = Data[NumElements - 1];
            NumElements -= 1;
            return Returned;
        }
        else
        {
            fprintf(stderr, "%s:%d: DynamicSet::RemoveIndex: error removing index %d, invalid index. exiting\n", __FILE__, __LINE__, i);
            exit(DYSET_INVALID_INDEX);
        }
    }

    /***
    * @brief Remove the last index in the set
    * @return The last element in the set (if valid)
    *
    * @warning This function exits the program if the index is out of bounds, preferring crashing early and
    *          loudly over breaking silently with potential side-effects
    */
    ElementType RemoveLast()
    {
        return RemoveIndex(NumElements - 1);
    }

    /***
    * @brief Add the given element to the set, expanding if necessary
    * @param Element - The element to add to the set
    */
    void Add(const ElementType& Element)
    {
        if (NumElements == MaxElements)
        {
            Expand();
        }

        Data[NumElements] = Element;

        NumElements++;
    }

    /***
     * @brief Clears the elements for use
     * @note Does not free the associated memory
    */
    void Empty()
    {
        NumElements = 0;
    }

    /***
    * @brief Return if the set is empty or not
    * @return True if the set is empty, false if non-empty
    */
    bool IsEmpty() const
    {
        return NumElements == 0;
    }

    /***
    * @brief Return the number of elements in the set
    * @return The number of elements in the set
    */
    int Num() const
    {
        return NumElements;
    }

    /***
    * @brief Bracket overload, return element at the given index
    * @param Index The index of the element to return
    * @return The element at the given index
    *
    * @warning This function exits the program if the index is out of bounds, preferring crashing early and
    *          loudly over breaking silently with potential side-effects
    */
    ElementType& operator[](int Index) const
    {
        if(Index >= 0 && Index < NumElements)
        {
            return Data[Index];
        }

        //report invaid access and crash
        fprintf(stderr, "%s:%d: DynamicSet::operator[]: error accessing index %d, invalid index. exiting\n", __FILE__, __LINE__);
        exit(DYSET_INVALID_INDEX);
    }

private:
    int MaxElements = 0;
    int NumElements = 0;
    int NumAllocations = 0;
    const int ExpansionSize = 1024;
    ElementType* Data = nullptr;

    /***
    * @brief Expand the dynamic set, allowing for more elements to be stored
    *
    * @warning This function exits the program if reallocation fails, preferring crashing early and
    *          loudly over breaking silently with potential side-effects
    */
    void Expand()
    {
        NumAllocations++;
        ElementType* reallocdat = (ElementType*) realloc(Data, ExpansionSize * NumAllocations * sizeof(ElementType));
        if (reallocdat != nullptr)
        {
            Data = reallocdat;
            MaxElements = NumAllocations * ExpansionSize;
        }
        else
        {
            //this should realistically never happen, so exiting the application is appropriate
            fprintf(stderr, "%s:%d: DynamicSet::Expand: error re-allocating element array. exiting\n", __FILE__, __LINE__);
            exit(DYSET_ALLOCATION_FAILURE);
        }
    }
};
