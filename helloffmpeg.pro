QT += qml quick core multimedia
CONFIG += console c++11

SOURCES += \
    main_read_file.cpp \
    VideoFaceDetector.cpp \
    main_encode_02.cpp \
    main_gstreamer.cpp \
    main_muxing.cpp

# OPENCV
INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/include
unix:!macx: LIBS += -L/usr/local/lib/  \
    -lopencv_objdetect \
    -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_features2d -lopencv_calib3d \
    -lopencv_video \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgcodecs \
    -lopencv_imgproc \
    -lopencv_videoio

## FFMPEG
INCLUDEPATH += /usr/include/x86_64-linux-gnu/
DEPENDPATH += /usr/include/x86_64-linux-gnu/

LIBS +=  \
    -lavformat \
    -lavcodec \
#    -lavdevice \
#    -lavfilter \
    -lavutil \
    -lswscale \
    -lswresample

# SDL2
INCLUDEPATH += /usr/include/SDL2
DEPENDPATH += /usr/include/SDL2
LIBS += \
    -lSDL2

# GSTREAMER
unix:!macx: DEPENDPATH += /usr/local/include
unix:!macx: INCLUDEPATH += /usr/include/gstreamer-1.0
#unix:!macx: INCLUDEPATH += /usr/local/include/gstreamer-1.0
unix:!macx: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gstreamer-1.0/include
unix:!macx: INCLUDEPATH += /usr/include/glib-2.0
unix:!macx: INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include

unix:!macx: LIBS += -LD:\usr\lib\x86_64-linux-gnu\
    -lglib-2.0 \
    -lgstreamer-1.0 \
    -lgstapp-1.0 \
    -lgstrtsp-1.0 \
    -lgstrtspserver-1.0 \
    -lgobject-2.0 \
    -lgstvideo-1.0

HEADERS += \
    VideoFaceDetector.h
