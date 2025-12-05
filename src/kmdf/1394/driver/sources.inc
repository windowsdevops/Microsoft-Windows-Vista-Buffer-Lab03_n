TARGETTYPE=DRIVER

KMDF_VERSION=1

MISCFILES=$(OBJ_PATH)\$(O)\$(TARGETNAME).inf

NTTARGETFILES=

INCLUDES=..\..\inc

C_DEFINES= $(C_DEFINES)

SOURCES= \
        ..\1394samp.c \
        ..\ioctl.c \
        ..\pnp.c \
        ..\util.c\
        ..\1394api.c \
        ..\isochapi.c \
        ..\asyncapi.c



