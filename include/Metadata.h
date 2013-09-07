
#ifndef Metadata_h__
#define Metadata_h__

#include "Variant.h"

#include <vector>

namespace Addin1C {

	const int PROP_READABLE = 1;
	const int PROP_WRITEABLE = 2;

	typedef std::basic_string<WCHAR_T> engineString;

	template <class Object>
	class ClassMetadata {
		typedef Variant (Object::*ComponentFunction)(VariantParameters&);
		typedef void (Object::*ComponentParameterSetter)(Variant);
		typedef Variant (Object::*ComponentParameterGetter)();
	
	private:
		struct Function {
			std::wstring englishName;
			std::wstring localName;
			long parametersCount;
			long optionalParametersCount;
			ComponentFunction method;
		};
		struct Property {
			std::wstring englishName;
			std::wstring localName;
			int modes;
			ComponentParameterSetter setter;
			ComponentParameterGetter getter;
		};
	
	public:	
		ClassMetadata(const std::wstring& name)
			:
			mName(name.cbegin(), name.cend())
		{}
	
		void addFunction(std::wstring englishName, std::wstring localName, long parametersCount, ComponentFunction method, long optionalParametersCount = 0) {
			Function func = { englishName, localName, parametersCount, optionalParametersCount, method };
			mFunctions.push_back(func);
		}
		void addProperty(std::wstring englishName, std::wstring localName, ComponentParameterSetter setter, ComponentParameterGetter getter, int modes = PROP_READABLE | PROP_WRITEABLE) {
			Property prop = { englishName, localName, modes, setter, getter };
			mProperties.push_back(prop);
		}
	
		const engineString& name() { return mName; }
		const std::vector<Function>& functions() { return mFunctions; }
		const std::vector<Property>& properties() { return mProperties; }
	
	private:
		engineString mName;
	
		std::vector<Function> mFunctions;
		std::vector<Property> mProperties;
	};
}

#endif // Metadata_h__

