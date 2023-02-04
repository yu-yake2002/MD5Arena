NAME = MD5Arena
SRCS = $(shell find src/ -name "*.c")

ifeq ($(wildcard $(NAVY_HOME)/apps/am-kernels/Makefile),)
TARGET = ./build/MD5Arena
$(TARGET):
	gcc -I ./include $(SRCS) -o $(TARGET)

run: $(TARGET)
	$(TARGET)
else
include $(NAVY_HOME)/Makefile
endif