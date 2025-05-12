/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <AGLTypes.hpp>
#include <DynamicBuffer.hpp>
#include <Metal/Metal.hpp>

namespace AGL {

/* ****************************************************************************************** */
/* Non-Member Variables                                                                       */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Class Declaration                                                                          */
/* ****************************************************************************************** */

	struct Material_s {

/* ****************************************************************************************** */
/* Member Variables                                                                           */
/* ****************************************************************************************** */

	private:
		std::string prv_Name = "";
		MTL::Device *prv_pDevice;

		float prv_Opacity = 1.0f;
		MTL::Texture* prv_pAlbedo = nullptr;
		MTL::Texture* prv_pNormal = nullptr;
		MTL::Texture* prv_pRough = nullptr;
		MTL::Texture* prv_pMetal = nullptr;
		MTL::Texture* prv_pAmbOcc = nullptr;
		MTL::Texture* prv_pHeight = nullptr;
		MTL::Texture* prv_pEmiss = nullptr;

/* ****************************************************************************************** */
/* Member Constructor/Destructors                                                             */
/* ****************************************************************************************** */

	public:
		Material_s(std::string Name, MTL::Device *pDevice);

/* ****************************************************************************************** */
/* Member Function Prototypes                                                                 */
/* ****************************************************************************************** */

	public:
		std::string GetName();
		void UseMaterial(MTL::RenderCommandEncoder &pEnc);
		uint32_t GetMaterialDataIndex();
		void SetDefault();
		void SetOpacity(float Opacity);
		void SetAlbedo(std::string Filepath);
		void SetAlbedo(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetNormal(std::string Filepath);
		void SetNormal(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetRough(std::string Filepath);
		void SetRough(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetMetal(std::string Filepath);
		void SetMetal(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetAmbOcc(std::string Filepath);
		void SetAmbOcc(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetHeight(std::string Filepath);
		void SetHeight(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		void SetEmiss(std::string Filepath);
		void SetEmiss(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	private:
		MTL::Texture *LoadTexture(std::string Filepath);
		MTL::Texture *LoadTexture(uint8_t *pTextureBytes, uint32_t TextureBytesLen);
		MTL::Texture *MakeAtomicTexture(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	public:
		friend std::ostream& operator<<(std::ostream &os, const Material_s &Material);

/* ****************************************************************************************** */
/* Class Declaration End                                                                      */
/* ****************************************************************************************** */

	}; // class Material_s

/* ****************************************************************************************** */
/* Non-Member Function Prototypes                                                             */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL

#endif // define MATERIAL_H
