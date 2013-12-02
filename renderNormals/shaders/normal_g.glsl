#version 330 compatibility// core
#pragma debug(on)
/*
 * Authors: Paul Gentemann, Bucky Frost
 * Date: Sun Nov 17 08:23:14 AKST 2013
 * File: displacement_g.glsl
 * For: CS 381 Fall 2013
 *
 * Purpose: geo shader for displacement mapping.
 */



layout (triangles) in;
layout (line_strip, max_vertices = 8) out;
uniform mat4 MV;
uniform mat4 P;
/*input from VS*/
in VS_GS_INTERFACE
{
//    vec4 position;
    vec3 normal;
//    vec4 color;
//    vec2 tex_coord[4];
} vertex[];


/*output to the frag*/
/*out GS_FS_VERTEX
{} vertex_out;*/

/*
uniform mat4 MV;
uniform mat4 P;
uniform float expansion_factor;
smooth out vec4 color;
*/
float normal_length = .7;
void main()
{
    vec4 color = vec4(1.f,.3f,.3f,1.f);
    for(int i = 0; i < gl_in.length(); ++i)
    {
        //gl_Position = gl_ModelViewProjectionMatrix * gl_in[n].gl_Position;
        //EmitVertex();

     //gl_Position = gl_ModelViewProjectionMatrix * (gl_in[i].gl_Position + vec4 (vertexData[i].normal
                            //* .05f, 0.f));


    vec3 P = gl_in[i].gl_Position.xyz;
    vec3 N = vertex[i].normal.xyz;

    gl_Position = gl_ModelViewProjectionMatrix * vec4(P, 1.0);
    //color = vertex[i].color;
    EmitVertex();

    gl_Position = gl_ModelViewProjectionMatrix * vec4(P + N * normal_length, 1.0);
    //color = vertex[i].color;
    EmitVertex();

    EndPrimitive();
    }
    EndPrimitive();

/*VISUALIZE THE NORMALS OF FACES*-/
    //------ One line for the face normal
    //
    vec3 P0 = gl_in[0].gl_Position.xyz;
    vec3 P1 = gl_in[1].gl_Position.xyz;
    vec3 P2 = gl_in[2].gl_Position.xyz;

    vec3 V0 = P0 - P1;
    vec3 V1 = P2 - P1;

    vec3 N = cross(V1, V0);
    N = normalize(N);

    // Center of the triangle
    vec3 P = (P0+P1+P2) / 3.0;

    gl_Position = gl_ModelViewProjectionMatrix * vec4(P, 1.0);
    color = vec4(1, 0, 0, 1);
    EmitVertex();

    gl_Position = gl_ModelViewProjectionMatrix * vec4(P + N * normal_length, 1.0);
    color = vec4(1, 0, 0, 1);
    EmitVertex();
    EndPrimitive();
/*    vec4 cent = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.f;
    vec3 face_normal = normalize (
            cross (gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz,
                gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));

    gl_Position = gl_ModelViewProjectionMatrix * cent;
    color = vec4 (.3f, 1.f, .3f, 1.f);
    EmitVertex ();

    gl_Position = gl_ModelViewProjectionMatrix * (cent + vec4 (face_normal * .1f, 0.f));
    color = vec4 (1.f);
    EmitVertex ();

    EndPrimitive ();
    */
}
