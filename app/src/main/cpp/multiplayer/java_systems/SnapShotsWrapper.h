//
// Created on 01.10.2023.
//

#pragma once

#include <unordered_map>
#include "rwcore.h"
#include "game/Enums/eEntityType.h"
#include "game/Core/Vector.h"
#include "game/CCustomPlateManager.h"
#include <queue>
#include <jni.h>
#include <mutex>

class SnapShotsWrapper {
public:
    enum class Types {
        OBJECT,
        PED,
        VEHICLE,
        PLATE
    };
    struct QueueItem {
        jobject     imageview;
        int         modelId;
        Types       type;
        CVector     rot;
        CVector     offset;

        ePlateType  plateType;
        std::string number;
        std::string region;

        QueueItem(jobject imageview, int modelId, Types type, CVector rot, CVector offset) {
            this->imageview = imageview;
            this->modelId   = modelId;
            this->type      = type;
            this->rot       = rot;
            this->offset    = offset;
        };

        QueueItem(jobject imageview, Types type, ePlateType plateType) {
            this->imageview = imageview;
            this->type      = type;
            this->plateType = plateType;
        }
    };

    static inline jclass clazz;
    static inline std::queue<QueueItem> queue;
    static inline std::mutex queueMutex;

public:
    static void Process();
    static jbyteArray ConvertTexToBitMapBytes(RwTexture *tex, bool needCrop);

    static void SetToImageView(jbyteArray bytes, int width, int height, jobject imageview);
};
