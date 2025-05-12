/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#include <SceneImpl.hpp>

#include <Window.hpp>
#include <WindowImpl.hpp>
#include <AGLTypes.hpp>
#include <Math.hpp>

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

	Scene_ci::~Scene_ci() {
		/*prv_pVertexBuffer->release();
		prv_pIndexBuffer->release();
		prv_pObjectDataBuffer->release();
		prv_pMaterialDataBuffer->release();
		for ( int i = 0; i < K_MAX_FRAMES_IN_FLIGHT; ++i ) {
			prv_pCameraDataBuffer[i]->release();
		}*/
	}

/* ------------------------------------------------------------------------------------------ */

	Scene_ci::Scene_ci(const Window_c& WindowForScene) {
		prv_pDevice = WindowForScene.prv_pImpl->GetMTLDevice();
		BuildBuffers();

		prv_Materials.emplace_back("Default Material", prv_pDevice);
		prv_Materials[prv_Materials.size()-1].SetDefault();
	}

/* ****************************************************************************************** */
/* Member Function Implementations                                                            */
/* ****************************************************************************************** */

	void Scene_ci::BuildBuffers() {
		prv_pVertexBuffer   = new StaticBuffer_s<Vertex_s>      (prv_pDevice, C_VERTEX_BUFFER_LENGTH,   C_BUFFER_DATA_TYPE_VERTEX);
		prv_pIndexBuffer    = new StaticBuffer_s<uint32_t>      (prv_pDevice, C_INDEX_BUFFER_LENGTH,    C_BUFFER_DATA_TYPE_INDEX);
		prv_pObjectBuffer   = new DynamicBuffer_s<ObjectData_s>    (prv_pDevice, C_OBJECT_BUFFER_LENGTH,   C_BUFFER_DATA_TYPE_OBJECT);
		prv_pCameraBuffer   = new DynamicBuffer_s<CameraData_s>    (prv_pDevice, C_CAMERA_BUFFER_LENGTH,   C_BUFFER_DATA_TYPE_CAMERA);
	}

/* ------------------------------------------------------------------------------------------ */

	void Scene_ci::DrawScene(MTL::RenderCommandEncoder &pEnc) {
		prv_frame = (prv_frame + 1) % K_MAX_FRAMES_IN_FLIGHT;
		MTL::Buffer* pCameraDataBuffer = prv_pCameraBuffer->GetBuffer();
		CameraData_s* pCameraData = reinterpret_cast< CameraData_s *>( pCameraDataBuffer->contents() );
		pCameraData->perspectiveTransform = math::makePerspective( 45.f * M_PI / 180.f, 1.f, 0.03f, 5000.0f ) ;
		pCameraData->worldTransform = math::makeTranslate({0.f, 0.f, -100.f});
		pCameraData->worldNormalTransform = math::discardTranslation( pCameraData->worldTransform );
		pCameraDataBuffer->didModifyRange( NS::Range::Make( 0, sizeof( CameraData_s ) ) );

		pEnc.setCullMode( MTL::CullModeBack );
		pEnc.setFrontFacingWinding( MTL::Winding::WindingCounterClockwise );

		prv_pVertexBuffer->UseBuffer(pEnc);
		prv_pCameraBuffer->UseBuffer(pEnc, 0);

		for(Object_s ObjectToDraw : prv_Objects) {

			prv_pObjectBuffer->UseBuffer(pEnc, ObjectToDraw.GetObjectDataBufferIndex() * sizeof(ObjectData_s));

			for(Mesh_s MeshToDraw : ObjectToDraw.GetMeshes()) {

				prv_Materials[MeshToDraw.GetMaterialIndex()].UseMaterial(pEnc);

				pEnc.drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
											MeshToDraw.GetNumIndices(), MTL::IndexType::IndexTypeUInt32,
											prv_pIndexBuffer->GetBuffer(),
											MeshToDraw.GetIndexBufferOffset()*4,
											1,
											MeshToDraw.GetVertexBufferOffset(),
											0);
			}
		}
	}

/* ------------------------------------------------------------------------------------------ */

	uint32_t Scene_ci::AddObjectFromData(std::vector<Vertex_s> &Vertices, std::vector<uint32_t> &Indices) {
		uint32_t NewObjectIndex = prv_pObjectBuffer->AddElement(ObjectData_s());

		prv_Objects.emplace_back(NewObjectIndex, prv_pDevice,
								Vertices, Indices,
								prv_pVertexBuffer,
								prv_pIndexBuffer);

		return NewObjectIndex;
	}

/* ------------------------------------------------------------------------------------------ */

	uint32_t Scene_ci::AddObjectFromFile(const std::string &ObjFilepath) {
		uint32_t NewObjectIndex = prv_Objects.size();

		prv_Objects.emplace_back(NewObjectIndex, prv_pDevice,
								ObjFilepath,
								prv_pVertexBuffer,
								prv_pIndexBuffer,
								prv_Materials);

		prv_pObjectBuffer->AddElement(ObjectData_s());

		return NewObjectIndex;
	}

/* ------------------------------------------------------------------------------------------ */

	const simd::float3 Scene_ci::GetObjectPosition(uint32_t ObjectIndex) {
		ObjectData_s* ObjectArr = reinterpret_cast<ObjectData_s*>(prv_pObjectBuffer->GetBuffer()->contents());
		return ObjectArr[ObjectIndex].Position;
	}
/* ------------------------------------------------------------------------------------------ */
	const simd::float4x4 Scene_ci::GetObjectRotation(uint32_t ObjectIndex) {
		ObjectData_s* ObjectArr = reinterpret_cast<ObjectData_s*>(prv_pObjectBuffer->GetBuffer()->contents());
		return ObjectArr[ObjectIndex].Rotation;
	}
/* ------------------------------------------------------------------------------------------ */
	const simd::float3 Scene_ci::GetObjectScale(uint32_t ObjectIndex) {
		ObjectData_s* ObjectArr = reinterpret_cast<ObjectData_s*>(prv_pObjectBuffer->GetBuffer()->contents());
		return ObjectArr[ObjectIndex].Scale;
	}
/* ------------------------------------------------------------------------------------------ */
	void Scene_ci::SetObjectPosition(uint32_t ObjectIndex, const simd::float3 PositionToSet) {
		ObjectData_s* ObjectArr = reinterpret_cast<ObjectData_s*>(prv_pObjectBuffer->GetBuffer()->contents());
		ObjectArr[ObjectIndex].Position = PositionToSet;
		prv_pObjectBuffer->GetBuffer()->didModifyRange(NS::Range::Make(ObjectIndex * sizeof(ObjectData_s), sizeof(ObjectData_s)));
	}
/* ------------------------------------------------------------------------------------------ */
	void Scene_ci::SetObjectRotation(uint32_t ObjectIndex, const simd::float4x4 RotationToSet) {
		ObjectData_s* ObjectArr = reinterpret_cast<ObjectData_s*>(prv_pObjectBuffer->GetBuffer()->contents());
		ObjectArr[ObjectIndex].Rotation = RotationToSet;
		prv_pObjectBuffer->GetBuffer()->didModifyRange(NS::Range::Make(ObjectIndex * sizeof(ObjectData_s), sizeof(ObjectData_s)));
	}
/* ------------------------------------------------------------------------------------------ */
	void Scene_ci::SetObjectScale(uint32_t ObjectIndex, const simd::float3 ScaleToSet) {
		ObjectData_s* ObjectArr = reinterpret_cast<ObjectData_s*>(prv_pObjectBuffer->GetBuffer()->contents());
		ObjectArr[ObjectIndex].Scale = ScaleToSet;
		prv_pObjectBuffer->GetBuffer()->didModifyRange(NS::Range::Make(ObjectIndex * sizeof(ObjectData_s), sizeof(ObjectData_s)));
	}

/* ****************************************************************************************** */
/* Non-Member Function Implementations                                                        */
/* ****************************************************************************************** */

	std::ostream& operator<<(std::ostream &os, const Scene_ci &Scene) {
		os << "Scene_ci(";
		os << Scene.prv_Materials.size();
		for(uint32_t i = 0; i < Scene.prv_Materials.size(); i++) {
			os << ", " << Scene.prv_Materials[i];
		}
		os << Scene.prv_Objects.size();
		for(uint32_t i = 0; i < Scene.prv_Objects.size(); i++) {
			os << ", " << Scene.prv_Objects[i];
		}
		os << ")";
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
