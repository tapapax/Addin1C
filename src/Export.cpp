
#include "AddinManager.h"

long GetClassObject(const WCHAR_T* wsName, Addin1C::BaseNativeAPI::IComponentBase** pInterface) {
    if(*pInterface) return 0;

	try {
		*pInterface = Addin1C::AddinManager::getSingleton().createObject(std::wstring(wsName));
	} catch (...) {
		return 0;
	}

    return (long)*pInterface;
}

long DestroyObject(Addin1C::BaseNativeAPI::IComponentBase** pIntf) {
   if(!*pIntf)
      return -1;

   delete *pIntf;
   *pIntf = 0;
   return 0;
}

const WCHAR_T* GetClassNames() {
	return Addin1C::AddinManager::getSingleton().getClassNames();
}

