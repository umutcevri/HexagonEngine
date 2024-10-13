#version 450
#extension GL_EXT_buffer_reference : require

layout (location = 0) out vec3 outColor;

struct Vertex {

	vec3 position;
	float uv_x;
	vec3 normal;
	float uv_y;
	vec4 color;
	int textureID;
};

struct ObjectBufferData
{
    mat4 renderMatrix;
    vec3 color;
};

layout(buffer_reference, std430) readonly buffer VertexBuffer{ 
	Vertex vertices[];
};

layout(buffer_reference, std430) readonly buffer ObjectsBuffer {
    ObjectBufferData objects[];
};

//push constants block
layout( push_constant ) uniform constants
{	
	mat4 render_matrix;
	VertexBuffer vertexBuffer;
	ObjectsBuffer objectsBuffer;
} PushConstants;

void main() 
{	
	//load vertex data from device adress
	Vertex v = PushConstants.vertexBuffer.vertices[gl_VertexIndex];
	ObjectsBufferData d = PushConstants.objectsBuffer.objects[gl_InstanceIndex];
	mat4 modelMatrix = d.renderMatrix;

	//output data
	gl_Position = PushConstants.render_matrix * modelMatrix * vec4(v.position, 1.0f);
	outColor = d.color;
}