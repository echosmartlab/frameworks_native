LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    Client.cpp                              \
    DisplayDevice.cpp                       \
    EventThread.cpp                         \
    Layer.cpp                               \
    LayerBase.cpp                           \
    LayerDim.cpp                            \
    LayerScreenshot.cpp                     \
    DisplayHardware/FramebufferSurface.cpp  \
    DisplayHardware/GraphicBufferAlloc.cpp  \
    DisplayHardware/HWComposer.cpp          \
    DisplayHardware/PowerHAL.cpp            \
    GLExtensions.cpp                        \
    MessageQueue.cpp                        \
    SurfaceFlinger.cpp                      \
    SurfaceTextureLayer.cpp                 \
    Transform.cpp                           \
    

LOCAL_CFLAGS:= -DLOG_TAG=\"SurfaceFlinger\"
LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES

ifeq ($(TARGET_BOARD_PLATFORM),omap3)
	LOCAL_CFLAGS += -DNO_RGBX_8888
endif
ifeq ($(TARGET_BOARD_PLATFORM),omap4)
	LOCAL_CFLAGS += -DHAS_CONTEXT_PRIORITY
endif
ifeq ($(TARGET_BOARD_PLATFORM),s5pc110)
	LOCAL_CFLAGS += -DHAS_CONTEXT_PRIORITY
	LOCAL_CFLAGS += -DNEVER_DEFAULT_TO_ASYNC_MODE
endif

ifeq ($(TARGET_DISABLE_TRIPLE_BUFFERING),true)
	LOCAL_CFLAGS += -DTARGET_DISABLE_TRIPLE_BUFFERING
endif

ifneq ($(NUM_FRAMEBUFFER_SURFACE_BUFFERS),)
  LOCAL_CFLAGS += -DNUM_FRAMEBUFFER_SURFACE_BUFFERS=$(NUM_FRAMEBUFFER_SURFACE_BUFFERS)
endif

ifeq ($(BOARD_USE_BGRA_8888),true)
	LOCAL_CFLAGS += -DUSE_BGRA_8888
endif

ifeq ($(BOARD_USES_FB_PHY_LINEAR),true)
	LOCAL_CFLAGS += -DUSE_FB_PHY_LINEAR
endif

ifeq ($(BOARD_USES_NO_FENCE_SYNC),true)
	LOCAL_CFLAGS += -DNO_FENCE_SYNC
endif

ifeq ($(BOARD_USES_SYNC_MODE_FOR_MEDIA),true)
	LOCAL_CFLAGS += -DSYNC_MODE_FOR_MEDIA
endif

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libdl \
	libhardware \
	libutils \
	libEGL \
	libGLESv1_CM \
	libbinder \
	libui \
	libgui

ifeq ($(BOARD_USES_HWC_SERVICES),true)
	LOCAL_SHARED_LIBRARIES += libExynosHWCService
	LOCAL_CFLAGS += -DHWC_SERVICES
	LOCAL_C_INCLUDES += \
		$(TOP)/hardware/samsung_slsi/$(TARGET_BOARD_PLATFORM)/libhwcService \
		$(TOP)/hardware/samsung_slsi/$(TARGET_BOARD_PLATFORM)/libhwc \
		$(TOP)/hardware/samsung_slsi/$(TARGET_BOARD_PLATFORM)/include \
		$(TOP)/hardware/samsung_slsi/$(TARGET_SOC)/include \
		$(TOP)/hardware/samsung_slsi/$(TARGET_SOC)/libhwcmodule \
		$(TOP)/hardware/samsung_slsi/exynos/libexynosutils \
		$(TOP)/hardware/samsung_slsi/exynos/include
ifeq ($(BOARD_USES_PRESENTATION_SUBTITLES),true)
	LOCAL_CFLAGS += -DPRESENTATION_SUBTITLES
endif
endif

ifeq ($(BOARD_USE_GRALLOC_FLAG_FOR_HDMI),true)
	LOCAL_CFLAGS += -DUSE_GRALLOC_FLAG_FOR_HDMI
	LOCAL_C_INCLUDES += $(TOP)/hardware/samsung_slsi/$(TARGET_SOC)/include/
endif

LOCAL_MODULE:= libsurfaceflinger

include $(BUILD_SHARED_LIBRARY)

###############################################################
# uses jni which may not be available in PDK
ifneq ($(wildcard libnativehelper/include),)
include $(CLEAR_VARS)
LOCAL_CFLAGS:= -DLOG_TAG=\"SurfaceFlinger\"

LOCAL_SRC_FILES:= \
    DdmConnection.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libdl

LOCAL_MODULE:= libsurfaceflinger_ddmconnection

include $(BUILD_SHARED_LIBRARY)
endif # libnativehelper
