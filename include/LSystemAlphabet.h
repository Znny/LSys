//
// Created by Ryan on 6/9/2024.
//

#ifndef LSYS_LSYSTEMALPHABET_H
#define LSYS_LSYSTEMALPHABET_H

#include <map>


typedef void* RuleReturnType;

typedef RuleReturnType (*RuleFunction)(void* data);

struct CharacterRuleMapping
{
    char Character;
    RuleFunction Operate;
};

//Map datastructure, just created an alias for
template <typename KeyType, typename ValueType>
class Map
{
public:

    Map<KeyType, ValueType>(KeyType K, ValueType V)
    : internal_data()
    {
        //this left blank
    }

    std::map<KeyType, ValueType>& operator->()
    {
        return internal_data;
    }

private:
    std::map<KeyType, ValueType> internal_data;
};

//alias for RuleFunction, Rule
typedef RuleFunction Rule;

class LSystemAlphabet
{
public:
    LSystemAlphabet()
    : CharacterRuleMapping('\0', nullptr)
    {
        //initialize with a single rule entry for 0:nullptr KV pair
    }

    //accessor
    Map<char, Rule>& Rules()
    {
        return CharacterRuleMapping;
    }

private:
    Map<char, Rule> CharacterRuleMapping;
};



#endif //LSYS_LSYSTEMALPHABET_H
