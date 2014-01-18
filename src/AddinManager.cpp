
#include "AddinManager.h"

#include <string>

namespace Addin1C {

BaseNativeAPI::IComponentBase* AddinManager::createObject(const platformString& name) {
	return mObjects.at(name)->create();
}

const WCHAR_T* AddinManager::getClassNames() {
	static platformString classNames;
	classNames.clear();
	classNames.reserve(mObjects.size() * 16); // optional

	for (auto object = mObjects.cbegin(); object != mObjects.cend(); object++) {
		classNames += object->first + (WCHAR_T)L'|';
	}
	if (!classNames.empty()) classNames.pop_back();

	return classNames.c_str();
}

AddinManager::~AddinManager() {
	for (auto object = mObjects.cbegin(); object != mObjects.cend(); object++) {
		delete object->second;
	}
}

bool AddinManager::ready() {
	return !mObjects.empty();
}

}
