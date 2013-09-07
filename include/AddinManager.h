
#ifndef ComponentManager_h__
#define ComponentManager_h__

#include <map>
#include <set>
#include <typeinfo>

#include "BaseNativeAPI.h"
#include "Metadata.h"

namespace Addin1C {

	class AbstractAddinObject : public BaseNativeAPI::IComponentBase {
	public:
		virtual AbstractAddinObject* clone() = 0;
	};

	class AddinManager {
	public:
		~AddinManager() {};

		static AddinManager& getSingleton(void) {
			static AddinManager singleton;
			return singleton;
		}

		template <class Object>
		void registerObject(Object* object) {
			size_t type = typeid(object).hash_code();

			if (typeIsRegistered(type)) {
				return;
			}

			mObjects[object->metadata().name()] = object;
			mRegistered.insert(type);
		}

		const WCHAR_T* getClassNames();

		AbstractAddinObject* createObject(const engineString& className);

		bool typeIsRegistered(const size_t);

	private:
		std::map<engineString, AbstractAddinObject*> mObjects;
		std::set<size_t> mRegistered;

		AddinManager() {};
		AddinManager(const AddinManager& root);
		AddinManager& operator=(const AddinManager&);
	};

}

#endif // ComponentManager_h__
