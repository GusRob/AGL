/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#ifndef SCENE_H
#define SCENE_H

#include <AGLTypes.hpp>

namespace AGL {

/* ****************************************************************************************** */
/* Non-Member Variables                                                                       */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Implementation Class Declaration                                                           */
/* ****************************************************************************************** */

	class Window_c;
	class Scene_ci;

/* ****************************************************************************************** */
/* Class Declaration                                                                          */
/* ****************************************************************************************** */

	class Scene_c {
		friend class Window_ci;

/* ****************************************************************************************** */
/* Member Variables                                                                           */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Member Constructor/Destructors                                                             */
/* ****************************************************************************************** */

	public:
		~Scene_c();
		Scene_c(const Window_c &WindowForScene);

/* ****************************************************************************************** */
/* Member Function Prototypes                                                                 */
/* ****************************************************************************************** */

	public:
		uint32_t AddObjectFromFile(const std::string &ObjFilepath);
		uint32_t AddObjectFromData(std::vector<Vertex_s> &Vertices, std::vector<uint32_t> &Indices);

		const simd::float3 GetObjectPosition(uint32_t ObjectIndex);
		const simd::float4x4 GetObjectRotation(uint32_t ObjectIndex);
		const simd::float3 GetObjectScale(uint32_t ObjectIndex);
		void SetObjectPosition(uint32_t ObjectIndex, const simd::float3 PositionToSet);
		void SetObjectRotation(uint32_t ObjectIndex, const simd::float4x4 RotationToSet);
		void SetObjectScale(uint32_t ObjectIndex, const simd::float3 ScaleToSet);

		friend std::ostream& operator<<(std::ostream &os, const Scene_c &Scene);

/* ****************************************************************************************** */
/* Implementation Class Pointer                                                               */
/* ****************************************************************************************** */

	private:
		std::unique_ptr<Scene_ci> prv_pImpl;

/* ****************************************************************************************** */
/* Class Declaration End                                                                      */
/* ****************************************************************************************** */

	}; // class Scene_c

/* ****************************************************************************************** */
/* Non-Member Function Prototypes                                                             */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL

#endif // define SCENE_H
