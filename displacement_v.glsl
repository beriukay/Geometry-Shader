/*
 * Authors: Paul Gentemann, Bucky Frost
 * Date: Sun Nov 17 08:23:14 AKST 2013
 * File: displacement_v.glsl
 * For: CS 381 Fall 2013
 *
 * Purpose: Vertex shader for displacement mapping.
 */

//This pass through shader is based  off of 
//http://www.geeks3d.com/20111111/simple-introduction-to-geometry-shaders-glsl-opengl-tutorial-part1

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main()
{
    vec4 world_pos = modelMatrix * gl_Vertex;
    vec4 view_pos = viewMatrix * world_pos;
    gl_Position = projectionMatrix * view_pos;
}
