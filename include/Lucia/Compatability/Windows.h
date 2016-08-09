#ifndef MIKUS_LUCIA_COMPATABILITY_WINDOWS_H
#define MIKUS_LUCIA_COMPATABILITY_WINDOWS_H
#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <Dwmapi.h>
#include <string>
#include <Lucia\Utils\Log.h>

// dont forget to -lws2_32 and -ldwmapi
namespace Lucia
{
    namespace Compat
    {
        void patch();
        //
        void applyPatchesForWindows();
        namespace Windows
        {
            enum Version
            {
                Ten=8,
                Eight=7,
                EightPointOne=6,
                Seven=5,
                Vista=4,
                XPro=3,
                XP=2,
                TwoThousand=1,
                Err=0
            };
            std::string getLastError();
            std::string getError(DWORD ID);
            Version findVersion();
            const char* findVersionName(Version v);
            void desktopComposition(bool b);
        };
    }
}
#endif
#endif // MIKUS_COMPATABILITY_WINDOWS_H
