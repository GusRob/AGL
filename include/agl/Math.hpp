/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#ifndef MATH_H
#define MATH_H

#include <AGLTypes.hpp>
#include <simd/simd.h>

namespace AGL {

/* ****************************************************************************************** */
/* Type Declarations                                                                          */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Variables                                                                                  */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Function Prototypes                                                                        */
/* ****************************************************************************************** */

	float32_t Length(simd::float3 Vector);

	float32_t Length(simd::float2 Vector);

	simd::float3 Normalise(simd::float3 Vector);

	simd::float3 CrossProduct(simd::float3 A, simd::float3 B);

	simd::float3 TriangleNormal(simd::float3 A, simd::float3 B, simd::float3 C);

	bool AreColinear(simd::float3 A, simd::float3 B, simd::float3 C);

	bool FloatEqual(simd::float3 A, simd::float3 B);

	bool FloatEqual(simd::float2 A, simd::float2 B);

	bool FloatEqual(float32_t A, float32_t B);

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL

//tmp to remove
namespace math
{
    inline constexpr simd::float3 add( const simd::float3& a, const simd::float3& b )
    {
        return { a.x + b.x, a.y + b.y, a.z + b.z };
    }

    inline constexpr simd_float4x4 makeIdentity()
    {
        using simd::float4;
        return (simd_float4x4){ (float4){ 1.f, 0.f, 0.f, 0.f },
                                (float4){ 0.f, 1.f, 0.f, 0.f },
                                (float4){ 0.f, 0.f, 1.f, 0.f },
                                (float4){ 0.f, 0.f, 0.f, 1.f } };
    }

    inline simd::float4x4 makePerspective( float fovRadians, float aspect, float znear, float zfar )
    {
        using simd::float4;
        float ys = 1.f / tanf(fovRadians * 0.5f);
        float xs = ys / aspect;
        float zs = zfar / ( znear - zfar );
        return simd_matrix_from_rows((float4){ xs, 0.0f, 0.0f, 0.0f },
                                     (float4){ 0.0f, ys, 0.0f, 0.0f },
                                     (float4){ 0.0f, 0.0f, zs, znear * zs },
                                     (float4){ 0, 0, -1, 0 });
    }

    inline simd::float4x4 makeXRotate( float angleRadians )
    {
        using simd::float4;
        const float a = angleRadians;
        return simd_matrix_from_rows((float4){ 1.0f, 0.0f, 0.0f, 0.0f },
                                     (float4){ 0.0f, cosf( a ), sinf( a ), 0.0f },
                                     (float4){ 0.0f, -sinf( a ), cosf( a ), 0.0f },
                                     (float4){ 0.0f, 0.0f, 0.0f, 1.0f });
    }

    inline simd::float4x4 makeYRotate( float angleRadians )
    {
        using simd::float4;
        const float a = angleRadians;
        return simd_matrix_from_rows((float4){ cosf( a ), 0.0f, sinf( a ), 0.0f },
                                     (float4){ 0.0f, 1.0f, 0.0f, 0.0f },
                                     (float4){ -sinf( a ), 0.0f, cosf( a ), 0.0f },
                                     (float4){ 0.0f, 0.0f, 0.0f, 1.0f });
    }

    inline simd::float4x4 makeZRotate( float angleRadians )
    {
        using simd::float4;
        const float a = angleRadians;
        return simd_matrix_from_rows((float4){ cosf( a ), sinf( a ), 0.0f, 0.0f },
                                     (float4){ -sinf( a ), cosf( a ), 0.0f, 0.0f },
                                     (float4){ 0.0f, 0.0f, 1.0f, 0.0f },
                                     (float4){ 0.0f, 0.0f, 0.0f, 1.0f });
    }

    inline simd::float4x4 makeTranslate( const simd::float3& v )
    {
        using simd::float4;
        const float4 col0 = { 1.0f, 0.0f, 0.0f, 0.0f };
        const float4 col1 = { 0.0f, 1.0f, 0.0f, 0.0f };
        const float4 col2 = { 0.0f, 0.0f, 1.0f, 0.0f };
        const float4 col3 = { v.x, v.y, v.z, 1.0f };
        return simd_matrix( col0, col1, col2, col3 );
    }

    inline simd::float4x4 makeScale( const simd::float3& v )
    {
        using simd::float4;
        return simd_matrix((float4){ v.x, 0, 0, 0 },
                           (float4){ 0, v.y, 0, 0 },
                           (float4){ 0, 0, v.z, 0 },
                           (float4){ 0, 0, 0, 1.0 });
    }
    inline simd::float3x3 discardTranslation( const simd::float4x4& m )
    {
        return simd_matrix( m.columns[0].xyz, m.columns[1].xyz, m.columns[2].xyz );
    }
}

#endif // define MATH_H
