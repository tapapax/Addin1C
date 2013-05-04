
#include "Variant.h"

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
			return std::wstring(var->pwstrVal, var->wstrLen);
		case BaseNativeAPI::VTYPE_ERROR:
		case BaseNativeAPI::VTYPE_EMPTY:
			return Undefined();
		default:
			throw std::runtime_error("<unsupported variant type>");
		}
	}

	inline void putDoubleInVariant(const double value, BaseNativeAPI::tVariant* var) {
		TV_VT(var) = BaseNativeAPI::VTYPE_R8;
		TV_R8(var) = value;
	}

	inline void putLongInVariant(const long value, BaseNativeAPI::tVariant* var) {
		TV_VT(var) = BaseNativeAPI::VTYPE_I4;
		TV_I4(var) = value;
	}

	inline void putBoolInVariant(const bool value, BaseNativeAPI::tVariant* var) {
		TV_VT(var) = BaseNativeAPI::VTYPE_BOOL;
		TV_BOOL(var) = value;
	}

	inline void putWStringInVariant(const std::wstring& str, BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) {
		wchar_t* ptr;
		auto size = (str.size() + 1) * sizeof(wchar_t);

		if (!memoryManager->AllocMemory((void**)&ptr, size)) {
			throw std::bad_alloc();
		}

		memcpy(ptr, str.c_str(), size);

		TV_VT(var) = BaseNativeAPI::VTYPE_PWSTR;
		TV_WSTR(var) = ptr;
		var->wstrLen = str.size();
	}

	inline void putStringInVariant(const std::string& str, BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) {
		char* ptr;
		auto size = (str.size() + 1) * sizeof(char);

		if (!memoryManager->AllocMemory((void**)&ptr, size)) {
			throw std::bad_alloc();
		}

		memcpy(ptr, str.c_str(), size);

		TV_VT(var) = BaseNativeAPI::VTYPE_PSTR;
		TV_STR(var) = ptr;
		var->strLen = str.size();
	}

	inline void putBinaryInVariant(const BinaryData& blob, BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager){
		putStringInVariant(blob.getData(), var, memoryManager);
		TV_VT(var) = BaseNativeAPI::VTYPE_BLOB;
	}

	void packVariant(Variant& svar, BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager* memoryManager) {
		if (svar.type() == typeid(std::wstring))
			putWStringInVariant(svar.getValue<std::wstring>(), var, memoryManager);
		else if (svar.type() == typeid(std::string))
			putStringInVariant(svar.getValue<std::string>(), var, memoryManager);
		else if (svar.type() == typeid(double))
			putDoubleInVariant(svar.getValue<double>(), var);
		else if (svar.type() == typeid(bool))
			putBoolInVariant(svar.getValue<bool>(), var);
		else if (svar.type() == typeid(BinaryData))
			putBinaryInVariant(svar.getValue<BinaryData>(), var, memoryManager);
		else if (svar.type() == typeid(long))
			putLongInVariant(svar.getValue<long>(), var);
		else if (svar.type() == typeid(Undefined))
			var->vt = BaseNativeAPI::VTYPE_EMPTY;
		else throw std::runtime_error("<cannot cast variable>");
	}

}
