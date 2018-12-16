CONFIG += console c++11

SOURCES += main.cpp \
    main_read_file.cpp


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


INCLUDEPATH += /home/hainh/ffmpeg_build/include
DEPENDPATH += /home/hainh/ffmpeg_build/include

LIBS += -L/home/hainh/ffmpeg_build/lib \
    -lavformat \
    -lavcodec \
    -lavdevice \
    -lavfilter \
    -lavutil \
    -lswscale \
    -lswresample \
    -lpostproc \
    -lSDL \
    -lass -laom -lfdk-aac -lvpx -lopus -lmp3lame \
    -lvorbis -lm\
    -lm -lz -llzma \
    -lx264 -ldl -lva -lm -lX11 \
    -lx265 -lm -lrt -ldl \
    -lvdpau -lva-drm -lva-x11

