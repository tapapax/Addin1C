
#include <AddinObject.h>

class Calculator : public Addin1C::AddinObject<Calculator> {
	bool property;

public:
	Calculator() : property(false) {}

	static Metadata getMetadata() {
		Metadata md(L"Calculator");
		md.addFunction(L"Multiply5", L"УмножитьНа5", 1, &multiply);
		md.addFunction(L"ReverseString", L"РазвернутьСтроку", 1, &reverseString);
		md.addProperty(L"Property", L"Свойство", &setProperty, &getProperty);
		return md;
	}

	void setProperty(Addin1C::Variant value) { property = value; }
	Addin1C::Variant getProperty() { return property; }

	Addin1C::Variant multiply(Addin1C::VariantParameters& p) {
		long num = p[0];
		return num * 5;
	}

	Addin1C::Variant reverseString(Addin1C::VariantParameters& p) {
		std::wstring str = p[0];
		return std::wstring(str.crbegin(), str.crend());
	}
};
