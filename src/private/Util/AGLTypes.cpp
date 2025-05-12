/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#include <AGLTypes.hpp>

namespace AGL {

/* ****************************************************************************************** */
/* OStream Overload Implementations                                                           */
/* ****************************************************************************************** */


std::ostream& operator<<(std::ostream &os, const AGL::Vertex_s &Vertex) {
	os << "Vertex_s(" << Vertex.VertexPosition << ", " << Vertex.VertexNormal << ", " << Vertex.TexturePosition << ")";
	return os;
}

/* ------------------------------------------------------------------------------------------ */

std::ostream& operator<<(std::ostream &os, const AGL::ObjectData_s &ObjectData) {
	os << "ObjectData_s(" << ObjectData.Position << ObjectData.Rotation << ObjectData.Scale;
	return os;

}

/* ------------------------------------------------------------------------------------------ */

std::ostream& operator<<(std::ostream &os, const AGL::CameraData_s &CameraData) {
	os << "CameraData_s()";
	return os;
}

/* ------------------------------------------------------------------------------------------ */

std::ostream& operator<<(std::ostream &os, const simd::float4x4 &Matrix) {
	os << "float4x4(" << Matrix.columns[0] << ", " << Matrix.columns[1] << ", " << Matrix.columns[2] <<  ", " << Matrix.columns[3] << ")";
	return os;

}

/* ------------------------------------------------------------------------------------------ */

std::ostream& operator<<(std::ostream &os, const simd::float4 &Vector) {
	os << "float4(" << Vector[0] << ", " << Vector[1] << ", " << Vector[2] <<  ", " << Vector[3] << ")";
	return os;

}

/* ------------------------------------------------------------------------------------------ */

std::ostream& operator<<(std::ostream &os, const simd::float3 &Vector) {
	os << "float3(" << Vector[0] << ", " << Vector[1] << ", " << Vector[2] << ")";
	return os;

}

/* ------------------------------------------------------------------------------------------ */

std::ostream& operator<<(std::ostream &os, const simd::float2 &Vector) {
	os << "float2(" << Vector[0] << ", " << Vector[1] << ")";
	return os;
}

/* ****************************************************************************************** */
/* Namespace End                                                                              */
/* ****************************************************************************************** */

} // namespace AGL
