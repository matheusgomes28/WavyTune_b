#version 430
in vec3 aPos;
in vec3 aNor;
in vec4 aCol;

out vec3 normal;
out vec4 colour;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 rotate;
void main()
{
    normal = aNor.yzx;
    colour = aCol;
    gl_Position = proj*view*rotate*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}