/*
 * Authors: Paul Gentemann, Bucky Frost
 * Date: Sun Nov 17 08:23:14 AKST 2013
 * File: displacement_v.glsl
 * For: CS 381 Fall 2013
 *
 * Purpose: Vertex shader for displacement mapping.
 */

#version 330 core 

/*Attributes*/
in vec4 position;
in vec3 normal;

/*Output to the GS*/
out VS_GS_INTERFACE
{
//    vec4 position;
    vec3 normal;
//    vec4 color;
//    vec2 tex_coord[4];
} vs_out;


/*My little pass through*/
void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * position;
    vs_out.normal = normal;
}
