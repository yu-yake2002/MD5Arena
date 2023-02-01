NAME = MD5Arena
SRCS = $(shell find src/ -name "*.cpp")

ifeq ($(wildcard $(NAVY_HOME)/apps/am-kernels/Makefile),)
TARGET = ./build/MD5Arena
$(TARGET):
	g++ -I ./include $(SRCS) -o $(TARGET)

run: $(TARGET)
	$(TARGET)
else
include $(NAVY_HOME)/Makefile
endif