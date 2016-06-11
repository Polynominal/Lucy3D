#include <Settings.h>
#include <iostream>
#include "Compatability\Windows.h"
#include <utilities\Log.h>

namespace Lucy
{
    namespace Compat
    {
        namespace Windows
        {
            Version findVersion()
            {
                DWORD Version=0;
                DWORD MajorVersion = 0;
                DWORD MinorVersion = 0;

                Version = GetVersion();
                MajorVersion = (DWORD)(LOBYTE(LOWORD(Version)));
                MinorVersion = (DWORD)(HIBYTE(LOWORD(Version)));
                switch(MajorVersion)
                {
                    case 5:
                        switch(MinorVersion)
                        {
                            case 2:
                                return XPro;
                            case 1:
                                return XP;
                            case 0:
                                return TwoThousand;
                        }
                    case 6:
                        switch(MinorVersion)
                        {
                            case 3:
                                return EightPointOne;
                            case 2:
                                return Eight;
                            case 1:
                                return Seven;
                            case 0:
                                return Vista;
                        }
                    case 10:
                        return Ten;
                }
                return Err;
            };
            const char* findVersionName(Version v)
            {
                switch(v)
                {
                    case Ten:
                        return "Windows 10";
                    case Eight:
                        return "Windows 8";
                    case EightPointOne:
                        return "Windows 8.1";
                    case Seven:
                        return "Windows 7";
                    case Vista:
                        return "Windows Vista";
                    case XPro:
                        return "Windows XP professional";
                    case XP:
                        return "Windows XP";
                    case TwoThousand:
                        return "Windows 2000";
                    case Err:
                        return "UNDEFINED WINDOWS OS";
                };
            }

            //Returns the last Win32 error, in string format. Returns an empty string if there is no error.
            std::string GetError(DWORD ID)
            {
                if(ID == 0)
                {
                    return std::string("NO ERROR"); //No error message has been recorded
                }
                LPSTR messageBuffer = nullptr;
                size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                             NULL, ID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

                std::string message(messageBuffer, size);

                //Free the buffer.
                LocalFree(messageBuffer);

                return message;
            };
            std::string GetLastError()
            {
                //Get the error message, if any.
                DWORD errorMessageID = ::GetLastError();
                return GetError(errorMessageID);
            };

            void desktopComposition(bool b)
            {
                HRESULT hr = S_OK;
                if (!b)
                {
                    hr = DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
                    if (SUCCEEDED(hr))
                    {
                       // ...
                    }else
                    {
                        std::cout << "" << GetError(hr) <<  std::endl;
                    }
                }else
                {

                };
            };
        };
        void applyPatchesForWindows()
        {

            Windows::Version Ver = Windows::findVersion();
            LOG << "Info" << "Applying Compatibility Patches for: " << Windows::findVersionName(Ver) << std::endl;
            if (Ver == Windows::Seven)
            {
                #if defined USE_GLES2 || defined USE_GLES3
                LOG << "Warning" << "Disabling desktop composition for GLES2" << std::endl;
                Windows::desktopComposition(false);
                #endif
            };
        };
        void patch()
        {
            #ifdef __WIN32
            applyPatchesForWindows();
            #endif // __WIN32
        };
    }
}
