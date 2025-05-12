/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#ifndef SCENEIMPL_H
#define SCENEIMPL_H

#include <AGLTypes.hpp>
#include <Object.hpp>
#include <DynamicBuffer.hpp>
#include <StaticBuffer.hpp>
#include <Metal/Metal.hpp>

namespace AGL {

#define C_VERTEX_BUFFER_LENGTH (uint32_t)(5000000 * sizeof(Vertex_s))
#define C_CAMERA_BUFFER_LENGTH (uint32_t)(10 * sizeof(CameraData_s))
#define C_OBJECT_BUFFER_LENGTH (uint32_t)(100000 * sizeof(ObjectData_s))
#define C_MATERIAL_BUFFER_LENGTH (uint32_t)(100000 * sizeof(MaterialData_s))

#define C_INDEX_BUFFER_LENGTH (uint32_t)(5000000 * sizeof(uint32_t))

/* ****************************************************************************************** */
/* Non-Member Variables                                                                       */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Class Declaration                                                                          */
/* ****************************************************************************************** */

	class Window_c;
	class Scene_ci {

/* ****************************************************************************************** */
/* Member Variables                                                                           */
/* ****************************************************************************************** */

	private:
		MTL::Device* prv_pDevice;

		StaticBuffer_s<Vertex_s> *prv_pVertexBuffer;
		StaticBuffer_s<uint32_t> *prv_pIndexBuffer;

		std::vector<Object_s> prv_Objects;
		std::vector<Material_s> prv_Materials;

		DynamicBuffer_s<ObjectData_s> *prv_pObjectBuffer;
		DynamicBuffer_s<CameraData_s> *prv_pCameraBuffer;

		uint8_t prv_frame = 0;

/* ****************************************************************************************** */
/* Member Constructor/Destructors                                                             */
/* ****************************************************************************************** */

	public:
		~Scene_ci();
		Scene_ci(const Window_c& WindowForScene);

/* ****************************************************************************************** */
/* Member Function Prototypes                                                                 */
/* ****************************************************************************************** */

	public:
		void BuildBuffers();
		void DrawScene(MTL::RenderCommandEncoder &pEnc);

		uint32_t AddObjectFromFile(const std::string &ObjFilepath);
		uint32_t AddObjectFromData(std::vector<Vertex_s> &Vertices, std::vector<uint32_t> &Indices);

		const simd::float3 GetObjectPosition(uint32_t ObjectIndex);
		const simd::float4x4 GetObjectRotation(uint32_t ObjectIndex);
		const simd::float3 GetObjectScale(uint32_t ObjectIndex);
		void SetObjectPosition(uint32_t ObjectIndex, const simd::float3 PositionToSet);
		void SetObjectRotation(uint32_t ObjectIndex, const simd::float4x4 RotationToSet);
		void SetObjectScale(uint32_t ObjectIndex, const simd::float3 ScaleToSet);

	public:
		friend std::ostream& operator<<(std::ostream &os, const Scene_ci &Scene);

/* ****************************************************************************************** */
/* Class Declaration End                                                                      */
/* ****************************************************************************************** */

	}; // class Scene_ci

/* ****************************************************************************************** */
/* Non-Member Function Prototypes                                                             */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL

#endif // define SCENEIMPL_H
