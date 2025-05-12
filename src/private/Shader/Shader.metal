#include <metal_stdlib>
using namespace metal;

struct v2f
{
	float4 position [[position]];
	float3 normal;
	float2 texcoord;
};

struct VertexData_s
{
	float3 position;
	float3 normal;
	float2 texcoord;
};

struct ObjectData_s
{
	float3 position;
	float4x4 rotation;
	float3 scale;
};

struct CameraData_s
{
	float4x4 perspectiveTransform;
	float4x4 worldTransform;
	float3x3 worldNormalTransform;
};

v2f vertex vertexMain(device const VertexData_s* vertexData [[buffer(0)]],
					  device const CameraData_s* cameraData [[buffer(2)]],
					  device const uint& cameraDataIndex [[buffer(12)]],
					  device const ObjectData_s* objectData [[buffer(3)]],
					  device const uint& objectDataIndex [[buffer(13)]],
					  uint vertexId [[vertex_id]])
{

	v2f o;

	const device VertexData_s& vd = vertexData[vertexId];

	// Build model matrix with non-uniform scale and rotation
	float4x4 modelMatrix = float4x4(
		float4(objectData[objectDataIndex].scale.x * objectData[objectDataIndex].rotation[0].xyz, 0.0),
		float4(objectData[objectDataIndex].scale.y * objectData[objectDataIndex].rotation[1].xyz, 0.0),
		float4(objectData[objectDataIndex].scale.z * objectData[objectDataIndex].rotation[2].xyz, 0.0),
		float4(objectData[objectDataIndex].position, 1.0)
	);

	// Transform position
	float4 localPos = float4(vd.position, 1.0);
	float4 worldPos = modelMatrix * localPos;
	float4 viewProjPos = cameraData[cameraDataIndex].perspectiveTransform * cameraData[cameraDataIndex].worldTransform * worldPos;
	o.position = viewProjPos;

	// Transform normal (approximate normal matrix from modelMatrix)
	float3x3 normalMatrix = float3x3(
		modelMatrix[0].xyz,
		modelMatrix[1].xyz,
		modelMatrix[2].xyz
	);
	float3 worldNormal = normalize(normalMatrix * vd.normal);
	o.normal = worldNormal;

	// Texture coordinates (flip Y)
	o.texcoord = float2(vd.texcoord.x, 1.0 - vd.texcoord.y);

	return o;
}

half4 fragment fragmentMain( v2f in [[stage_in]],
							texture2d< half, access::sample > tex [[texture(0)]],
							constant float& opacity [[buffer(1)]])
{
    constexpr sampler s( address::repeat, filter::linear );
    half4 texel = tex.sample( s, in.texcoord );

    // assume light coming from (front-top-right)
    float3 l = normalize(float3( 1.0, 1.0, 0.8 ));
    float3 n = normalize( in.normal );

    half ndotl = half( saturate( dot( n, l ) ) );

    half3 illum = (texel.rgb * 0.1) + (texel.rgb * ndotl);
    return half4( illum, opacity);
}
