/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#include <Scene.hpp>
#include <SceneImpl.hpp>
#include <Window.hpp>

namespace AGL {

/* ****************************************************************************************** */
/* Private Non-Member Variables                                                               */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Private Non-Member Function Prototypes                                                     */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Member Constructor/Destructor Implementations                                              */
/* ****************************************************************************************** */

	Scene_c::~Scene_c() = default;

/* ------------------------------------------------------------------------------------------ */

	Scene_c::Scene_c(const Window_c& WindowForScene) {
		//Create the pointer to the implementation class
		prv_pImpl = std::unique_ptr<Scene_ci>(new Scene_ci(WindowForScene));
	}

/* ****************************************************************************************** */
/* Member Function Implementations                                                            */
/* ****************************************************************************************** */

	uint32_t Scene_c::AddObjectFromFile(const std::string &ObjFilepath) {
		return prv_pImpl->AddObjectFromFile(ObjFilepath);
	}

/* ------------------------------------------------------------------------------------------ */

	uint32_t Scene_c::AddObjectFromData(std::vector<Vertex_s> &Vertices, std::vector<uint32_t> &Indices) {
		return prv_pImpl->AddObjectFromData(Vertices, Indices);
	}

/* ------------------------------------------------------------------------------------------ */

	const simd::float3 Scene_c::GetObjectPosition(uint32_t ObjectIndex) {
		return prv_pImpl->GetObjectPosition(ObjectIndex);
	}
/* ------------------------------------------------------------------------------------------ */
	const simd::float4x4 Scene_c::GetObjectRotation(uint32_t ObjectIndex) {
		return prv_pImpl->GetObjectRotation(ObjectIndex);
	}
/* ------------------------------------------------------------------------------------------ */
	const simd::float3 Scene_c::GetObjectScale(uint32_t ObjectIndex) {
		return prv_pImpl->GetObjectScale(ObjectIndex);
	}
/* ------------------------------------------------------------------------------------------ */
	void Scene_c::SetObjectPosition(uint32_t ObjectIndex, const simd::float3 PositionToSet) {
		prv_pImpl->SetObjectPosition(ObjectIndex, PositionToSet);
	}
/* ------------------------------------------------------------------------------------------ */
	void Scene_c::SetObjectRotation(uint32_t ObjectIndex, const simd::float4x4 RotationToSet) {
		prv_pImpl->SetObjectRotation(ObjectIndex, RotationToSet);
	}
/* ------------------------------------------------------------------------------------------ */
	void Scene_c::SetObjectScale(uint32_t ObjectIndex, const simd::float3 ScaleToSet) {
		prv_pImpl->SetObjectScale(ObjectIndex, ScaleToSet);
	}

/* ****************************************************************************************** */
/* Non-Member Function Implementations                                                        */
/* ****************************************************************************************** */

	std::ostream& operator<<(std::ostream &os, const Scene_c &Scene) {
		os << "Scene_c(" << *(Scene.prv_pImpl) << ")";
		return os;
	}

/* ****************************************************************************************** */
/* Private Non-Member Function Implementations                                                */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL
