
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

} // namespace BaseNativeAPI

typedef std::basic_string<WCHAR_T> platformString;

inline platformString convertStringToPlatform(const std::wstring& source) {
#ifdef _WINDOWS
	return source;
#else
	return platformString(source.cbegin(), source.cend());
#endif
}

inline bool isEqualICase(const platformString& l, const WCHAR_T* r) {
#ifdef _WINDOWS
	return _wcsicmp(l.c_str(), r) == 0;
#else
	if (!r) return false;

	size_t r_size = 0;
	for (; r[r_size]; r_size++) {}

	return l.size() == r_size
		&& std::equal(l.cbegin(), l.cend(), r,
		[](std::wstring::value_type l1, WCHAR_T r1)
	{ return std::towupper(l1) == std::towupper(r1); });
#endif
}

}

#endif // BaseNativeAPI_h__
