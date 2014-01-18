
#include <AddinObject.h>

class Calculator : public Addin1C::AddinObject<Calculator> {
	bool property;

public:
	Calculator() : property(false) {}

	static void getMetadata(Metadata& md) {
		md.addFunction(L"Multiply5", L"УмножитьНа5", 1, &Calculator::multiply);
		md.addFunction(L"ReverseString", L"РазвернутьСтроку", 1, &Calculator::reverseString);
		md.addProperty(L"Property", L"Свойство", &Calculator::setProperty, &Calculator::getProperty);
	}

	static std::wstring getName() {
		return L"Calculator";
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


class Keeper : public Addin1C::AddinObject<Keeper> {
	long property;

public:
	Keeper() : property(3) {}

	static void getMetadata(Metadata& md) {
		md.addProperty(L"Num", L"Число", &Keeper::setProperty, &Keeper::getProperty);
	}

	static std::wstring getName() {
		return L"Keeper";
	}

	void setProperty(Addin1C::Variant value) { property = value; }
	Addin1C::Variant getProperty() { return property; }
};
