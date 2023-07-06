//1.20.1.02
#include "pch.h"

void modInit();
void modExit();

BOOL APIENTRY DllMain( const HMODULE hModule,
                       const DWORD  ulReasonForCall,
                       LPVOID lpReserved
                     )
{
	switch (ulReasonForCall)
    {
        case DLL_PROCESS_ATTACH:
 		    modInit();
		    break;
	    case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
		    break;
        case DLL_PROCESS_DETACH:
		    modExit();
            break;
	default:
        break;
    }
    return TRUE;
}

