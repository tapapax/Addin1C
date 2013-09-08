
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
	virtual AbstractAddinObject* createNewInstance() = 0;
};

class AddinManager {
public:
	~AddinManager() {};

	static AddinManager& getSingleton(void) {
		static AddinManager singleton;
		return singleton;
	}

	template <class Object>
	void registerObject(Object* object);

	const WCHAR_T* getClassNames();
	AbstractAddinObject* createObject(const platformString& className);
	bool typeIsRegistered(const size_t);

private:
	std::map<platformString, AbstractAddinObject*> mObjects;
	std::set<size_t> mRegistered;

	AddinManager() {};
	AddinManager(const AddinManager& root);
	AddinManager& operator=(const AddinManager&);
};

template <class Object>
inline void AddinManager::registerObject(Object* object) {
	size_t type = typeid(object).hash_code();

	if (typeIsRegistered(type)) {
		return;
	}

	mObjects[object->metadata().name()] = object;
	mRegistered.insert(type);
}

}

#endif // ComponentManager_h__
