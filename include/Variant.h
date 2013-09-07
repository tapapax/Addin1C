
#ifndef SmartVariant_h__
#define SmartVariant_h__

#include <string>
#include <exception>
#include <vector>
#include <typeinfo>

#include "BaseNativeAPI.h"

#define ADD_TYPE_SUPPORT(type) \
	Variant(const type& value) : mContent(new ConcreteContent<type>(value)) {} \
	Variant& operator=(const type& value) { Variant(value).swap(*this); return *this; } \
	operator type() { return getValue<type>(); }

namespace Addin1C {

	class Variant;
	Variant extractVariant(BaseNativeAPI::tVariant* var);
	void packVariant(const Variant& svar, BaseNativeAPI::tVariant* var, BaseNativeAPI::IMemoryManager*);

	class Undefined {};

	class BinaryData {
	public:
		BinaryData(std::string data) : mData(data) {}

		const std::string& getData() const { return mData;}
	private:
		std::string mData;
	};

	class Variant {
		friend class VariantParameters;

	public:
		class BadCast : public std::exception {
			virtual const char* what() const throw() {
				return "<bad cast>";
			}
		};

		Variant() : mContent(NULL) {}
		~Variant() { delete mContent; }

		ADD_TYPE_SUPPORT(std::string)
		ADD_TYPE_SUPPORT(std::wstring)
		ADD_TYPE_SUPPORT(long)
		ADD_TYPE_SUPPORT(double)
		ADD_TYPE_SUPPORT(bool)
		ADD_TYPE_SUPPORT(BinaryData)
		ADD_TYPE_SUPPORT(Undefined)

		Variant(const Variant& sec) : mContent(sec.mContent ? sec.mContent->clone() : 0) {}

		Variant& operator=(const Variant& sec) {
			Variant(sec).swap(*this);
			return *this;
		}

		bool empty() const {
			return !mContent;
		}

		const std::type_info& type() const {
			return mContent ? mContent->type() : typeid(void);
		}

		Variant& swap(Variant& sec) {
			std::swap(mContent, sec.mContent);
			return *this;
		}

		template <class T>
		T getValue() const {
			if (typeid(T) != mContent->type()) throw BadCast();
			return (static_cast<Variant::ConcreteContent<T>*>(mContent))->mValue;
		}

	private:
		class Content {
		public:
			virtual ~Content() {}

			virtual const std::type_info& type() const = 0;
			virtual Content* clone() const = 0;
		};

		template <class T>
		class ConcreteContent: public Content {
		public:
			ConcreteContent(const T& value): mValue(value) {}

			virtual const std::type_info& type() const { return typeid(T); }

			virtual ConcreteContent* clone() const {
				return new ConcreteContent(mValue);
			}

			T mValue;
		};

		Content* mContent;
	};

	class VariantParameters {
		struct VariantWithInitialContent {
			Variant variant;
			Variant::Content* content;
		};

		std::vector<VariantWithInitialContent> mParameters;

	public:
		void pack(BaseNativeAPI::tVariant* baseParameters, BaseNativeAPI::IMemoryManager* manager) {
			for (size_t i = 0; i < mParameters.size(); i++) {
				if (mParameters[i].variant.mContent != mParameters[i].content) packVariant(mParameters[i].variant, baseParameters + i, manager);
			}
		}

		VariantParameters(BaseNativeAPI::tVariant* baseParameters, size_t count)
			:
			mParameters(count)
		{
			for (size_t i = 0; i < count; i++) {
				mParameters[i].variant = extractVariant(baseParameters + i);
				mParameters[i].content = mParameters[i].variant.mContent;
			}
		}

		Variant& operator[](size_t i) {
			return mParameters[i].variant;
		}
	};

}

#endif // SmartVariant_h__

