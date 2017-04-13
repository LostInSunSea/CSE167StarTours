#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;


// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;
//uniform mat3 normal_m;
//Pass in Normal Matrix?

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out vec3 Normal;
out vec3 FragPos;
out vec3 objNormal;


void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    FragPos = vec3(modelview * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(modelview))) * normal;
    objNormal = normal;
    //Normal = normal_m * normal;

    //Normal = mat3(transpose(inverse(modelview)) * vec3(1.0f);
}
