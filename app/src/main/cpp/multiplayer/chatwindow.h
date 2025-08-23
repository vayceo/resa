#pragma once
#include <vector>
#include "vendor/raknet/SingleProducerConsumer.h"

class CChatWindow
{
public:
	static void AddChatMessage(char* szNick, uint32_t dwNickColor, char* szMessage);
	static void AddMessage(char* szFormat, ...);
    static void DebugMessage(char *szFormat, ...);
};