/*
 * Copyright (C) 2011 The Android Open Source Project
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
 *  @author   tellen
 *  @version  1.0        
 *  @date     2013/04/26
 *  @par function description:
 *  - 1 write property or sysfs from native to java service
 */

#ifndef ANDROID_ISYSTEMWRITESERVICE_H
#define ANDROID_ISYSTEMWRITESERVICE_H

#include <utils/Errors.h>
#include <binder/IInterface.h>
#include <utils/String8.h>
#include <utils/String16.h>

namespace android {

// ----------------------------------------------------------------------------

// must be kept in sync with interface defined in ISystemWriteService.aidl
class ISystemWriteService : public IInterface
{
public:
    DECLARE_META_INTERFACE(SystemWriteService);

	virtual bool getProperty(const String16& key, String16& value) = 0;
	virtual bool getPropertyString(const String16& key, String16& def, String16& value) = 0;
	virtual int32_t getPropertyInt(const String16& key, int32_t def) = 0;
	virtual int64_t getPropertyLong(const String16& key, int64_t def) = 0;

	virtual bool getPropertyBoolean(const String16& key, bool def) = 0;
	virtual void setProperty(const String16& key, const String16& value) = 0;

	virtual bool readSysfs(const String16& path, String16& value) = 0;
	virtual bool writeSysfs(const String16& path, String16& value) = 0;
};

// ----------------------------------------------------------------------------

}; // namespace android

#endif // ANDROID_ISYSTEMWRITESERVICE_H