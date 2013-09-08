
#ifndef _SMART_COMPONENT_BASE_H_
#define _SMART_COMPONENT_BASE_H_

#ifdef _WINDOWS
#include <WTypes.h>
#endif

#include <wctype.h>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>

#include "Metadata.h"
#include "Variant.h"
#include "AddinManager.h"
#include "BaseNativeAPI.h"

#define ADDIN1C_REGISTER(Object) static Object prot_ ## Object;

#define ADDIN1C_CALL_CONCRETE_ADDIN_MEMBER(member) ((*(ConcreteAddin*)this).*(member))

namespace Addin1C {

template <class ConcreteAddin>
class AddinObject : public AbstractAddinObject {
public:
	typedef ClassMetadata<ConcreteAddin> Metadata;

	AddinObject();
	virtual ~AddinObject() {}

	Variant getErrorDescription(VariantParameters&);
	void message(const std::wstring& msg, const long code = 0);

private:
	friend class AddinManager;

	std::wstring mLastErrorDescription;
	BaseNativeAPI::IAddInDefBase* mConnect;
	BaseNativeAPI::IMemoryManager* mMemoryManager;

	AbstractAddinObject* createNewInstance() { return new ConcreteAddin; }

	static inline Metadata& getMetadata() {
		static Metadata md = ConcreteAddin::getMetadata();
		md.addFunction(L"ErrorDescription", L"ОписаниеОшибки", 0, &ConcreteAddin::getErrorDescription);
		return md;
	}

	static inline Metadata& metadata() {
		static Metadata& md = getMetadata();
		return md;
	}

	void handleExternalException();

	virtual bool ADDIN_API Init(void*);
	virtual bool ADDIN_API setMemManager(void* mem);
	virtual long ADDIN_API GetInfo();
	virtual void ADDIN_API Done();
	virtual bool ADDIN_API RegisterExtensionAs(WCHAR_T**);
	virtual long ADDIN_API GetNProps();
	virtual long ADDIN_API FindProp(const WCHAR_T* wsPropName);
	virtual const WCHAR_T* ADDIN_API GetPropName(long lPropNum, long lPropAlias);
	virtual bool ADDIN_API GetPropVal(const long lPropNum, BaseNativeAPI::tVariant* pvarPropVal);
	virtual bool ADDIN_API SetPropVal(const long lPropNum, BaseNativeAPI::tVariant* varPropVal);
	virtual bool ADDIN_API IsPropReadable(const long lPropNum);
	virtual bool ADDIN_API IsPropWritable(const long lPropNum);
	virtual long ADDIN_API GetNMethods();
	virtual long ADDIN_API FindMethod(const WCHAR_T* wsMethodName);
	virtual const WCHAR_T* ADDIN_API GetMethodName(const long lMethodNum, const long lMethodAlias);
	virtual long ADDIN_API GetNParams(const long lMethodNum);
	virtual bool ADDIN_API GetParamDefValue(const long lMethodNum, const long lParamNum, BaseNativeAPI::tVariant *pvarParamDefValue);
	virtual bool ADDIN_API HasRetVal(const long lMethodNum);
	virtual bool ADDIN_API CallAsProc(const long lMethodNum, BaseNativeAPI::tVariant* paParams, const long lSizeArray);
	virtual bool ADDIN_API CallAsFunc(const long lMethodNum, BaseNativeAPI::tVariant* pvarRetValue, BaseNativeAPI::tVariant* paParams, const long lSizeArray);
	virtual void ADDIN_API SetLocale(const WCHAR_T* loc);
};

template <class ConcreteAddin>
void Addin1C::AddinObject<ConcreteAddin>::handleExternalException() {
	try {
		try {
			throw;
		} catch (std::wstring& errorDescription) {
			mLastErrorDescription = errorDescription;
		} catch (std::exception& e) {
			std::string what = e.what();
			mLastErrorDescription = std::wstring(what.begin(), what.end());
		} catch (...) {
			mLastErrorDescription = L"<unknown error>";
		}

		message(mLastErrorDescription, 11L);
	} catch (...) {} // to suppress bad_alloc on string operations, for example
}

template <class ConcreteAddin>
Variant AddinObject<ConcreteAddin>::getErrorDescription(VariantParameters&) {
	return mLastErrorDescription;
}

template <class ConcreteAddin>
AddinObject<ConcreteAddin>::AddinObject()
	:
	mMemoryManager(NULL),
	mConnect(NULL)
{
	AddinManager::getSingleton().registerObject((ConcreteAddin*)this);
}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::Init(void* pConnection) {
	mConnect = (BaseNativeAPI::IAddInDefBase*)pConnection;
	return mConnect != NULL;
}

template <class ConcreteAddin>
long AddinObject<ConcreteAddin>::GetInfo() {
	return 2000L;
}

template <class ConcreteAddin>
void AddinObject<ConcreteAddin>::Done() {

}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::RegisterExtensionAs(WCHAR_T** wsExtensionName) {
	const platformString& name = metadata().name();

	size_t bytes = (name.size() + 1) * sizeof(WCHAR_T);
	if (!mMemoryManager || !mMemoryManager->AllocMemory((void**)wsExtensionName, bytes))
		return false;

	memcpy(*wsExtensionName, name.c_str(), bytes);

	return true;
}

template <class ConcreteAddin>
long AddinObject<ConcreteAddin>::GetNProps() {
	return metadata().properties().size();
}

template <class ConcreteAddin>
long AddinObject<ConcreteAddin>::FindProp(const WCHAR_T* wsPropName) {
	auto& props = metadata().properties();

	for (auto i = 0U; i < props.size(); i++) {
		if (isEqualICase(props[i].englishName, wsPropName)
            || isEqualICase(props[i].localName, wsPropName)) return i;
	}

	return -1;
}

template <class ConcreteAddin>
const WCHAR_T* AddinObject<ConcreteAddin>::GetPropName(long lPropNum, long lPropAlias) {
	auto& props = metadata().properties();

	if ((unsigned long)lPropNum >= props.size()) return NULL;

	const platformString* name;
	if (lPropAlias == 0)
		name = &props[lPropNum].englishName;
	else
		name = &props[lPropNum].localName;

	if (!mMemoryManager) return NULL;

	WCHAR_T* result = NULL;
	size_t bytes = (name->size() + 1) * sizeof(WCHAR_T);
	if (!mMemoryManager->AllocMemory((void**)&result, bytes))
		return NULL;

	memcpy(result, name->c_str(), bytes);

	return result;
}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::GetPropVal(const long lPropNum, BaseNativeAPI::tVariant* pvarPropVal) {
	auto& props = metadata().properties();

	if ((unsigned long)lPropNum >= props.size()) return false;

	auto property = props[lPropNum];

	try {

		packVariant(ADDIN1C_CALL_CONCRETE_ADDIN_MEMBER(property.getter)(), pvarPropVal, mMemoryManager);

		mLastErrorDescription.clear();

	} catch (...) {
		handleExternalException();
		return false;
	}

	return true;
}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::SetPropVal(const long lPropNum, BaseNativeAPI::tVariant *varPropVal) {
	auto& props = metadata().properties();

	if ((unsigned long)lPropNum >= props.size()) return false;

	auto property = props[lPropNum];

	try {

		ADDIN1C_CALL_CONCRETE_ADDIN_MEMBER(property.setter)(extractVariant(varPropVal));

		mLastErrorDescription.clear();

	} catch (...) {
		handleExternalException();
		return false;
	}

	return true;
}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::IsPropReadable(const long lPropNum) {
	auto& props = metadata().properties();

	if ((unsigned long)lPropNum >= props.size()) return false;

	return (props[lPropNum].modes & PROP_READABLE) != 0;
}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::IsPropWritable(const long lPropNum) {
	auto& props = metadata().properties();

	if ((unsigned long)lPropNum >= props.size()) return false;

	return (props[lPropNum].modes & PROP_WRITEABLE) != 0;
}

template <class ConcreteAddin>
long AddinObject<ConcreteAddin>::GetNMethods() {
	return metadata().functions().size();
}

template <class ConcreteAddin>
long AddinObject<ConcreteAddin>::FindMethod(const WCHAR_T* wsMethodName) {
	auto& funcs = metadata().functions();

	for (auto i = 0U; i < funcs.size(); i++) {
		if (isEqualICase(funcs[i].englishName, wsMethodName) ||
			isEqualICase(funcs[i].localName, wsMethodName)) return i;
	}

	return -1;
}

template <class ConcreteAddin>
const WCHAR_T* AddinObject<ConcreteAddin>::GetMethodName(const long lMethodNum, const long lMethodAlias) {
	auto& funcs = metadata().functions();

	if ((size_t)lMethodNum >= funcs.size()) return NULL;

	const platformString* name;
	if (lMethodAlias == 0)
		name = &funcs[lMethodNum].englishName;
	else
		name = &funcs[lMethodNum].localName;

	if (!mMemoryManager) return NULL;

	WCHAR_T* result;
	size_t bytes = (name->size() + 1) * sizeof(WCHAR_T);
	if (!mMemoryManager->AllocMemory((void**)&result, bytes)) return NULL;

	memcpy(result, name->c_str(), bytes);

	return result;
}

template <class ConcreteAddin>
long AddinObject<ConcreteAddin>::GetNParams(const long lMethodNum) {
	auto& funcs = metadata().functions();

	if ((size_t)lMethodNum >= funcs.size()) return 0;

	return funcs[lMethodNum].parametersCount;
}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::GetParamDefValue(const long lMethodNum, const long lParamNum, BaseNativeAPI::tVariant *pvarParamDefValue) {
	if ((size_t)lMethodNum >= metadata().functions().size()) return false;

	if (lParamNum >= metadata().functions()[lMethodNum].parametersCount - metadata().functions()[lMethodNum].optionalParametersCount) {
		TV_VT(pvarParamDefValue) = BaseNativeAPI::VTYPE_ERROR; // extractValue will convert this value to Undefined.
	} else {
		TV_VT(pvarParamDefValue) = BaseNativeAPI::VTYPE_EMPTY;
	}

	return true;
}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::HasRetVal(const long lMethodNum) {
	return true;
}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::CallAsProc(const long lMethodNum, BaseNativeAPI::tVariant* paParams, const long lSizeArray) {
	return false;
}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::CallAsFunc(const long lMethodNum, BaseNativeAPI::tVariant* pvarRetValue, BaseNativeAPI::tVariant* paParams, const long lSizeArray) {
	auto& funcs = metadata().functions();

	if ((size_t)lMethodNum >= funcs.size()) return false;
	if (lSizeArray != funcs[lMethodNum].parametersCount) return false;

	try {

		// extract parameters
		VariantParameters smartParameters(paParams, lSizeArray);

		// call the handler function
		Variant result = ADDIN1C_CALL_CONCRETE_ADDIN_MEMBER(funcs[lMethodNum].method)(smartParameters);

		// pack back parameters (only changed)
		smartParameters.pack(paParams, mMemoryManager);

		// pack function result
		packVariant(result, pvarRetValue, mMemoryManager);

		mLastErrorDescription.clear();

	} catch (...) {
		handleExternalException();
		return false;
	}

	return true;
}

template <class ConcreteAddin>
void AddinObject<ConcreteAddin>::SetLocale(const WCHAR_T* loc) {
	#ifdef _WINDOWS
		_wsetlocale(LC_ALL, loc);
	#endif
}

template <class ConcreteAddin>
bool AddinObject<ConcreteAddin>::setMemManager(void* mem) {
	mMemoryManager = (BaseNativeAPI::IMemoryManager*)mem;
	return mMemoryManager != NULL;
}

template <class ConcreteAddin>
void AddinObject<ConcreteAddin>::message(const std::wstring& msg, const long code /*= 0*/) {
	if (!mConnect) return;

	mConnect->AddError(ADDIN_E_INFO, metadata().name().c_str(), convertStringToPlatform(msg).c_str(), code);
}

} // namespace Addin1C

#endif
