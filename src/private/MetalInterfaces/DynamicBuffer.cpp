/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#include <DynamicBuffer.hpp>
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

	template <typename T>
	DynamicBuffer_s<T>::DynamicBuffer_s(MTL::Device *pDevice, uint32_t MaxElements, uint32_t BufferType) {
		prv_pDevice = pDevice;
		prv_pBuffer = prv_pDevice->newBuffer( MaxElements * sizeof(T), MTL::ResourceStorageModeManaged );

		prv_NumElements = 0;
		prv_MaxElements = MaxElements;
		prv_BufferType = BufferType;
	}

/* ****************************************************************************************** */
/* Member Function Implementations                                                            */
/* ****************************************************************************************** */

	template <typename T>
	void DynamicBuffer_s<T>::UseBuffer(MTL::RenderCommandEncoder &pEnc, uint32_t Offset) {
		//bind buffer with offset 0 using prv_BufferType, then provide offset using prv_BufferType with offset
		//this prevents the need to rebind the buffer for different offsets
		pEnc.setVertexBuffer( prv_pBuffer, 0, prv_BufferType);
		pEnc.setVertexBytes( &Offset, sizeof(uint32_t), prv_BufferType + C_BUFFER_DATA_TYPE_INDEX_OFFSET);
	}

/* ------------------------------------------------------------------------------------------ */

	template <typename T>
	void DynamicBuffer_s<T>::UpdateElement(uint32_t ElementIndex, T Element) {
		reinterpret_cast<T *>(prv_pBuffer->contents())[ElementIndex] = Element;
		prv_pBuffer->didModifyRange( NS::Range::Make( ElementIndex * sizeof(T), sizeof(T) ) );
	}

/* ------------------------------------------------------------------------------------------ */

	template <typename T>
	uint32_t DynamicBuffer_s<T>::AddElement(T Element) {
		T* BufferData = reinterpret_cast<T *>(prv_pBuffer->contents());
		BufferData[prv_NumElements] = Element;
		prv_pBuffer->didModifyRange( NS::Range::Make( prv_NumElements * sizeof(T), sizeof(T) ) );
		return prv_NumElements++;
	}

/* ------------------------------------------------------------------------------------------ */

	template <typename T>
	T DynamicBuffer_s<T>::GetElement(uint32_t ElementIndex) {
		T* BufferData = reinterpret_cast<T *>(prv_pBuffer->contents());
		return BufferData[ElementIndex];
	}

/* ------------------------------------------------------------------------------------------ */

	template <typename T>
	uint32_t DynamicBuffer_s<T>::GetNumElements() {
		return prv_NumElements;
	}

/* ------------------------------------------------------------------------------------------ */

	template <typename T>
	MTL::Buffer *DynamicBuffer_s<T>::GetBuffer() {
		return prv_pBuffer;
	}

/* ****************************************************************************************** */
/* Non-Member Function Implementations                                                        */
/* ****************************************************************************************** */

	template <typename T>
	std::ostream& operator<<(std::ostream &os, const DynamicBuffer_s<T> &BufferData) {
		os << "DynamicBuffer_s(" << BufferData.prv_BufferType << ", " << BufferData.prv_NumElements << ")";
		return os;
	}
	template std::ostream& operator<<(std::ostream& os, const DynamicBuffer_s<uint32_t>& BufferData);
	template std::ostream& operator<<(std::ostream& os, const DynamicBuffer_s<Vertex_s>& BufferData);
	template std::ostream& operator<<(std::ostream& os, const DynamicBuffer_s<CameraData_s>& BufferData);
	template std::ostream& operator<<(std::ostream& os, const DynamicBuffer_s<ObjectData_s>& BufferData);

/* ****************************************************************************************** */
/* Private Non-Member Function Implementations                                                */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

template struct DynamicBuffer_s<uint32_t>;
template struct DynamicBuffer_s<Vertex_s>;
template struct DynamicBuffer_s<CameraData_s>;
template struct DynamicBuffer_s<ObjectData_s>;

} // namespace AGL
