/*
 * Authors: Paul Gentemann, Bucky Frost
 * Date: Sun Nov 17 08:23:14 AKST 2013
 * File: displacement_g.glsl
 * For: CS 381 Fall 2013
 *
 * Purpose: geo shader for displacement mapping.
 */

#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main()
{
    int i;
    for(i = 0;i < gl_in.length();i++)
    {

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
