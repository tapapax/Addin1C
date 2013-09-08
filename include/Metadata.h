
#ifndef Metadata_h__
#define Metadata_h__

#include "Variant.h"

#include <vector>

namespace Addin1C {

const int PROP_READABLE = 1;
const int PROP_WRITEABLE = 2;

template <class Object>
class ClassMetadata {
	typedef Variant (Object::*ComponentFunction)(VariantParameters&);
	typedef void (Object::*ComponentParameterSetter)(Variant);
	typedef Variant (Object::*ComponentParameterGetter)();
	
private:
	struct Function {
		platformString englishName;
		platformString localName;
		long parametersCount;
		long optionalParametersCount;
		ComponentFunction method;
	};
	struct Property {
		platformString englishName;
		platformString localName;
		int modes;
		ComponentParameterSetter setter;
		ComponentParameterGetter getter;
	};
	
public:	
	ClassMetadata(const std::wstring& name)
		:
		mName(name)
	{}
	
	void addFunction(const std::wstring& englishName, const std::wstring& localName, long parametersCount, ComponentFunction method, const long optionalParametersCount = 0) {
		Function func = { convertStringToPlatform(englishName), convertStringToPlatform(localName), parametersCount, optionalParametersCount, method };
		mFunctions.push_back(func);
	}
	void addProperty(const std::wstring& englishName, const std::wstring& localName, ComponentParameterSetter setter, ComponentParameterGetter getter, const int modes = PROP_READABLE | PROP_WRITEABLE) {
		Property prop = { convertStringToPlatform(englishName), convertStringToPlatform(localName), modes, setter, getter };
		mProperties.push_back(prop);
	}
	
	const platformString& name() { return mName; }
	const std::vector<Function>& functions() { return mFunctions; }
	const std::vector<Property>& properties() { return mProperties; }
	
private:
	platformString mName;
	
	std::vector<Function> mFunctions;
	std::vector<Property> mProperties;
};

}

#endif // Metadata_h__

