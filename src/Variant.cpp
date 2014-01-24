
#include "Variant.h"

#include <stdexcept>

namespace Addin1C {

Variant extractVariant(BaseNativeAPI::tVariant* var) {
	switch (var->vt) {
	case BaseNativeAPI::VTYPE_BOOL:
		return var->bVal;
	case BaseNativeAPI::VTYPE_I2:
	case BaseNativeAPI::VTYPE_I4:
	case BaseNativeAPI::VTYPE_UI1:
		return (long)var->lVal;
	case BaseNativeAPI::VTYPE_R4:
	case BaseNativeAPI::VTYPE_R8:
		return var->dblVal;
	case BaseNativeAPI::VTYPE_PWSTR:
		return std::wstring(var->pwstrVal, var->pwstrVal + var->wstrLen);
	case BaseNativeAPI::VTYPE_ERROR:
	case BaseNativeAPI::VTYPE_EMPTY:
		return Undefined();
	default:
		throw std::runtime_error("<unsupported variant type>");
	}
}

inline void putStringInVariant(const std::string& str, BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) {
	char* ptr;
	auto size = (str.size() + 1) * sizeof(char);

	if (!memoryManager->AllocMemory((void**)&ptr, size)) {
		throw std::bad_alloc();
	}

	std::memcpy(ptr, str.c_str(), size);

	TV_VT(var) = BaseNativeAPI::VTYPE_PSTR;
	TV_STR(var) = ptr;
	var->strLen = str.size();
}

template <>
void Variant::ConcreteContent<double>::packTo1cVariant(BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) const {
	TV_VT(var) = BaseNativeAPI::VTYPE_R8;
	TV_R8(var) = mValue;
}

template <>
void Variant::ConcreteContent<long>::packTo1cVariant(BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) const {
	TV_VT(var) = BaseNativeAPI::VTYPE_I4;
	TV_I4(var) = mValue;
}

template <>
void Variant::ConcreteContent<bool>::packTo1cVariant(BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) const {
	TV_VT(var) = BaseNativeAPI::VTYPE_BOOL;
	TV_BOOL(var) = mValue;
}

template <>
void Variant::ConcreteContent<std::wstring>::packTo1cVariant(BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) const {
	WCHAR_T* ptr;
	size_t bytes = (mValue.size() + 1) * sizeof(WCHAR_T);

	if (!memoryManager->AllocMemory((void**)&ptr, bytes)) {
		throw std::bad_alloc();
	}

#ifdef _WINDOWS
	memcpy(ptr, mValue.c_str(), bytes);
#else
	const wchar_t* strPtr = str.c_str();
	for (int i = 0; i < str.size() + 1; i++) {
		ptr[i] = strPtr[i];
	}
#endif

	TV_VT(var) = BaseNativeAPI::VTYPE_PWSTR;
	TV_WSTR(var) = ptr;
	var->wstrLen = mValue.size();
}

template <>
void Variant::ConcreteContent<std::string>::packTo1cVariant(BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) const {
	putStringInVariant(mValue, var, memoryManager);
}

template <>
void Variant::ConcreteContent<BinaryData>::packTo1cVariant(BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) const {
	putStringInVariant(mValue.getData(), var, memoryManager);
	TV_VT(var) = BaseNativeAPI::VTYPE_BLOB;
}

template <>
void Variant::ConcreteContent<Undefined>::packTo1cVariant(BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) const {
	TV_VT(var) = BaseNativeAPI::VTYPE_EMPTY;
}

template <class T>
void Variant::ConcreteContent<T>::packTo1cVariant(BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) const {
	throw std::runtime_error("<cannot cast variable>");
}

void packVariant(const Variant& svar, BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) {
	svar.packTo1cVariant(var, memoryManager);
}

}
