/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#ifndef DYNAMICBUFFER_H
#define DYNAMICBUFFER_H

#include <AGLTypes.hpp>
#include <Metal/Metal.hpp>

namespace AGL {

/* ****************************************************************************************** */
/* Non-Member Variables                                                                       */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Class Declaration                                                                          */
/* ****************************************************************************************** */

	template <typename T>
	struct DynamicBuffer_s {

/* ****************************************************************************************** */
/* Member Variables                                                                           */
/* ****************************************************************************************** */

	private:
		MTL::Device *prv_pDevice;
		MTL::Buffer *prv_pBuffer;
		uint32_t prv_BufferType;
		uint32_t prv_NumElements;
		uint32_t prv_MaxElements;

/* ****************************************************************************************** */
/* Member Constructor/Destructors                                                             */
/* ****************************************************************************************** */

	public:
		DynamicBuffer_s(MTL::Device *pDevice, uint32_t MaxElements, uint32_t BufferType);

/* ****************************************************************************************** */
/* Member Function Prototypes                                                                 */
/* ****************************************************************************************** */

	public:
		void UseBuffer(MTL::RenderCommandEncoder &pEnc, uint32_t Offset);
		void UpdateElement(uint32_t ElementIndex, T Element);
		uint32_t AddElement(T Element);
		T GetElement(uint32_t ElementIndex);
		uint32_t GetNumElements();
		MTL::Buffer *GetBuffer();
	public:
	template <typename U>
		friend std::ostream& operator<<(std::ostream &os, const DynamicBuffer_s<U> &BufferData);

/* ****************************************************************************************** */
/* Class Declaration End                                                                      */
/* ****************************************************************************************** */

	}; // class DynamicBuffer_s

/* ****************************************************************************************** */
/* Non-Member Function Prototypes                                                             */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

} // namespace AGL

#endif // define DYNAMICBUFFER_H
