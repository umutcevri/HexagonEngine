#version 450
#extension GL_EXT_buffer_reference : require

struct ObjectBufferData
{
    mat4 renderMatrix;
    vec3 color;
    int isSolidColor;
	vec3 boundingSphereCenter;
    float boundingSphereRadius;
};

layout(buffer_reference, std430) readonly buffer ObjectBuffer {
    ObjectBufferData objects[];
};

layout( push_constant ) uniform constants
{	
	mat4 lightSourceMatrix;
    VertexBuffer vertexBuffer;
    ObjectBuffer objectBuffer;
} PushConstants;

void main() 
{	
	Vertex v = PushConstants.vertexBuffer.vertices[gl_VertexIndex];

    ObjectBufferData o = PushConstants.objectBuffer.objects[gl_InstanceIndex];

    gl_Position = PushConstants.lightSourceMatrix * o.renderMatrix * vec4(v.position, 1.0f);
}