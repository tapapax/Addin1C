
#ifndef ComponentManager_h__
#define ComponentManager_h__

#include <map>
#include <set>
#include <typeinfo>

#include "BaseNativeAPI.h"
#include "Metadata.h"

namespace Addin1C {

class AbstractObjectCreator {
public:
	virtual BaseNativeAPI::IComponentBase* create() = 0;
};

template <class Object>
class ConcreteObjectCreator : public AbstractObjectCreator {
public:
	BaseNativeAPI::IComponentBase* create() { return new Object; }
};

class AddinManager {
public:
	~AddinManager();

	static AddinManager& getSingleton(void) {
		static AddinManager singleton;
		return singleton;
	}

	template <class Object>
	void registerObject();

	const WCHAR_T* getClassNames();
	BaseNativeAPI::IComponentBase* createObject(const platformString& className);

	bool ready();

private:
	std::map<platformString, AbstractObjectCreator*> mObjects;

	AddinManager() {};
	AddinManager(const AddinManager& root);
	AddinManager& operator=(const AddinManager&);
};

template <class Object>
inline void AddinManager::registerObject() {
	static bool registered = false;

	if (registered) return;
	registered = true;

	mObjects[Object::getName()] = new ConcreteObjectCreator<Object>;
}

}

#endif // ComponentManager_h__
