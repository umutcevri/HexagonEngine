#version 450
#extension GL_EXT_buffer_reference : require

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec2 outUV;
layout (location = 2) flat out int outTextureID;
layout (location = 3) flat out int outIsSolidColor;
layout (location = 4) out vec3 outNormal;
layout (location = 5) out vec3 fragPos;

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
    int isSolidColor;
	vec3 boundingSphereCenter;
    float boundingSphereRadius;
};

layout(buffer_reference, std430) readonly buffer VertexBuffer{ 
	Vertex vertices[];
};

layout(buffer_reference, std430) readonly buffer ObjectBuffer {
    ObjectBufferData objects[];
};

layout(buffer_reference, std430) readonly buffer VisibleInstanceBuffer {
    uint visibleInstanceIDs[];
};

//push constants block
layout( push_constant ) uniform constants
{	
	mat4 render_matrix;
	VertexBuffer vertexBuffer;
	ObjectBuffer objectBuffer;
	VisibleInstanceBuffer visibleInstanceBuffer;
} PushConstants;

void main() 
{	
	//load vertex data from device adress
	Vertex v = PushConstants.vertexBuffer.vertices[gl_VertexIndex];

	uint instanceID =  PushConstants.visibleInstanceBuffer.visibleInstanceIDs[gl_InstanceIndex];
    ObjectBufferData o = PushConstants.objectBuffer.objects[instanceID];

	mat4 modelMatrix = o.renderMatrix;

	//output data
	gl_Position = PushConstants.render_matrix * modelMatrix * vec4(v.position, 1.0f);
	fragPos = vec3(modelMatrix * vec4(v.position, 1.0));
	outColor = o.color;
	outUV.x = v.uv_x;
	outUV.y = v.uv_y;
	outTextureID = v.textureID;
	outIsSolidColor = o.isSolidColor;
	outNormal = mat3(transpose(inverse(modelMatrix))) * v.normal;

}