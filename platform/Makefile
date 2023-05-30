# CROSS Compiler 
CROSS_COMPILER = riscv32-unknown-elf-

#Input CFG file for VMs configuration
CFG_FILE = tasks-cfg/test.cfg

# Platform info
CFLAGS = -DCPU_ID="RISCv 32"
CFLAGS += -DCPU_ARCH="QEMU VIRT 32"
CFLAGS += -DCPU_FREQ=16000000

F_CLK=200000000
DEBUG_GUEST = no
RAM_BASE = ""

#Configure CROSS COMPILER
AS = $(CROSS_COMPILER)as 
LD = $(CROSS_COMPILER)ld 
OBJDUMP = $(CROSS_COMPILER)objdump
READELF = $(CROSS_COMPILER)readelf
OBJCOPY = $(CROSS_COMPILER)objcopy
SIZE = $(CROSS_COMPILER)size
CC= $(CROSS_COMPILER)gcc
STRIP = $(CROSS_COMPILER)strip

APP = rtos
APP_LIST = ""

#GCC flags
CFLAGS += -c -mabi=ilp32 -g -DCPU_SPEED=$(F_CLK) -nostdlib -fno-builtin  -fcommon

TOPDIR=../

### Include dirs ###
INC_DIRS = -I$(TOPDIR)sys/lib/include \
		   -I$(TOPDIR)arch/include \
		   -I$(TOPDIR)sys/kernel/include \
		   -I$(TOPDIR)platform/include \
		   -I$(TOPDIR)include
	

#all: config_vms lib kernel common pic32mz platform drivers $(APP) bare_apps generate_firmware
all: make_genconf lib arch kernel $(APP) apps generate_firmware

include $(TOPDIR)sys/lib/lib.mk
include $(TOPDIR)arch/arch.mk
include $(TOPDIR)sys/kernel/kernel.mk

$(APP): 
	$(LD) $(LDFLAGS) -g -Tlinker-script/qemu-virt32.ld -Map $(APP).map -N -o $(APP).elf *.o 
	$(OBJDUMP) --disassemble --reloc $(APP).elf > $(APP).lst
	$(OBJDUMP) -h $(APP).elf > $(APP).sec
	$(OBJDUMP) -s $(APP).elf > $(APP).cnt
	$(OBJCOPY) -O binary $(APP).elf $(APP).bin
	$(SIZE) $(APP).elf

PROGRAM_ELF = rtos.elf
QEMU_RISCV32 = qemu-system-riscv32

simulate: $(PROGRAM_ELF)

	$(QEMU_RISCV32) -readconfig cfg/qemu.cfg -bios firmware.bin -nographic -s -S &

apps:
	for i in $(APP_LIST) ; do \
	$(MAKE) -C ../app/platform/ APP=$$i CLEAN=$$i clean; \
	$(MAKE) -C ../app/platform/ APP=$$i CROSS_COMPILER=$(CROSS_COMPILER) F_CLK=$(F_CLK) RAM_BASE=`awk '$$1=='\"$$i\"' {print $$3}' include/vms.info` FLASH_SIZE=1024 RAM_SIZE=4096 STACK_SIZE=256 DEBUG_GUEST=$(DEBUG_GUEST) \
	;done


stop:
	killall qemu-system-riscv32 riscv32-unknown-elf-gdb | true

reload: stop all simulate
	gnome-terminal -e "riscv32-unknown-elf-gdb --command gdb_cmd.txt"

make_genconf:
	$(eval APP_LIST:=$(shell python3 genconf.py $(CFG_FILE) || (exit 1)))
	$(info ${APP_LIST})

generate_firmware:
	$(shell export BASE_ADDR=0x80000000; ../scripts/genhex.sh $(APP_LIST))

clean: make_genconf
	rm -f $(APP).bin
	rm -f $(APP).cnt
	rm -f $(APP).elf
	rm -f $(APP).lst
	rm -f $(APP).map
	rm -f $(APP).sec
	rm -f *.o
	rm -f firmware.bin
	for i in $(APP_LIST) ; do \
	$(MAKE) -C ../app/platform/ APP=$$i CLEAN=$$i clean \
	;done


#$(QEMU_RISCV32) -readconfig cfg/qemu.cfg -kernel  $(PROGRAM_ELF) -nographic &


