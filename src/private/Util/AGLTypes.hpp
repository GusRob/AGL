/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#ifndef AGLTYPES_H
#define AGLTYPES_H

#include <simd/simd.h>
#include <iostream>
#include <Metal/Metal.hpp>

namespace AGL {

#define C_BUFFER_DATA_TYPE_VERTEX   (0)
#define C_BUFFER_DATA_TYPE_OPACITY  (1)
#define C_BUFFER_DATA_TYPE_CAMERA   (2) /* index is at 12 */
#define C_BUFFER_DATA_TYPE_OBJECT   (3) /* index is at 13 */

#define C_BUFFER_DATA_TYPE_INDEX_OFFSET (10)

#define C_BUFFER_DATA_TYPE_INDEX    (99)

/* ****************************************************************************************** */
/* Type Declarations                                                                          */
/* ****************************************************************************************** */

	struct Vertex_s
	{
		simd::float3 VertexPosition;
		simd::float3 VertexNormal;
		simd::float2 TexturePosition;
	};

/* ------------------------------------------------------------------------------------------ */

	struct ObjectData_s
	{
		simd::float3   Position = {0, 0, 0};
		simd::float4x4 Rotation = simd_matrix4x4(simd_quaternion(0.0f, 0.0f, 0.0f, 1.0f));
		simd::float3   Scale = {1, 1, 1};
	};

/* ------------------------------------------------------------------------------------------ */

	struct CameraData_s
	{
		simd::float4x4 perspectiveTransform;
		simd::float4x4 worldTransform;
		simd::float3x3 worldNormalTransform;
	};

/* ****************************************************************************************** */
/* OStream Overloads                                                                          */
/* ****************************************************************************************** */

std::ostream& operator<<(std::ostream &os, const AGL::Vertex_s &Vertex);

std::ostream& operator<<(std::ostream &os, const AGL::ObjectData_s &ObjectData);

std::ostream& operator<<(std::ostream &os, const AGL::CameraData_s &CameraData);

std::ostream& operator<<(std::ostream &os, const simd::float4x4 &Matrix);

std::ostream& operator<<(std::ostream &os, const simd::float4 &Vector);

std::ostream& operator<<(std::ostream &os, const simd::float3 &Vector);

std::ostream& operator<<(std::ostream &os, const simd::float2 &Vector);

/* ****************************************************************************************** */
/* Namespace End                                                                              */
/* ****************************************************************************************** */

} // namespace AGL

/* ****************************************************************************************** */
/* Preprocessor End                                                                           */
/* ****************************************************************************************** */

#endif // define AGLTYPES_H
