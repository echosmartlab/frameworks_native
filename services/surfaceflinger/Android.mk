LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    Client.cpp \
    DisplayDevice.cpp \
    DispSync.cpp \
    EventControlThread.cpp \
    EventThread.cpp \
    FrameTracker.cpp \
    Layer.cpp \
    LayerDim.cpp \
    MessageQueue.cpp \
    SurfaceFlinger.cpp \
    SurfaceFlingerConsumer.cpp \
    SurfaceTextureLayer.cpp \
    Transform.cpp \
    DisplayHardware/FramebufferSurface.cpp \
    DisplayHardware/HWComposer.cpp \
    DisplayHardware/PowerHAL.cpp \
    DisplayHardware/VirtualDisplaySurface.cpp \
    Effects/Daltonizer.cpp \
    EventLog/EventLogTags.logtags \
    EventLog/EventLog.cpp \
    RenderEngine/Description.cpp \
    RenderEngine/Mesh.cpp \
    RenderEngine/Program.cpp \
    RenderEngine/ProgramCache.cpp \
    RenderEngine/GLExtensions.cpp \
    RenderEngine/RenderEngine.cpp \
    RenderEngine/Texture.cpp \
    RenderEngine/GLES10RenderEngine.cpp \
    RenderEngine/GLES11RenderEngine.cpp \
    RenderEngine/GLES20RenderEngine.cpp


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
endif

ifeq ($(TARGET_DISABLE_TRIPLE_BUFFERING),true)
	LOCAL_CFLAGS += -DTARGET_DISABLE_TRIPLE_BUFFERING
endif

ifeq ($(TARGET_FORCE_HWC_FOR_VIRTUAL_DISPLAYS),true)
    LOCAL_CFLAGS += -DFORCE_HWC_COPY_FOR_VIRTUAL_DISPLAYS
endif

ifneq ($(NUM_FRAMEBUFFER_SURFACE_BUFFERS),)
  LOCAL_CFLAGS += -DNUM_FRAMEBUFFER_SURFACE_BUFFERS=$(NUM_FRAMEBUFFER_SURFACE_BUFFERS)
endif

ifeq ($(TARGET_RUNNING_WITHOUT_SYNC_FRAMEWORK),true)
    LOCAL_CFLAGS += -DRUNNING_WITHOUT_SYNC_FRAMEWORK
endif

# See build/target/board/generic/BoardConfig.mk for a description of this setting.
ifneq ($(VSYNC_EVENT_PHASE_OFFSET_NS),)
    LOCAL_CFLAGS += -DVSYNC_EVENT_PHASE_OFFSET_NS=$(VSYNC_EVENT_PHASE_OFFSET_NS)
else
    LOCAL_CFLAGS += -DVSYNC_EVENT_PHASE_OFFSET_NS=0
endif

# See build/target/board/generic/BoardConfig.mk for a description of this setting.
ifneq ($(SF_VSYNC_EVENT_PHASE_OFFSET_NS),)
    LOCAL_CFLAGS += -DSF_VSYNC_EVENT_PHASE_OFFSET_NS=$(SF_VSYNC_EVENT_PHASE_OFFSET_NS)
else
    LOCAL_CFLAGS += -DSF_VSYNC_EVENT_PHASE_OFFSET_NS=0
endif

ifneq ($(PRESENT_TIME_OFFSET_FROM_VSYNC_NS),)
    LOCAL_CFLAGS += -DPRESENT_TIME_OFFSET_FROM_VSYNC_NS=$(PRESENT_TIME_OFFSET_FROM_VSYNC_NS)
else
    LOCAL_CFLAGS += -DPRESENT_TIME_OFFSET_FROM_VSYNC_NS=0
endif

LOCAL_CFLAGS += -fvisibility=hidden

ifeq ($(BOARD_USE_BGRA_8888),true)
	LOCAL_CFLAGS += -DUSE_BGRA_8888
endif

ifeq ($(BOARD_USES_FB_PHY_LINEAR),true)
	LOCAL_CFLAGS += -DUSE_FB_PHY_LINEAR
endif

ifeq ($(BOARD_USES_NO_FENCE_SYNC),true)
	LOCAL_CFLAGS += -DNO_FENCE_SYNC
endif

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	liblog \
	libdl \
	libhardware \
	libutils \
	libEGL \
	libGLESv1_CM \
	libGLESv2 \
	libbinder \
	libui \
	libgui

ifeq ($(BOARD_USES_HWC_SERVICES), true)
	LOCAL_CFLAGS += -DUSES_HWC_SERVICES
	LOCAL_SHARED_LIBRARIES += libExynosHWCService
	LOCAL_C_INCLUDES += \
		$(TOP)/hardware/samsung_slsi/$(TARGET_BOARD_PLATFORM)/libhwcService \
		$(TOP)/hardware/samsung_slsi/$(TARGET_BOARD_PLATFORM)/include \
		$(TOP)/hardware/samsung_slsi/$(TARGET_SOC)/include \
		$(TOP)/hardware/samsung_slsi/$(TARGET_SOC)/libhwcmodule \
		$(TOP)/hardware/samsung_slsi/exynos/libhwc \
		$(TOP)/hardware/samsung_slsi/exynos/include \
		$(TOP)/hardware/samsung_slsi/exynos/libexynosutils
ifeq ($(BOARD_USES_VIRTUAL_DISPLAY),true)
	LOCAL_CFLAGS += -DUSES_VIRTUAL_DISPLAY
endif
ifeq ($(BOARD_SUPPORT_DQ_Q_SEQUENCE), true)
	LOCAL_CFLAGS += -DSUPPORT_DQ_Q_SEQUENCE
endif
ifeq ($(BOARD_SUPPORT_Q_DQ_SEQUENCE), true)
	LOCAL_CFLAGS += -DSUPPORT_Q_DQ_SEQUENCE
endif
ifneq ($(BOARD_HDMI_INCAPABLE), true)
	LOCAL_CFLAGS += -DHDMI_ENABLED
endif
endif

ifeq ($(BOARD_USES_FIMC), true)
	LOCAL_CFLAGS += -DUSES_FIMC
endif

LOCAL_MODULE:= libsurfaceflinger

include $(BUILD_SHARED_LIBRARY)

###############################################################
# build surfaceflinger's executable
include $(CLEAR_VARS)

LOCAL_CFLAGS:= -DLOG_TAG=\"SurfaceFlinger\"

LOCAL_SRC_FILES:= \
	main_surfaceflinger.cpp 

LOCAL_SHARED_LIBRARIES := \
	libsurfaceflinger \
	libcutils \
	liblog \
	libbinder \
	libutils

LOCAL_MODULE:= surfaceflinger

include $(BUILD_EXECUTABLE)

###############################################################
# uses jni which may not be available in PDK
ifneq ($(wildcard libnativehelper/include),)
include $(CLEAR_VARS)
LOCAL_CFLAGS:= -DLOG_TAG=\"SurfaceFlinger\"

LOCAL_SRC_FILES:= \
    DdmConnection.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	liblog \
	libdl

LOCAL_MODULE:= libsurfaceflinger_ddmconnection

include $(BUILD_SHARED_LIBRARY)
endif # libnativehelper
