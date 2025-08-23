//
// Created on 21.01.2023.
//

#pragma once

#include <jni.h>

class CDuelsGui {
public:
    static inline jobject thiz = nullptr;

public:
    static void addMessage(unsigned short killer, unsigned short killee, int reason, int team);

    static void clearKillList();

    static void showKillsLeft(bool show, int kills, int needKills);

    static void addTop(unsigned short top1, unsigned short top2, unsigned short top3, bool show);

    static void addStatistic(unsigned short kills, unsigned short deaths, bool show);
};
