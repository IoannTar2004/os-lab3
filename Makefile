MODULE_NAME := my_ss_module

KERNEL_DIR := /lib/modules/$(shell uname -r)/build
PROJ_DIR := /home/ivan_linux/Documents/projects/lab3

SRC_FILES := $(shell find $(PROJ_DIR) -type f -name "*.c")
OBJ_FILES := $(patsubst $(PROJ_DIR)/src/%.c, src/%.o, $(SRC_FILES))

obj-m += $(MODULE_NAME).o
$(MODULE_NAME)-objs := $(OBJ_FILES)

PWD := $(CURDIR)

all:
	make -C $(KERNEL_DIR) M=$(PWD) modules

module: all
	rm -rf $(shell find . -type f \( -name "*.cmd" -o -name "*.o" -o -name "Module.symvers" -o -name "modules.order" -o -name "*.mod*" \))

clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean