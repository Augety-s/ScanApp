#pragma once
#include "CareRayAPI.h"
#include "CrStructure.h"
#include <rpc/msgpack.hpp>
struct RegisterPara
{
    int detectorIndex;
    int appModeKey;
    int modeId;
    float frameRate;
    float exposureTime;
    int triggerType;
    int gainLevel;
    int rowIndex;
    int columnIndex;
    MSGPACK_DEFINE(
        detectorIndex,
        appModeKey,
        modeId,
        frameRate,
        exposureTime,
        triggerType,
        gainLevel,
        rowIndex,
        columnIndex
    );
};

//查询状态
struct RpcExpProgress
{
    int exposure_status;
    int is_fetchable;
    int result;
    MSGPACK_DEFINE(
        exposure_status,
        is_fetchable,
        result
    );
};

struct RpcEvent
{
    int detr_index;
    int event_id;

    int width;
    int height;
    int pixel_depth;
    int header_len;

    uint32_t data_len;
    std::vector<uint8_t> data;

    MSGPACK_DEFINE(
        event_id,
        detr_index,
        width,
        height,
        pixel_depth,
        header_len,
        data_len,
        data
    );
};

//校准状态
struct RpcCalibrationInfo
{
    int total_frame_num;
    int current_frame_num;
    int current_frame_mean;

    std::string error_msg;   // 替代 char[256]

    int result;
    int exposure_status;
    int target_gray_value;

    MSGPACK_DEFINE(
        total_frame_num,
        current_frame_num,
        current_frame_mean,
        error_msg,
        result,
        exposure_status,
        target_gray_value
    );
};

struct RpcModeInfo
{
    int mode_id;
    int image_width;
    int image_height;
    int cutoff_x;
    int cutoff_y;
    int bin_x;
    int bin_y;
    float max_frame_rate;
    float max_exposure_time;
    int pixel_depth;

    std::vector<int> trigger_types;   // 原数组改成 vector
    int valid_trigger_type_num;

    std::vector<int> gain_levels;     // 原数组改成 vector
    int valid_gain_num;

    int default_trigger_type;
    int default_gain_level;
    int roi_x;
    int roi_y;
    int mode_conf;

    std::string desc;                 // char 改成 string

    MSGPACK_DEFINE(
        mode_id,
        image_width,
        image_height,
        cutoff_x,
        cutoff_y,
        bin_x,
        bin_y,
        max_frame_rate,
        max_exposure_time,
        pixel_depth,
        trigger_types,
        valid_trigger_type_num,
        gain_levels,
        valid_gain_num,
        default_trigger_type,
        default_gain_level,
        roi_x,
        roi_y,
        mode_conf,
        desc
    );
};