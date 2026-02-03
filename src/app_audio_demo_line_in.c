/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#include <stdbool.h>
#include "trace.h"
#include "audio.h"
#include "audio_type.h"
#include "audio_line.h"
#include "app_audio_demo_line_in.h"

static T_AUDIO_LINE_HANDLE line_handle;

void app_audio_line_in_init(void)
{
    line_handle = audio_line_create(AUDIO_DEVICE_IN_AUX | AUDIO_DEVICE_OUT_SPK,
                                    48000,
                                    48000);
}

bool app_audio_line_in_start(void)
{
    return audio_line_start(line_handle);
}

bool app_audio_line_in_stop(void)
{
    return audio_line_stop(line_handle);
}
