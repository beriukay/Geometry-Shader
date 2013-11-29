/*
 * Authors: Paul Gentemann, Bucky Frost
 * Date: Sun Nov 17 08:23:14 AKST 2013
 * File: displacement_g.glsl
 * For: CS 381 Fall 2013
 *
 * Purpose: geo shader for displacement mapping.
 */

#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

/*input from VS*/
in VS_GS_INTERFACE
{
//    vec4 position;
    vec3 normal;
//    vec4 color;
//    vec2 tex_coord[4];
} vertex_in[];

/*output to the frag*/
/*out GS_FS_VERTEX
{} vertex_out;*/

uniform mat4 MVP;
smooth out vec4 color;


void main()
{   
    for(int i = 0; i < gl_in.length(); ++i)
    {
    	gl_Position = MVP * gl_in[i].gl_Position;
	EmitVertex();
    }
/* 
    for(int i = 0; i < gl_in.length(); ++i)
    {
    	color = vec4(1.0f, 0.3f, 0.3f, 1.0f);
    	gl_Position = MVP * gl_in[i].gl_Position;
	EmitVertex();

	color = vec4(0.0f);
	gl_Position = MVP * (gl_in[i].gl_Position + vec4(vertex_in[i].normal * 0.05f, 0.0f));
	EmitVertex();

	EndPrimitive();
    }
*/
    vec4 cent = (gl_in[0].gl_Position + gl_in[1].gl_Position +
            gl_in[2].gl_Position) / 3.f;
    vec3 face_normal = normalize (
            cross (gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz,
                gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));
 
    gl_Position = MVP * cent;
    color = vec4 (.3f, 1.f, .3f, 1.f);
    EmitVertex ();
 
    gl_Position = MVP * (cent + vec4 (face_normal * .1f, 0.f));
    color = vec4 (0.f);
    EmitVertex ();
 
    EndPrimitive ();    
}
