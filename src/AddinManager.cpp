
#include "AddinManager.h"

#include <string>

namespace Addin1C {

	AbstractAddinObject* AddinManager::createObject(const engineString& name) {
		return mObjects.at(name)->clone();
	}

	const WCHAR_T* AddinManager::getClassNames() {
		static engineString classNames;
		classNames.clear();
		classNames.reserve(mObjects.size() * 16); // optional

		for (auto object = mObjects.begin(); object != mObjects.end(); object++) {
			classNames += object->first + (WCHAR_T)L'|';
		}
		if (!classNames.empty()) classNames.pop_back();

		return classNames.c_str();
	}

	bool AddinManager::typeIsRegistered(const size_t type) {
		return mRegistered.find(type) != mRegistered.end();
	}

}
