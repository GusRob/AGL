/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#include <Object.hpp>
#include <MetalKit/MetalKit.hpp>
#include <Foundation/NSPrivate.hpp>

namespace AGL {

#define C_DEFAULT_TEXTURE_SCALE (10)

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

	Object_s::Object_s(uint32_t ObjectBufferIndex, MTL::Device* pDevice,
						std::string Filepath,
						StaticBuffer_s<Vertex_s> *pVertexBuffer,
						StaticBuffer_s<uint32_t> *pIndexBuffer,
						std::vector<Material_s> &Materials) {
		prv_pDevice = pDevice;

		//load the passed object
		LoadObject(ObjectBufferIndex, Filepath, pVertexBuffer, pIndexBuffer, Materials);
		prv_ObjectDataBufferIndex = ObjectBufferIndex;
	}

/* ------------------------------------------------------------------------------------------ */

	Object_s::Object_s(uint32_t ObjectBufferIndex, MTL::Device* pDevice,
						std::vector<Vertex_s> &Vertices, std::vector<uint32_t> &Indices,
						StaticBuffer_s<Vertex_s> *pVertexBuffer,
						StaticBuffer_s<uint32_t> *pIndexBuffer) {
		prv_pDevice = pDevice;
		prv_ObjectDataBufferIndex = ObjectBufferIndex;

		prv_Meshes.emplace_back(pVertexBuffer->GetNumElements(), Vertices.size(),
								pIndexBuffer->GetNumElements(), Indices.size());

		for(Vertex_s Vertex : Vertices) {
			pVertexBuffer->AddElement(Vertex);
		}
		for(uint32_t Index : Indices) {
			pIndexBuffer->AddElement(Index);
		}
	}

/* ****************************************************************************************** */
/* Member Function Implementations                                                            */
/* ****************************************************************************************** */

	void Object_s::SetVisibility(bool Visible) {
		prv_Visible = Visible;
	}

/* ------------------------------------------------------------------------------------------ */

	uint32_t Object_s::GetObjectDataBufferIndex() {
		return prv_ObjectDataBufferIndex;
	}

/* ------------------------------------------------------------------------------------------ */

	std::vector<Mesh_s> &Object_s::GetMeshes() {
		return prv_Meshes;
	}

/* ------------------------------------------------------------------------------------------ */

	uint32_t Object_s::GetNumVertices() {
		return prv_NumVertices;
	}

/* ------------------------------------------------------------------------------------------ */

	uint32_t Object_s::GetNumIndices() {
		return prv_NumIndices;
	}

/* ------------------------------------------------------------------------------------------ */

	void Object_s::LoadObject(uint32_t ObjectBufferIndex, std::string Filepath,
					StaticBuffer_s<Vertex_s> *pVertexBuffer,
					StaticBuffer_s<uint32_t> *pIndexBuffer,
					std::vector<Material_s> &Materials) {

		Assimp::Importer AssetImporter;
		const aiScene* AssimData = AssetImporter.ReadFile(Filepath,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_GenSmoothNormals | //TBD
			aiProcess_ImproveCacheLocality |
			aiProcess_ValidateDataStructure);
		if (!AssimData || !AssimData->HasMeshes()) {std::cout << "Error importing " << Filepath << std::endl;}

		uint32_t MaterialIndexOffset = Materials.size();
		if (AssimData->HasMaterials()) {
			for (uint32_t i = 0; i < AssimData->mNumMaterials; ++i) {
				aiMaterial* AssimDataMaterial = AssimData->mMaterials[i];

				bool MaterialAlreadyExists = false;
				for(Material_s CurrentMaterial : Materials) {
					if(CurrentMaterial.GetName() == AssimDataMaterial->GetName().C_Str()) {
						MaterialAlreadyExists = true;
					}
				}
				if(!MaterialAlreadyExists) {
					LoadMaterial(Filepath, AssimDataMaterial, Materials);
				}
			}
		}

		uint32_t CurrentVertexOffset = pVertexBuffer->GetNumElements();
		uint32_t CurrentIndexOffset = pIndexBuffer->GetNumElements();
		for (uint32_t i = 0; i < AssimData->mNumMeshes; i++) {
			uint32_t ThisMeshVertices = 0;
			uint32_t ThisMeshIndices = 0;
			aiMesh *AssimDataMesh = AssimData->mMeshes[i];

			//Copy into buffer Vertices
			for (uint32_t j = 0; j < AssimDataMesh->mNumVertices; j++) {
				aiVector3D pos = AssimDataMesh->mVertices[j];
				aiVector3D norm = aiVector3D(0, 1, 0);
				if(AssimDataMesh->HasNormals()) {
					norm = AssimDataMesh->mNormals[j]; //it SHOULD always have textures (because of aiProcess_GenSmoothNormals) but better to be safe
				}
				aiVector3D uv = aiVector3D((pos.x*C_DEFAULT_TEXTURE_SCALE), (pos.y*C_DEFAULT_TEXTURE_SCALE), 0);
				if(AssimDataMesh->HasTextureCoords(0)) {
					uv = AssimDataMesh->mTextureCoords[0][j];
				}

				Vertex_s ThisVertex;
				ThisVertex.VertexPosition = { pos.x, pos.y, pos.z };
				ThisVertex.VertexNormal = { norm.x, norm.y, norm.z };
				ThisVertex.TexturePosition = { uv.x, uv.y };

				pVertexBuffer->AddElement(ThisVertex);
				ThisMeshVertices++;
			}

			//Copy into buffer Indices
			for (uint32_t j = 0; j < AssimDataMesh->mNumFaces; j++) {
				aiFace AssimDataFace = AssimDataMesh->mFaces[j];

				for (uint32_t k = 0; k < AssimDataFace.mNumIndices; k++) {
					pIndexBuffer->AddElement(AssimDataFace.mIndices[k]);
					ThisMeshIndices++;
				}
			}

			prv_Meshes.emplace_back(CurrentVertexOffset, ThisMeshVertices,
									CurrentIndexOffset, ThisMeshIndices);

			bool MaterialFound = false;
			for(Material_s CurrentMaterial : Materials) {
				if(CurrentMaterial.GetName() == AssimData->mMaterials[AssimDataMesh->mMaterialIndex]->GetName().C_Str()){
					prv_Meshes[prv_Meshes.size()-1].SetMaterialIndex(MaterialIndexOffset + AssimDataMesh->mMaterialIndex);
					MaterialFound = true;
					break;
				}
			}
			if(!MaterialFound) {
					prv_Meshes[prv_Meshes.size()-1].SetMaterialIndex(0);
			}

			CurrentVertexOffset += ThisMeshVertices;
			CurrentIndexOffset += ThisMeshIndices;

			prv_NumVertices += ThisMeshVertices;
			prv_NumIndices += ThisMeshIndices;
		}
	}

/* ------------------------------------------------------------------------------------------ */

	void Object_s::LoadMaterial(std::string ObjectFilepath,
								aiMaterial* AssimDataMaterial,
								std::vector<Material_s> &Materials) {
		uint32_t ThisMaterialIndex = Materials.size();
		Materials.emplace_back(AssimDataMaterial->GetName().C_Str(), prv_pDevice);

		size_t LastSlash = ObjectFilepath.find_last_of("/\\");
		std::string ObjectDirectory = (LastSlash == std::string::npos) ? "" : ObjectFilepath.substr(0, LastSlash);

		float Opacity = 1.0f;
		AssimDataMaterial->Get(AI_MATKEY_OPACITY, Opacity);
		Materials[ThisMaterialIndex].SetOpacity(Opacity);


		//albedo
		aiString AssimDataPath;
		if (AssimDataMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &AssimDataPath) == AI_SUCCESS) {
			Materials[ThisMaterialIndex].SetAlbedo(ObjectDirectory + "/" + AssimDataPath.C_Str());
		} else {
			aiColor4D Albedo = {1.0f, 1.0f, 1.0f, 1.0f};
			if (AssimDataMaterial->Get(AI_MATKEY_BASE_COLOR, Albedo) != AI_SUCCESS) {
				aiColor3D AlbedoNoOpac = {1.0f, 1.0f, 1.0f};
				AssimDataMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, AlbedoNoOpac);
				Albedo = {AlbedoNoOpac.r, AlbedoNoOpac.g, AlbedoNoOpac.b, 1.0f};
			}
			Materials[ThisMaterialIndex].SetAlbedo((uint8_t)(0xFF*Albedo.r), (uint8_t)(0xFF*Albedo.g), (uint8_t)(0xFF*Albedo.b), (uint8_t)(0xFF*Albedo.a));
		}
		//normal
		if (AssimDataMaterial->GetTexture(aiTextureType_NORMALS, 0, &AssimDataPath) == AI_SUCCESS) {
			Materials[ThisMaterialIndex].SetNormal(ObjectDirectory + "/" + AssimDataPath.C_Str());
		}
		//rough
		if (AssimDataMaterial->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &AssimDataPath) == AI_SUCCESS) {
			Materials[ThisMaterialIndex].SetRough(ObjectDirectory + "/" + AssimDataPath.C_Str());
		} else {
			float Roughness = 1.0f;
			AssimDataMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, Roughness);
			uint8_t RoughnessByte = (uint8_t)(0xFF*Roughness);
			Materials[ThisMaterialIndex].SetRough(RoughnessByte, RoughnessByte, RoughnessByte, 0xFF);
		}
		//metal
		if (AssimDataMaterial->GetTexture(aiTextureType_METALNESS, 0, &AssimDataPath) == AI_SUCCESS) {
			Materials[ThisMaterialIndex].SetMetal(ObjectDirectory + "/" + AssimDataPath.C_Str());
		} else {
			float Metallic = 1.0f;
			AssimDataMaterial->Get(AI_MATKEY_METALLIC_FACTOR, Metallic);
			uint8_t MetallicByte = (uint8_t)(0xFF*Metallic);
			Materials[ThisMaterialIndex].SetMetal(MetallicByte, MetallicByte, MetallicByte, 0xFF);
		}
		//ambocc
		if (AssimDataMaterial->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &AssimDataPath) == AI_SUCCESS) {
			Materials[ThisMaterialIndex].SetAmbOcc(ObjectDirectory + "/" + AssimDataPath.C_Str());
		}
		//height
		if (AssimDataMaterial->GetTexture(aiTextureType_HEIGHT, 0, &AssimDataPath) == AI_SUCCESS) {
			Materials[ThisMaterialIndex].SetHeight(ObjectDirectory + "/" + AssimDataPath.C_Str());
		}
		//emiss
		if (AssimDataMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &AssimDataPath) == AI_SUCCESS) {
			Materials[ThisMaterialIndex].SetEmiss(ObjectDirectory + "/" + AssimDataPath.C_Str());
		} else {
			float Emissive = 1.0f;
			AssimDataMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, Emissive);
			uint8_t EmissiveByte = (uint8_t)(0xFF*Emissive);
			Materials[ThisMaterialIndex].SetMetal(EmissiveByte, EmissiveByte, EmissiveByte, 0xFF);
		}
	}

/* ****************************************************************************************** */
/* Non-Member Function Implementations                                                        */
/* ****************************************************************************************** */

	std::ostream& operator<<(std::ostream &os, const Object_s &Object) {
		os << "Object_s(" << Object.prv_NumVertices << ", " << Object.prv_NumIndices;
		os << ", " << Object.prv_Meshes.size();
		for(uint32_t i = 0; i < Object.prv_Meshes.size(); i++) {
			os << ", " << Object.prv_Meshes[i];
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
