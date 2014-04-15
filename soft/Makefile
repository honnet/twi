# Adapted from: stackoverflow.com/questions/16443790/makefile-for-nrf51sdk

CC            := arm-none-eabi-g++
OBJCOPY       := arm-none-eabi-objcopy

NRF51_SDK     := /opt/nrf51sdk
NRF51_INCLUDE := $(NRF51_SDK)/Nordic/nrf51822/Include
NRF51_SRC     := $(NRF51_SDK)/Nordic/nrf51822/Source

INCLUDEDIRS = $(NRF51_INCLUDE)
INCLUDEDIRS += $(NRF51_INCLUDE)/gcc
INCLUDEDIRS += $(NRF51_SDK)/Nordic/nrf51822/Board/nrf6310/twi_sw_master_example
INCLUDEDIRS += $(NRF51_SDK)/Nordic/nrf51822/Board/nrf6310/radio_example
INCLUDEDIRS += ./MPU9150
INCLUDEDIRS += ./I2Cdev


CPU    := cortex-m0
DEFINE  = NRF51
DEFINE += NRF51422_QFAA_C0

# For the compiler stage
CFLAGS  = -mcpu=$(CPU)
CFLAGS += -mthumb
CFLAGS += $(patsubst %,-D%, $(DEFINE))
CFLAGS += $(patsubst %,-I%, $(INCLUDEDIRS))
CFLAGS += -Wall

# For the Linker stage
LDIRS  = /usr/arm-none-eabi/lib/armv6-m
LDIRS += /usr/lib/gcc/arm-none-eabi/4.7.4/armv6-m
TDIRS  = $(NRF51_SRC)/templates/gcc/gcc_nrf51_blank_xxaa.ld
#TDIRS  = $(NRF51_SRC)/templates/gcc/gcc_nrf51_s110_xxaa.ld

LFLAGS  = -mcpu=$(CPU)
LFLAGS += -mthumb
LFLAGS += -mabi=aapcs
LFLAGS += -Wall
LFLAGS += $(patsubst %, -L%, $(LDIRS))
LFLAGS += $(patsubst %, -T%, $(TDIRS))

# Source files to compile
SRC  = main.c
SRC += $(NRF51_SRC)/templates/system_nrf51.c
SRC += $(NRF51_SRC)/nrf_delay/nrf_delay.c
SRC += $(NRF51_SRC)/simple_uart/simple_uart.c
SRC += $(NRF51_SRC)/twi_master/twi_hw_master.c
SRC += $(NRF51_SDK)/Nordic/nrf51822/Board/nrf6310/radio_example/radio_config.c
SRC += ./MPU9150/MPU9150.cpp
SRC += ./I2Cdev/I2Cdev.cpp


ASSEMBLY_SRC += $(NRF51_SRC)/templates/gcc/gcc_startup_nrf51.s

OBJ = $(ASSEMBLY_SRC:.s=.o) $(addsuffix .o,$(basename $(SRC)))

.PHONY: all build upload clean begin end gcc_version

# Default target
all: begin gcc_version build end

build: main.bin main.hex

main.out: $(OBJ)
	@echo "\n Linking compiled file. Output will be saved to: " $@
	$(CC) $(notdir $(OBJ)) -o $@ $(LFLAGS)


main.bin: main.out
	@echo "\n Making binary file. Output will be saved to: " $@
	$(OBJCOPY) -O binary $< $@

main.hex: main.out
	@echo "\n Making hex file. Output will be saved to: " $@
	$(OBJCOPY) -O ihex $< $@

upload: all
	@echo "\n Uploading file to MCU: "
	sed  's#\[\[--filename--\]\]#$(PWD)/main.bin#' segger/burn-template.seg > burn.seg
	./segger/segger.sh $(PWD)/burn.seg || echo Done.

erase-all:
	@/bin/echo -e "device nrf51822\nw4 4001e504 2\nw4 4001e50c 1\nw4 4001e514 1\nr\nexit\n" > erase-all.jlink
	JLinkExe -if swd -speed 1000 erase-all.jlink || echo Done

recover:
	@echo "!!!!!!!!!!! erase !!!!!!!!!!\n"
	make erase-all
	@echo "\n!!!!!!!!!!! reflash !!!!!!!!!!!\n"
	make upload

clean:
	rm -f *.o *.out *.hex *.seg *.map *.bin *.hex

# Eye Candy
begin:
	@echo "---------- begin ----------"

end:
	@echo "\n----------- end -----------"

gcc_version:
	@echo
	@$(CC) --version

# General Rule for compiling C source files
%.o : %.c
	@echo "\n Compiling: " $(notdir $<)
	$(CC) $(CFLAGS) -c $< -o $(notdir $@)

%.o : %.cpp
	@echo "\n Compiling: " $(notdir $<)
	$(CC) $(CFLAGS) -c $< -o $(notdir $@)

# General Rule for compiling assembly source files
%.o : %.s
	@echo "\n Compiling: " $(notdir $<)
	$(CC) $(CFLAGS) -c $< -o $(notdir $@)

