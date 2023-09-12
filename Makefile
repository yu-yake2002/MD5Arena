NAME = MD5Arena
SRCS = $(shell find src/ -name "*.c")

ifeq ($(wildcard $(NAVY_HOME)/apps/am-kernels/Makefile),)
TARGET = ./build/MD5Arena
$(TARGET):
	mkdir ./build
	gcc -I ./include $(SRCS) -g -o $(TARGET)

run: $(TARGET)
	$(TARGET)

.PHONY: clean

clean:
	rm -rf ./build

else
include $(NAVY_HOME)/Makefile
endif