#
# Project makefile
#
PROJECT_NAME := test_project

include $(IDF_PATH)/make/project.mk

clean:
	rm -f sdkconfig sdkconfig.old
