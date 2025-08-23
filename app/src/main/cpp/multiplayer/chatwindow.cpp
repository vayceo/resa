#include "main.h"
#include "gui/gui.h"
#include "chatwindow.h"
#include "keyboard.h"
#include "CSettings.h"
#include "game/game.h"
#include "net/netgame.h"
#include "vendor/bass/bass.h"
#include <dlfcn.h>
#include "java_systems/Tab.h"
#include "java_systems/HUD.h"
#include <cstdarg>
#include <cstdio>
#include <string>

extern CGUI *pGUI;
extern CNetGame *pNetGame;
extern CAMERA_AIM * pcaInternalAim;

#include "util/CJavaWrapper.h"

uint32_t g_uiHeadMoveEnabled = 1;

uint32_t g_uiBorderedText = 1;
#include "CDebugInfo.h"
#include "CLocalisation.h"
#include "game/CCustomPlateManager.h"
#include "java_systems/Inventory.h"
#include <fcntl.h>

void CChatWindow::AddChatMessage(char* szNick, uint32_t dwNickColor, char* szMessage)
{
	CHUD::AddChatMessage(szMessage);
}

void CChatWindow::AddMessage(char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	size_t length = vsnprintf(nullptr, 0, szFormat, args) + 1;
	va_end(args);

	std::string formattedString(length, '\0');
	va_start(args, szFormat);
	vsnprintf(&formattedString[0], length, szFormat, args);
	va_end(args);

	CHUD::AddChatMessage(formattedString.c_str());
}

void CChatWindow::DebugMessage(char* szFormat, ...)
{
    #ifdef NDEBUG

    #else
    va_list args;
    va_start(args, szFormat);

    size_t length = vsnprintf(nullptr, 0, szFormat, args) + 1;
    va_end(args);

    std::string formattedString(length, '\0');
    va_start(args, szFormat);
    vsnprintf(&formattedString[0], length, szFormat, args);
    va_end(args);

    std::string prefixedString = "{ffffff}" + formattedString;

    CHUD::AddChatMessage(prefixedString.c_str());
#endif
}
