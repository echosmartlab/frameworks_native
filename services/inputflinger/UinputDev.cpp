#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG_TAG "UniputDev"

#include <log/log.h>

#include "UinputDev.h"
#include <cutils/properties.h>
#include <ui/DisplayInfo.h>

#include <binder/Parcel.h>
#include <binder/IServiceManager.h>

#define die(str, args...) { \
	ALOGE(str); \
	return -1; \
}

using namespace android;

static int32_t suinput_write(int32_t uinput_fd,
                  uint16_t type, uint16_t code, int32_t value)
{
    if (uinput_fd < 0)
        return -1;

    struct input_event event;
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, 0); /* This should not be able to fail ever.. */
    event.type = type;
    event.code = code;
    event.value = value;

    if (write(uinput_fd, &event, sizeof(event)) != sizeof(event))
        return -1;

    return 0;
}

static int32_t suinput_write_syn(int32_t uinput_fd)
{
    if (suinput_write(uinput_fd, EV_MSC, MSC_TIMESTAMP, 0))
        return -1;
    return suinput_write(uinput_fd, EV_SYN, SYN_REPORT, 0);
}

int32_t init_touch_device(int32_t w, int32_t h)
{

    int32_t fd = -1;

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    if (fd < 0){
        die("MNG failed to open /dev/uinput");
    }

	// enable synchronization
	if (ioctl(fd, UI_SET_EVBIT, EV_SYN) < 0)
		die("error: ioctl UI_SET_EVBIT EV_SYN");

	// enable 1 button
	if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
		die("error: ioctl UI_SET_EVBIT EV_KEY");
	if (ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH) < 0)
		die("error: ioctl UI_SET_KEYBIT");


	// enable 2 main axes + pressure (absolute positioning)
	if (ioctl(fd, UI_SET_EVBIT, EV_ABS) < 0)
		die("error: ioctl UI_SET_EVBIT EV_ABS");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_X) < 0)
		die("error: ioctl UI_SETEVBIT ABS_X");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_Y) < 0)
		die("error: ioctl UI_SETEVBIT ABS_Y");

	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_SLOT) < 0)
		die("error: ioctl UI_SETEVBIT ABS_MT_SLOT");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR) < 0)
		die("error: ioctl UI_SETEVBIT ABS_MT_TOUCH_MAJOR");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MINOR) < 0)
		die("error: ioctl UI_SETEVBIT ABS_MT_TOUCH_MINOR");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_ORIENTATION) < 0)
		die("error: ioctl UI_SETEVBIT ABS_MT_ORIENTATION");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_X) < 0)
		die("error: ioctl UI_SETEVBIT ABS_MT_POSITION_X");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y) < 0)
		die("error: ioctl UI_SETEVBIT ABS_MT_POSITION_Y");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_TOOL_X) < 0)
		die("error: ioctl UI_SETEVBIT ABS_MT_TOOL_X");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_TOOL_Y) < 0)
		die("error: ioctl UI_SETEVBIT ABS_MT_TOOL_Y");

	// enable msc
	if (ioctl(fd, UI_SET_EVBIT, EV_MSC) < 0)
		die("error: ioctl UI_SET_EVBIT EV_MSC");
	if (ioctl(fd, UI_SET_MSCBIT, MSC_TIMESTAMP) < 0)
		die("error: ioctl MSC_TIMESTAMP");

	if (ioctl(fd, UI_SET_PROPBIT, INPUT_PROP_DIRECT) < 0)
		die("error: ioctl UI_SET_PROPBIT, MSC_TIMESTAMP");

        {
          struct uinput_abs_setup abs_setup;
          struct uinput_setup setup;

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_X;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = w;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 4;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            die("error: UI_ABS_SETUP ABS_X");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_Y;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = h;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 4;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            die("error: UI_ABS_SETUP ABS_Y");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_MT_SLOT;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = 9;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 0;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            die("error: UI_ABS_SETUP ABS_MT_SLOT");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_MT_TOUCH_MAJOR;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = 4095;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 0;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            die("error: UI_ABS_SETUP ABS_MT_TOUCH_MAJOR");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_MT_TOUCH_MINOR;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = 4096;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 0;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            die("error: UI_ABS_SETUP ABS_MT_TOUCH_MINOR");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_MT_ORIENTATION;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = 1;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 0;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            die("error: UI_ABS_SETUP ABS_MT_ORIENTATION");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_MT_POSITION_X;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = w;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 4;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            die("error: UI_ABS_SETUP ABS_MT_POSITION_X");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_MT_POSITION_Y;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = h;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 4;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            die("error: UI_ABS_SETUP ABS_MT_POSITION_Y");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_MT_TOOL_X;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = w;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 4;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            die("error: UI_ABS_SETUP ABS_MT_TOOL_X");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_MT_TOOL_Y;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = h;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 4;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            die("error: UI_ABS_SETUP ABS_MT_TOOL_Y");

          memset(&setup, 0, sizeof(setup));
          snprintf(setup.name, UINPUT_MAX_NAME_SIZE, "ZOOMER");
          setup.id.bustype = BUS_VIRTUAL;
          setup.id.vendor  = 0x1;
          setup.id.product = 0x1;
          setup.id.version = 2;
          setup.ff_effects_max = 0;
          if (ioctl(fd, UI_DEV_SETUP, &setup) < 0)
            die("error: UI_DEV_SETUP");

          if (ioctl(fd, UI_DEV_CREATE) < 0)
            die("error: ioctl");
        }

    ALOGI("MNG created virtual touch device ZOOMER (%dx%d)", w, h);
    return fd;
}

int32_t close_touch_device(int32_t uinput_fd)
{
    ALOGI("MNG close_touch_device /dev/uinput");
    /*
    Sleep before destroying the device because there still can be some
    unprocessed events. This is not the right way, but I am still
    looking for better ways. The question is: how to know whether there
    are any unprocessed uinput events?
   */
//    sleep(2);

    if (uinput_fd < 0)
        return 0;

    if (ioctl(uinput_fd, UI_DEV_DESTROY) == -1) {
        close(uinput_fd);
        return -1;
    }

    if (close(uinput_fd) == -1)
        return -1;

    return 0;
}

void setPointers(int32_t uinput_fd, int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    suinput_write(uinput_fd, EV_ABS, ABS_MT_SLOT, 0);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_TRACKING_ID, 100);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_POSITION_X, x1);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_POSITION_Y, y1);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_TOOL_X, x1);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_TOOL_Y, y1);
    suinput_write(uinput_fd, EV_KEY, BTN_TOUCH, 1);
    suinput_write(uinput_fd, EV_ABS, ABS_X, x1);
    suinput_write(uinput_fd, EV_ABS, ABS_Y, y1);
    suinput_write_syn(uinput_fd);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_SLOT, 1);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_TRACKING_ID, 101);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_POSITION_X, x2);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_POSITION_Y, y2);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_TOOL_X, x2);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_TOOL_Y, y2);
    suinput_write(uinput_fd, EV_ABS, ABS_X, x2);
    suinput_write(uinput_fd, EV_ABS, ABS_Y, y2);
    suinput_write_syn(uinput_fd);
}

void movePointers(int32_t uinput_fd, int32_t x2, int32_t y2) {
    suinput_write(uinput_fd, EV_ABS, ABS_MT_SLOT, 1);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_POSITION_X, x2);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_POSITION_Y, y2);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_TOOL_X, x2);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_TOOL_Y, y2);
    suinput_write(uinput_fd, EV_ABS, ABS_X, x2);
    suinput_write(uinput_fd, EV_ABS, ABS_Y, y2);
    suinput_write_syn(uinput_fd);
}

void releasePointers(int32_t uinput_fd) {
    suinput_write(uinput_fd, EV_ABS, ABS_MT_SLOT, 1);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    suinput_write_syn(uinput_fd);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_SLOT, 0);
    suinput_write(uinput_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    suinput_write(uinput_fd, EV_KEY, BTN_TOUCH, 0);
    suinput_write_syn(uinput_fd);
}

int32_t dp_getWidth() {

    int32_t w, h;
    char vendor_res[PROPERTY_VALUE_MAX];

    property_get("vendor.display-size", vendor_res, "1920x1080");
    sscanf(vendor_res, "%dx%d", &w, &h);

  return w;
}

int32_t dp_getHeight() {

    int32_t w, h;
    char vendor_res[PROPERTY_VALUE_MAX];

    property_get("vendor.display-size", vendor_res, "1920x1080");
    sscanf(vendor_res, "%dx%d", &w, &h);

  return h;
}

int32_t sf_getrotation() {

    char sf_orientation[PROPERTY_VALUE_MAX];
    int32_t sf_rotation;

    property_get("ro.surface_flinger.primary_display_orientation", sf_orientation, "ORIENTATION_0");

    if (strcmp(sf_orientation, "ORIENTATION_90") == 0) {
        sf_rotation = DISPLAY_ORIENTATION_90;
    } else if (strcmp(sf_orientation, "ORIENTATION_180") == 0) {
        sf_rotation = DISPLAY_ORIENTATION_180;
    } else if (strcmp(sf_orientation, "ORIENTATION_270") == 0) {
        sf_rotation = DISPLAY_ORIENTATION_270;
    } else {
        sf_rotation = DISPLAY_ORIENTATION_0;
    }
  return sf_rotation;
}

static String16 get_interface_name(sp<IBinder> service)
{
    if (service != nullptr) {
        Parcel data, reply;
        status_t err = service->transact(IBinder::INTERFACE_TRANSACTION, data, &reply);
        if (err == NO_ERROR) {
            return reply.readString16();
        }
    }
    return String16();
}

int callinput(int32_t key, int32_t value) {

    sp<IServiceManager> sm = defaultServiceManager();
    if (sm == nullptr) {
        ALOGE("MNG callinput Unable to get default service manager!");
        return -1;
    }
    sp<IBinder> service = sm->checkService(String16("input"));
    if (service == nullptr) {
        ALOGE("MNG callinput Service input not found!");
        return -1;
    }
    String16 ifName = get_interface_name(service);
        if (service != nullptr && ifName.size() > 0) {
            Parcel data, reply;
    
            // the interface name is first
            data.writeInterfaceToken(ifName);
            data.writeInt32(value);
            service->transact(key, data, &reply);
        } else {
            ALOGE("MNG callinput ifName not found!");
            return -1;
        }
    return 0;
}

