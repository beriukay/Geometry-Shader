/*
 * Authors: Paul Gentemann, Bucky Frost
 * Tue Nov 26 03:16:40 AKST 2013
 * File: passthrough_v.glsl
 * For: CS 381 Fall 2013
 *	Geometry Shader Project
 * Purpose: A simple passthrough shader.
 */
#version 330 compatibility 
/*Compatibility so that gl_Color can be used.
  Remove depricated aspects?*/

struct vData
{
    vec3 normal;
    vec4 color;
};

out vData vertex;

void main()
{
    vertex.normal = gl_NormalMatrix * gl_Normal;
    vertex.color = gl_Color;
    gl_Position = ftransform();
}
