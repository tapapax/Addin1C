
#include "Header.h"

void addinInitialization(Addin1C::AddinManager& manager) {
	manager.registerObject<Calculator>();
	manager.registerObject<Keeper>();
}