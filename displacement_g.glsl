/*
 * Authors: Paul Gentemann, Bucky Frost
 * Date: Sun Nov 17 08:23:14 AKST 2013
 * File: displacement_g.glsl
 * For: CS 381 Fall 2013
 *
 * Purpose: Generate additional geometry based on a height map.
 */

//This pass through shader is passed off of 
//http://www.geeks3d.com/20111111/simple-introduction-to-geometry-shaders-glsl-opengl-tutorial-part1

#version 330 compatibility 

// Specifing the input and output primitive types.
layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

void main()
{
    //looping over all the input
    for (int i = 0; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
	EmitVertex();
    }

    EndPrimitive();
    
}
