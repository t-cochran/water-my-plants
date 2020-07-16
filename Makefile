#
# Project makefile
#
PROJECT_NAME := main

include $(IDF_PATH)/make/project.mk

clean:
	rm -f sdkconfig sdkconfig.old
