#pragma once

#include <QtCore/QString>

namespace LauncherDefine
{

    namespace ResourceType
    {
#ifdef _LOCALE_JP

        const QString IconPath(":/Launcher/image/NPLauncher_jp.ico");
        const QString ProfilePath(":/ToastMessage/image/ToastMesage/profile_jp.png");
        const QString InstallImgPath(":/install/image/Install/left_image_jp.png");

#else 

        const QString IconPath(":/Launcher/image/NPLauncher.ico");
        const QString ProfilePath(":/ToastMessage/image/ToastMesage/profile.png");
        const QString InstallImgPath(":/install/image/Install/left_image.png");

#endif // _LOCALE_JP

    }

    namespace FontType
    {
#ifdef _LOCALE_JP
        const QString fontPath(":/font/image/font/meiryo.ttc");
#else 
        const QString fontPath(":/font/image/font/MalgunGothic.ttf");
#endif // _LOCALE_JP
    }

    namespace LocaleFileType
    {
#ifdef _LOCALE_JP
        const QString localeFilePath(":/language/translation file/gransagalauncher_ja.qm");
#else 
        const QString localeFilePath(":/language/translation file/gransagalauncher_ko.qm");
#endif // _LOCALE_JP
    }


}








