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

#define LOG_TAG "ISystemWriteService"
//#define LOG_NDEBUG 0

#include <utils/Log.h>
#include <stdint.h>
#include <sys/types.h>
#include <binder/Parcel.h>
#include <ISystemWriteService.h>

namespace android {

// must be kept in sync with ISystemWriteService.aidl
enum {
    GET_PROPERTY			= IBinder::FIRST_CALL_TRANSACTION,
    GET_PROPERTY_STRING		= IBinder::FIRST_CALL_TRANSACTION + 1,
    GET_PROPERTY_INT		= IBinder::FIRST_CALL_TRANSACTION + 2,
    GET_PROPERTY_LONG		= IBinder::FIRST_CALL_TRANSACTION + 3,
    GET_PROPERTY_BOOL		= IBinder::FIRST_CALL_TRANSACTION + 4,
    SET_PROPERTY			= IBinder::FIRST_CALL_TRANSACTION + 5,

	READ_SYSFS				= IBinder::FIRST_CALL_TRANSACTION + 6,
	WRITE_SYSFS				= IBinder::FIRST_CALL_TRANSACTION + 7,
};

class BpSystemWriteService : public BpInterface<ISystemWriteService>
{
public:
    BpSystemWriteService(const sp<IBinder>& impl)
        : BpInterface<ISystemWriteService>(impl)
    {
    }

	virtual bool getProperty(const String16& key, String16& value)
	{
		Parcel data, reply;
        data.writeInterfaceToken(ISystemWriteService::getInterfaceDescriptor());
        data.writeString16(key);
		ALOGV("getProperty key:%s\n", String8(key).string());
		
        if (remote()->transact(GET_PROPERTY, data, &reply) != NO_ERROR) {
            ALOGD("getProperty could not contact remote\n");
            return false;
        }
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGD("getProperty caught exception %d\n", err);
            return false;
        }
        value = reply.readString16();
        return true;
	}
	
	virtual bool getPropertyString(const String16& key, String16& def, String16& value)
	{
		Parcel data, reply;
        data.writeInterfaceToken(ISystemWriteService::getInterfaceDescriptor());
        data.writeString16(key);
		ALOGV("getPropertyString key:%s\n", String8(key).string());
		
        if (remote()->transact(GET_PROPERTY_STRING, data, &reply) != NO_ERROR) {
            ALOGD("getPropertyString could not contact remote\n");
            return false;
        }
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGD("getPropertyString caught exception %d\n", err);
            return false;
        }
        value = reply.readString16();
		if(NULL == value){
			value = def;
		}
        return true;
	}
	
	virtual int32_t getPropertyInt(const String16& key, int32_t def)
	{
		Parcel data, reply;
        data.writeInterfaceToken(ISystemWriteService::getInterfaceDescriptor());
        data.writeString16(key);
		ALOGV("getPropertyInt key:%s\n", String8(key).string());
		
        if (remote()->transact(GET_PROPERTY_INT, data, &reply) != NO_ERROR) {
            ALOGD("getPropertyInt could not contact remote\n");
            return -1;
        }
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGD("getPropertyInt caught exception %d\n", err);
            return def;
        }
        return reply.readInt32();
	}
	
	virtual int64_t getPropertyLong(const String16& key, int64_t def)
	{
		Parcel data, reply;
        data.writeInterfaceToken(ISystemWriteService::getInterfaceDescriptor());
        data.writeString16(key);
		ALOGV("getPropertyLong key:%s\n", String8(key).string());
		
        if (remote()->transact(GET_PROPERTY_LONG, data, &reply) != NO_ERROR) {
            ALOGD("getPropertyLong could not contact remote\n");
            return -1;
        }
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGD("getPropertyLong caught exception %d\n", err);
            return def;
        }
        return reply.readInt64();
	}

	virtual bool getPropertyBoolean(const String16& key, bool def)
	{
		Parcel data, reply;
        data.writeInterfaceToken(ISystemWriteService::getInterfaceDescriptor());
        data.writeString16(key);
		ALOGV("getPropertyBoolean key:%s\n", String8(key).string());
		
        if (remote()->transact(GET_PROPERTY_BOOL, data, &reply) != NO_ERROR) {
            ALOGD("getPropertyBoolean could not contact remote\n");
            return false;
        }
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGD("getPropertyBoolean caught exception %d\n", err);
            return def;
        }
        return reply.readInt32() != 0;
	}
	
	virtual void setProperty(const String16& key, const String16& value)
	{
		Parcel data, reply;
        data.writeInterfaceToken(ISystemWriteService::getInterfaceDescriptor());
        data.writeString16(key);
		data.writeString16(value);
		ALOGV("setProperty key:%s, value:%s\n", String8(key).string(), String8(value).string());
		
        if (remote()->transact(SET_PROPERTY, data, &reply) != NO_ERROR) {
            ALOGD("setProperty could not contact remote\n");
            return;
        }
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGD("setProperty caught exception %d\n", err);
            return;
        }
	}

	virtual bool readSysfs(const String16& path, String16& value)
	{
		Parcel data, reply;
        data.writeInterfaceToken(ISystemWriteService::getInterfaceDescriptor());
        data.writeString16(path);
		ALOGV("setProperty path:%s\n", String8(path).string());
		
        if (remote()->transact(READ_SYSFS, data, &reply) != NO_ERROR) {
            ALOGD("readSysfs could not contact remote\n");
            return false;
        }
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGD("readSysfs caught exception %d\n", err);
            return false;
        }
        value = reply.readString16();
        return true;
	}
	
	virtual bool writeSysfs(const String16& path, String16& value)
	{
		Parcel data, reply;
        data.writeInterfaceToken(ISystemWriteService::getInterfaceDescriptor());
        data.writeString16(path);
		data.writeString16(value);
		ALOGV("writeSysfs path:%s, value:%s\n", String8(path).string(), String8(value).string());
		
        if (remote()->transact(WRITE_SYSFS, data, &reply) != NO_ERROR) {
            ALOGD("writeSysfs could not contact remote\n");
            return false;
        }
        int32_t err = reply.readExceptionCode();
        if (err < 0) {
            ALOGD("writeSysfs caught exception %d\n", err);
            return false;
        }
        return reply.readInt32() != 0;
	}
};

IMPLEMENT_META_INTERFACE(SystemWriteService, "android.app.ISystemWriteService");

// ----------------------------------------------------------------------------

}; // namespace android
