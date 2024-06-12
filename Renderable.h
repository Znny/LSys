//
// Created by Ryan on 6/12/2024.
//

#ifndef LSYS_RENDERABLE_H
#define LSYS_RENDERABLE_H

#include <glfw3.h>
#include <glm/glm.hpp>


class RenderingPrimitive
{
    /* rendering primitive type
     * valid types are as follows:
     *  GL_POINTS - each vertex is considered a point
     *  GL_LINES - verts 0 and 1 are considered a line, 2 and 3, 4-5, etc.
     *  GL_LINE_STRIP - adjacent vertices are considered lines, i.e. 1 and 2, 2 and 3, 3 and 4, etc.
     *  GL_LINE_LOOP - same as line strips, but first and last vertices also create a line, ensuring a closed loop
     *  GL_TRIANGLES - every 3 verts form a triangle, or one piece of input
     *  GL_TRIANGLE_STRIP - every 3 adjacent verts form a triangle
     *  GL_TRIANGLE_FAN - first vertex is always held fixed
     *
     *  Notes: properties can be enabled via glEnable
     *  POINT PRIMS:
     *      - GL_PROGRAM_POINT_SIZE - gl_PointSize in GLSL
     *      - GL_POINT_SIZE_GRANULARITY
     *      - GL_POINT_SPRITE_COORD_ORIGIN - GL_LOWER_LEFT, GL_UPPER_LEFT
     *      todo: finish filling in below properties
     *  LINE PRIMS:
     *      - GL_LINE_WIDTH
     *  TRIANGLE PRIMS:
     *      - winding order?
     *  AJDACENCY PRIMS:
     *  PATCHES:
     */
    GLenum PrimitiveType;

};

class Renderable
{
public:
    //this should be one of the following:
    //
    GLenum RenderingPrimitive;

};

class Soup : Renderable
{

};

#endif //LSYS_RENDERABLE_H

/*
 *
 * Line, Line-List, Triangles, Triangle-List, etc.
 *
 * RenderableBase
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */


//program object
//  attached shaders
//

//renderable base data

//Vertex Data Layout
    //attributes, locations

// Vertex Data(s)
    //array of vertex data layout
    //element count

    //ideally vertex data which is using the same shader program should be re-using the shader asset


// instance data?