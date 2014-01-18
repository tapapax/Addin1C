
#include "AddinManager.h"

void addinInitialization(Addin1C::AddinManager& manager);

extern "C" long GetClassObject(const WCHAR_T* wsName, Addin1C::BaseNativeAPI::IComponentBase** pInterface) {
    if(*pInterface) return 0;

	auto& manager = Addin1C::AddinManager::getSingleton();
	if (!manager.ready()) addinInitialization(manager);

	try {
		*pInterface = manager.createObject(Addin1C::platformString(wsName));
	} catch (...) {
		return 0;
	}

    return (long)*pInterface;
}

extern "C" long DestroyObject(Addin1C::BaseNativeAPI::IComponentBase** pIntf) {
   if(!*pIntf)
      return -1;

   delete *pIntf;
   *pIntf = 0;
   return 0;
}

extern "C" const WCHAR_T* GetClassNames() {
	auto& manager = Addin1C::AddinManager::getSingleton();
	if (!manager.ready()) addinInitialization(manager);

	return manager.getClassNames();
}
