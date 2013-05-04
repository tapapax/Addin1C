
#include "AddinManager.h"

#include <string>

namespace Addin1C {

	AbstractAddinObject* AddinManager::createObject(std::wstring name) {
		return mObjects.at(name)->clone();
	}

	const wchar_t* AddinManager::getClassNames() {
		static std::wstring classNames;
		classNames.clear();
		classNames.reserve(mObjects.size() * 16); // optional

		for (auto object = mObjects.begin(); object != mObjects.end(); object++) {
			classNames += object->first + L'|';
		}
		if (!classNames.empty()) classNames.pop_back();

		return classNames.c_str();
	}

	bool AddinManager::typeIsRegistered(size_t type) {
		return mRegistered.find(type) != mRegistered.end();
	}

}
