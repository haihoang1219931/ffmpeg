CONFIG += console c++11

SOURCES += main.cpp

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

