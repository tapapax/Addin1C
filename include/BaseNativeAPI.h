
#ifndef BaseNativeAPI_h__
#define BaseNativeAPI_h__

#include <cstdint>
#include <locale>
#include <sys/types.h>
#include <cstring>

namespace Addin1C {

	namespace BaseNativeAPI {

#include "BaseNativeAPI/ComponentBase.h"
#include "BaseNativeAPI/AddInDefBase.h"
#include "BaseNativeAPI/IMemoryManager.h"

	}

inline void wstringToWCHAR(const std::wstring& source, WCHAR_T* dst) {
#ifdef _WINDOWS
	memcpy(dst, source.c_str(), (source.size() + 1) * sizeof(wchar_t));
#else
	const wchar_t* ptr = source.c_str();
	for (int i = 0; i < source.size() + 1; i++) {
		dst[i] = ptr[i];
	}
#endif
}


}

#endif // BaseNativeAPI_h__
