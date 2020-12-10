/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _UI_INPUTREADER_UINPUT_DEV_H
#define _UI_INPUTREADER_UINPUT_DEV_H


int32_t init_touch_device(int32_t w, int32_t h);
int32_t close_touch_device(int32_t uinput_fd);
void setPointers(int32_t uinput_fd, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void movePointers(int32_t uinput_fd, int32_t x2, int32_t y2);
void releasePointers(int32_t uinput_fd);
int32_t dp_getWidth();
int32_t dp_getHeight();
int32_t sf_getrotation();
int callinput(int32_t key, int32_t value);

#endif
