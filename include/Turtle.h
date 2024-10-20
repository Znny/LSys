//
// Created by Ryanc on 10/20/2024.
//

#ifndef TURTLE_H
#define TURTLE_H

#include "Transform.h"
#include "ColoredTriangle.h"
#include "lindenmayer.h"


/* Turtle class
 * used for interpreting and drawing L-Systems
 */
class Turtle
{
public:
    Turtle()
            : MyTransform()
    //Location(glm::vec3(0)), Forwards(glm::vec3(0.0, 1.0, 0.0)), Right(glm::vec3(1.0, 0.0, 0.0))
    {

    }

    /** Turtle::DrawSystem
     * @param System - the system to draw
     * @return A list of ColoredTriangles
     */
    ColoredTriangleList* DrawSystem(LSystem& System);

    Transform MyTransform;
};


#endif //TURTLE_H
