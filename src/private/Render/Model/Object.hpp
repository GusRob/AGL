/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#ifndef OBJECT_H
#define OBJECT_H

#include <AGLTypes.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <DynamicBuffer.hpp>
#include <StaticBuffer.hpp>
#include <vector>
#include <Metal/Metal.hpp>

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

namespace AGL {

/* ****************************************************************************************** */
/* Non-Member Variables                                                                       */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Class Declaration                                                                          */
/* ****************************************************************************************** */

	struct Object_s {

/* ****************************************************************************************** */
/* Member Variables                                                                           */
/* ****************************************************************************************** */

	private:
		MTL::Device* prv_pDevice;

		uint32_t prv_ObjectDataBufferIndex = 0;
		bool prv_Visible = true;

		uint32_t prv_NumVertices = 0;
		uint32_t prv_NumIndices = 0;

		std::vector<Mesh_s> prv_Meshes;

/* ****************************************************************************************** */
/* Member Constructor/Destructors                                                             */
/* ****************************************************************************************** */

	public:
	Object_s(uint32_t ObjectBufferIndex, MTL::Device* pDevice,
			std::string Filepath,
			StaticBuffer_s<Vertex_s> *pVertexBuffer,
			StaticBuffer_s<uint32_t> *pIndexBuffer,
			std::vector<Material_s> &Materials);

	Object_s(uint32_t ObjectBufferIndex, MTL::Device* pDevice,
			std::vector<Vertex_s> &Vertices, std::vector<uint32_t> &Indices,
			StaticBuffer_s<Vertex_s> *pVertexBuffer,
			StaticBuffer_s<uint32_t> *pIndexBuffer);

/* ****************************************************************************************** */
/* Member Function Prototypes                                                                 */
/* ****************************************************************************************** */

	public:
		void SetVisibility(bool Visible);
		uint32_t GetObjectDataBufferIndex();
		std::vector<Mesh_s> &GetMeshes();
		uint32_t GetNumVertices();
		uint32_t GetNumIndices();
	private:
		void LoadObject(uint32_t ObjectBufferIndex, std::string Filepath,
						StaticBuffer_s<Vertex_s> *pVertexBuffer,
						StaticBuffer_s<uint32_t> *pIndexBuffer,
						std::vector<Material_s> &Materials);
		void LoadMaterial(std::string ObjectFilepath,
							aiMaterial* AssimDataMaterial,
							std::vector<Material_s> &Materials);
	public:
		friend std::ostream& operator<<(std::ostream &os, const Object_s &Object);

/* ****************************************************************************************** */
/* Class Declaration End                                                                      */
/* ****************************************************************************************** */

	}; // class Object_s

/* ****************************************************************************************** */
/* Non-Member Function Prototypes                                                             */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL

#endif // define OBJECT_H
