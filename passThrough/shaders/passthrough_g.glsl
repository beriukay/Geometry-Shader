/*
 * Authors: Paul Gentemann, Bucky Frost
 * Date: Tue Nov 26 03:07:56 AKST 2013
 * File: displacement_g.glsl
 * For: CS 381 Fall 2013
 *      Class Geometry Shader Project
 * Purpose: A simple pass through geometry shader
 */

/*Version depends on supporting hardware*/
#version 330 
//#version 400


/*Specifying the formats the the geometry*/
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main()
{
    /*For each passed element, simply emit its pos*/
    for(int i = 0; i < gl_in.length(); i++)
    {

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
