//
// Created by ryan on 5/31/24.
//
#pragma once

#define MaxReplacementLength 48
struct LS_RewritingRule
{
    LS_RewritingRule() : LS_RewritingRule(' ', ""){}
    LS_RewritingRule(char c, const char* R);

    char Character = ' ';
    char RString[MaxReplacementLength]={0};
};

/* LSystem
 * A representation of a Lindenmayer System
 * Contains variables for rewriting, like the initial axiom, a number of iterations,
 *  and rewriting rules for each iteration
 * Also contains variables related to rendering the L-System using a turtle,
 *  such as the distance to move, or the angle to rotate by
 */
class LSystem
{
  friend class UIManager;
  friend class Turtle;
public:
    LSystem();

    /** SetName
     * Sets the name of the L-System
     **/
    void SetName(const char* NewName);

    /** AddRule
     * Adds a rewriting rule to the L-System, for when the given character is read, it is replaced with RewrittenString
     * @param character - the character to be rewritten
     * @param RewrittenString - the string the character should be replaced with
     */
    void AddRule(char character, const char* RewrittenString);

    /** AddRuleFromString
     * Adds a rewriting rule from the given string. The provided string should be in the format C:RWRULE,
     * where C is the character we're creating a rule for, and RWRULE is the string which replaces it.
     * IF a rule already exists, it will be overwritten
     */
     void AddRuleFromString(const char* String);

    /** SetAxiom
     * Sets the initial string, or starting point, of the L-System
     * @param NewAxiom - the new axiom string
     */
    void SetAxiom(const char* NewAxiom);

    /** SetIterations
     * Sets the number of iterations to be run when Rewrite is called
     * @param NewIterations - the new number of iterations
     */
    void SetIterations(int NewIterations);

    /** SetAngle
     * Sets the Angle to rotate
     * @param NewAngle - the new Angle of the system
     */
    void SetAngle(float NewAngle);

    /** SetDistance
     * Sets the distance to move
     * @param NewDistance - the new distance to use for the system
     */
    void SetDistance(float NewDistance);

    /** LoadFromFile
     * Load L-System settings from a file
     * @param Filename
     */
    void LoadFromFile(const char* Filename);

    /** LoadFromFile
     * Save L-System settings to the given file
     * @param Filename
     */
    void SaveToFile(const char* Filename);

    void Rewrite();
    void Reset();

protected:

    //name of the system
    char* Name = nullptr;

    //initial string which rewriting works off of
    char* Axiom = nullptr;

    //the generated string from a number of rewritings, is used as an intermediary if multiple iterations occur
    char* GeneratedString = nullptr;

    //rules for rewriting the axiom or generated string for each iteration of rewriting
    LS_RewritingRule RewritingRules[128];

    //the number of times the string should be rewritten, using the rewriting rules provided
    int Iterations = 1.0f;

    //the distance a turtle should move when a move command is read
    float Distance = 1.0f;

    //the angle a turtle should rotate when a rotation command is read
    float Angle = 90.0f;
};

