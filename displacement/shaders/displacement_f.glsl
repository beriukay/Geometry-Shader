/*
 * Authors: Paul Gentemann, Bucky Frost
 * Date: Sun Nov 17 08:23:14 AKST 2013
 * File: displacement_f.glsl
 * For: CS 381 Fall 2013
 *
 * Purpose: frag shader for displacement mapping.
 */
#version 330

struct vData
{
    vec3 normal;
    vec4 color;
};

in vData frag;

void main()
{
    gl_FragColor = frag.color;
}
