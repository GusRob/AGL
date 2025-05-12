/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#include <Math.hpp>

namespace AGL {

/* ****************************************************************************************** */
/* Private Variables                                                                          */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Private Function Prototypes                                                                */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Function Implementations                                                                   */
/* ****************************************************************************************** */

	float32_t Length(simd::float3 Vector) {
		return std::sqrt((Vector[0]*Vector[0]) + (Vector[1]*Vector[1]) + (Vector[2]*Vector[2]));
	}

/* ------------------------------------------------------------------------------------------ */
	float32_t Length(simd::float2 Vector) {
		return std::sqrt((Vector[0]*Vector[0]) + (Vector[1]*Vector[1]));
	}

/* ------------------------------------------------------------------------------------------ */

	simd::float3 Normalise(simd::float3 Vector) {
		if(Length(Vector) == 0) {
			//return {0, 1, 0};
		}
		return Vector / Length(Vector);
	}

/* ------------------------------------------------------------------------------------------ */

	simd::float3 CrossProduct(simd::float3 A, simd::float3 B) {
		simd::float3 Result;
		Result[0] = A[1]*B[2] - A[2]*B[1];
		Result[1] = A[2]*B[0] - A[0]*B[2];
		Result[2] = A[0]*B[1] - A[1]*B[0];
		return Result;
	}

/* ------------------------------------------------------------------------------------------ */

	simd::float3 TriangleNormal(simd::float3 A, simd::float3 B, simd::float3 C) {
		return Normalise(CrossProduct(B-A, C-A));
	}

/* ------------------------------------------------------------------------------------------ */

	bool AreColinear(simd::float3 A, simd::float3 B, simd::float3 C) {
		float32_t AB = Length(B-A);
		float32_t AC = Length(C-A);
		float32_t BC = Length(C-B);
		bool ABLongest = FloatEqual(AB, AC + BC);
		bool ACLongest = FloatEqual(AC, AB + BC);
		bool BCLongest = FloatEqual(BC, AB + AC);

		return ABLongest || ACLongest || BCLongest;
	}

/* ------------------------------------------------------------------------------------------ */

	bool FloatEqual(simd::float3 A, simd::float3 B) {
		return std::fabs(Length(A-B)) < std::numeric_limits<float>::epsilon();
	}

/* ------------------------------------------------------------------------------------------ */

	bool FloatEqual(simd::float2 A, simd::float2 B) {
		return std::fabs(Length(A-B)) < std::numeric_limits<float>::epsilon();
	}

/* ------------------------------------------------------------------------------------------ */

	bool FloatEqual(float32_t A, float32_t B) {
		return std::fabs(A-B) < std::numeric_limits<float>::epsilon();
	}

/* ****************************************************************************************** */
/* Private Function Implementations                                                           */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL
