/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#include <Material.hpp>
#include <DefaultTexture.hpp>
#include <MetalKit/MetalKit.hpp>

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

	Material_s::Material_s(std::string Name, MTL::Device *pDevice) {
		prv_Name = Name;
		prv_pDevice = pDevice;
		prv_pAlbedo  = MakeAtomicTexture(255, 255, 255, 255); // white
		prv_pNormal  = MakeAtomicTexture(128, 128, 255, 255); // flat normal
		prv_pRough   = MakeAtomicTexture(128, 128, 128, 255); // mid rough
		prv_pMetal   = MakeAtomicTexture(  0,   0,   0, 255); // non-metal
		prv_pAmbOcc  = MakeAtomicTexture(255, 255, 255, 255); // fully lit
		prv_pHeight  = MakeAtomicTexture(128, 128, 128, 255); // neutral displacement
		prv_pEmiss   = MakeAtomicTexture(  0,   0,   0, 255); // no emission
	}

/* ****************************************************************************************** */
/* Member Function Implementations                                                            */
/* ****************************************************************************************** */

	std::string Material_s::GetName() {
		return prv_Name;
	}

/* ------------------------------------------------------------------------------------------ */
	void Material_s::UseMaterial(MTL::RenderCommandEncoder &pEnc) {

		pEnc.setFragmentBytes( &prv_Opacity, sizeof(float), C_BUFFER_DATA_TYPE_OPACITY);

		//albedo texture
		pEnc.setFragmentTexture(prv_pAlbedo, 0);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetDefault() {
		prv_pAlbedo = LoadTexture(DefaultTextureRawData, DefaultTextureRawData_len);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetOpacity(float Opacity) {
		prv_Opacity = Opacity;
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetAlbedo(std::string Filepath) {
		prv_pAlbedo = LoadTexture(Filepath);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetAlbedo(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		prv_pAlbedo = MakeAtomicTexture(r, g, b, a);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetNormal(std::string Filepath) {
		prv_pNormal = LoadTexture(Filepath);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetNormal(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		prv_pNormal = MakeAtomicTexture(r, g, b, a);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetRough(std::string Filepath) {
		prv_pRough = LoadTexture(Filepath);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetRough(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		prv_pRough = MakeAtomicTexture(r, g, b, a);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetMetal(std::string Filepath) {
		prv_pMetal = LoadTexture(Filepath);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetMetal(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		prv_pMetal = MakeAtomicTexture(r, g, b, a);
	}


/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetAmbOcc(std::string Filepath) {
		prv_pAmbOcc = LoadTexture(Filepath);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetAmbOcc(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		prv_pAmbOcc = MakeAtomicTexture(r, g, b, a);
	}


/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetHeight(std::string Filepath) {
		prv_pHeight = LoadTexture(Filepath);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetHeight(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		prv_pHeight = MakeAtomicTexture(r, g, b, a);
	}


/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetEmiss(std::string Filepath) {
		prv_pEmiss = LoadTexture(Filepath);
	}

/* ------------------------------------------------------------------------------------------ */

	void Material_s::SetEmiss(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		prv_pEmiss = MakeAtomicTexture(r, g, b, a);
	}


/* ------------------------------------------------------------------------------------------ */

	MTL::Texture *Material_s::LoadTexture(std::string Filepath) {
		MTL::Texture* pTexture = nullptr;
		MTK::TextureLoader* pTextureLoader = MTK::TextureLoader::alloc()->init(prv_pDevice);
		std::replace(Filepath.begin(), Filepath.end(), '\\', '/');
		NS::URL* pTextureURL = NS::URL::alloc()->initFileURLWithPath(NS::String::string(Filepath.c_str(), NS::StringEncoding::UTF8StringEncoding));
		FILE* file = std::fopen(pTextureURL->fileSystemRepresentation(), "r");
		if (file) {
			std::fclose(file);
			NS::String* pSRGBKey = NS::String::string("SRGB", NS::StringEncoding::UTF8StringEncoding);
			NS::Number* pSRGBValue = NS::Number::number(false);
			const NS::Object* values[] = { pSRGBValue };
			const NS::Object* keys[] = { pSRGBKey };
			NS::Dictionary* pOptions = NS::Dictionary::alloc()->init(values, keys, 1);

			NS::Error* pError = nullptr;
			pTexture = pTextureLoader->newTexture(pTextureURL, pOptions, &pError);
			if (!pTexture) {
				std::cout << "Failed to load texture from: " << pTextureURL->fileSystemRepresentation() << std::endl;
				if (pError) {
					std::cout << "Texture load error: " << (pError)->localizedDescription()->utf8String() << std::endl;
				}
			}
		} else {
			std::cout << "File doesn't exist: " << pTextureURL->fileSystemRepresentation() << std::endl;
		}
		pTexture->retain();
		return pTexture;
	}

/* ------------------------------------------------------------------------------------------ */

	MTL::Texture *Material_s::LoadTexture(uint8_t *pTextureBytes, uint32_t TextureBytesLen) {
		// Assume rawPointer points to your texture bytes (e.g., PNG memory), and rawSize is the size
		NS::Data* pData = NS::Data::alloc()->init(pTextureBytes, TextureBytesLen);
		MTK::TextureLoader* pTextureLoader = MTK::TextureLoader::alloc()->init(prv_pDevice);

		// If you want SRGB off for example:
		NS::String* pSRGBKey = NS::String::string("SRGB", NS::StringEncoding::UTF8StringEncoding);
		NS::Number* pSRGBValue = NS::Number::number(false);
		const NS::Object* values[] = { pSRGBValue };
		const NS::Object* keys[] = { pSRGBKey };
		NS::Dictionary* pOptions = NS::Dictionary::alloc()->init(values, keys, 1);
		NS::Error* pError = nullptr;
		MTL::Texture* pTexture = pTextureLoader->newTexture(pData, pOptions, &pError);

		if (!pTexture) {
			std::cout << "Failed to create texture from memory: " << pError->localizedDescription()->utf8String() << std::endl;
		}
		pTexture->retain();
		return pTexture;
	}

/* ------------------------------------------------------------------------------------------ */

	MTL::Texture* Material_s::MakeAtomicTexture(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		MTL::TextureDescriptor* desc = MTL::TextureDescriptor::texture2DDescriptor(MTL::PixelFormatRGBA8Unorm, 1, 1, false);
		desc->setStorageMode(MTL::StorageModeManaged);
		desc->setUsage(MTL::TextureUsageShaderRead);

		MTL::Texture* tex = prv_pDevice->newTexture(desc);

		uint8_t pixel[4] = { r, g, b, a };
		MTL::Region region = MTL::Region::Make2D(0, 0, 1, 1);
		tex->replaceRegion(region, 0, pixel, 4);

		return tex;
	}

/* ****************************************************************************************** */
/* Non-Member Function Implementations                                                        */
/* ****************************************************************************************** */

	std::ostream& operator<<(std::ostream &os, const Material_s &Material) {
		os << "Material_s(" << Material.prv_Name;
		os << ", Albedo Texture: ";
		if(Material.prv_pAlbedo == nullptr) {
			os << "Empty";
		} else {
			os << "(" << Material.prv_pAlbedo->width() << ", " << Material.prv_pAlbedo->height() << ")";
		}
		os << ", Normal Texture: " << ((Material.prv_pNormal == nullptr)?"Not Set":"True");
		os << ", Rough Texture: "  << ((Material.prv_pRough  == nullptr)?"Not Set":"True");
		os << ", Metal Texture: "  << ((Material.prv_pMetal  == nullptr)?"Not Set":"True");
		os << ", AmbOcc Texture: " << ((Material.prv_pAmbOcc == nullptr)?"Not Set":"True");
		os << ", Height Texture: " << ((Material.prv_pHeight == nullptr)?"Not Set":"True");
		os << ", Emiss Texture: "  << ((Material.prv_pEmiss  == nullptr)?"Not Set":"True");
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
