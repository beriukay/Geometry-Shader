/*
 * Authors: Paul Gentemann, Bucky Frost
 * Date: Sun Nov 17 08:23:14 AKST 2013
 * File: displacement_v.glsl
 * For: CS 381 Fall 2013
 *
 * Purpose: Vertex shader for displacement mapping.
 */
#version 330 compatibility

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
