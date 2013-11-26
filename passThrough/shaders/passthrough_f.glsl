/*
 * Authors: Paul Gentemann, Bucky Frost
 * Tue Nov 26 03:16:40 AKST 2013
 * File: passthrough_f.glsl
 * For: CS 381 Fall 2013
 *	Geometry Shader Project
 * Purpose: A simple passthrough shader.
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
