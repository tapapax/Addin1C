
#ifndef Metadata_h__
#define Metadata_h__

#include "Variant.h"

#include <vector>

namespace Addin1C {

	typedef std::vector<Variant>& VariantParameters;
	
	template <class Object>
	class ClassMetadata {
		typedef Variant (Object::*ComponentFunction)(VariantParameters);
		typedef void (Object::*ComponentParameterSetter)(Variant);
		typedef Variant (Object::*ComponentParameterGetter)();
	
	private:
		struct Function {
			std::wstring englishName;
			std::wstring localName;
			long parametersCount;
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
		ClassMetadata(std::wstring name)
			:
			mName(name)
		{}
	
		void addFunction(std::wstring englishName, std::wstring localName, long parametersCount, ComponentFunction method) {
			Function func = { englishName, localName, parametersCount, method };
			mFunctions.push_back(func);
		}
		void addProperty(std::wstring englishName, std::wstring localName, ComponentParameterSetter setter, ComponentParameterGetter getter, int modes = PROP_READABLE | PROP_WRITEABLE) {
			Property prop = { englishName, localName, setter, getter, modes };
			mProperties.push_back(prop);
		}
	
		const std::wstring& name() { return mName; }
		const std::vector<Function>& functions() { return mFunctions; }
		const std::vector<Property>& properties() { return mProperties; }
	
	private:
		std::wstring mName;
	
		std::vector<Function> mFunctions;
		std::vector<Property> mProperties;
	};
}

#endif // Metadata_h__

