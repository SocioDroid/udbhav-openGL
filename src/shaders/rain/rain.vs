#version 460 core

in vec4 positionMC;
in vec4 vertexSeed;
in vec4 vertexVelo;

//out VertexData
//{
out float texArrayIDin;
out vec4 velocity;
//} vertex;




void main(void)
{
    texArrayIDin = vertexSeed.w;
    // random enlight value in w component of velocity
    velocity = vertexVelo;
    gl_Position = positionMC;
    //gl_PointSize = 10.0;
}