/* ****************************************************************************************** */
/* Preprocessor/Namespace                                                                     */
/* ****************************************************************************************** */

#include <StaticBuffer.hpp>
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
	StaticBuffer_s<T>::StaticBuffer_s(MTL::Device *pDevice, uint32_t MaxElements, uint32_t BufferType) {
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
	void StaticBuffer_s<T>::UseBuffer(MTL::RenderCommandEncoder &pEnc) {
		pEnc.setVertexBuffer( prv_pBuffer, 0, prv_BufferType);
	}

/* ------------------------------------------------------------------------------------------ */

	template <typename T>
	void StaticBuffer_s<T>::UpdateElement(uint32_t ElementIndex, T Element) {
		reinterpret_cast<T *>(prv_pBuffer->contents())[ElementIndex] = Element;
		prv_pBuffer->didModifyRange( NS::Range::Make( ElementIndex * sizeof(T), sizeof(T) ) );
	}

/* ------------------------------------------------------------------------------------------ */

	template <typename T>
	uint32_t StaticBuffer_s<T>::AddElement(T Element) {
		T* BufferData = reinterpret_cast<T *>(prv_pBuffer->contents());
		BufferData[prv_NumElements] = Element;
		prv_pBuffer->didModifyRange( NS::Range::Make( prv_NumElements * sizeof(T), sizeof(T) ) );
		return prv_NumElements++;
	}

/* ------------------------------------------------------------------------------------------ */

	template <typename T>
	T StaticBuffer_s<T>::GetElement(uint32_t ElementIndex) {
		T* BufferData = reinterpret_cast<T *>(prv_pBuffer->contents());
		return BufferData[ElementIndex];
	}

/* ------------------------------------------------------------------------------------------ */

	template <typename T>
	uint32_t StaticBuffer_s<T>::GetNumElements() {
		return prv_NumElements;
	}

/* ------------------------------------------------------------------------------------------ */

	template <typename T>
	MTL::Buffer *StaticBuffer_s<T>::GetBuffer() {
		return prv_pBuffer;
	}

/* ****************************************************************************************** */
/* Non-Member Function Implementations                                                        */
/* ****************************************************************************************** */

	template <typename T>
	std::ostream& operator<<(std::ostream &os, const StaticBuffer_s<T> &BufferData) {
		os << "StaticBuffer_s(" << BufferData.prv_BufferType << ", " << BufferData.prv_NumElements << ")";
		return os;
	}
	template std::ostream& operator<<(std::ostream& os, const StaticBuffer_s<uint32_t>& BufferData);
	template std::ostream& operator<<(std::ostream& os, const StaticBuffer_s<Vertex_s>& BufferData);
	template std::ostream& operator<<(std::ostream& os, const StaticBuffer_s<CameraData_s>& BufferData);
	template std::ostream& operator<<(std::ostream& os, const StaticBuffer_s<ObjectData_s>& BufferData);

/* ****************************************************************************************** */
/* Private Non-Member Function Implementations                                                */
/* ****************************************************************************************** */

	/* None */

/* ****************************************************************************************** */
/* Preprocessor/Namespace End                                                                 */
/* ****************************************************************************************** */

template struct StaticBuffer_s<uint32_t>;
template struct StaticBuffer_s<Vertex_s>;
template struct StaticBuffer_s<CameraData_s>;
template struct StaticBuffer_s<ObjectData_s>;

} // namespace AGL
