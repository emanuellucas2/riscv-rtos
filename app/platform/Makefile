
all: conf_liker lib arch tasks $(APP)

ifndef CLEAN
    ifndef CROSS_COMPILER
        $(error  CROSS_COMPILER not defined)
    endif

    ifndef F_CLK
        $(error  F_CLK not defined)
    endif

    ifndef FLASH_SIZE
        $(error  FLASH_SIZE not defined.)
    endif

    ifndef RAM_SIZE
        $(error  RAM_SIZE not defined.)
    endif

    ifndef STACK_SIZE
        $(error  STACK_SIZE not defined.)
    endif

    ifndef RAM_BASE
        $(error  RAM_BASE not defined.)
    endif

endif


TOPDIR=../

INC_DIRS += -I$(TOPDIR)/lib/include \
            -I$(TOPDIR)/arch/include \
            -I$(TOPDIR)/../include


include $(TOPDIR)/lib/lib.mk
include $(TOPDIR)/arch/arch.mk
include $(TOPDIR)apps/$(APP)/app.mk


           
#Configure CROSS COMPILER
AS = $(CROSS_COMPILER)as 
LD = $(CROSS_COMPILER)ld 
OBJDUMP = $(CROSS_COMPILER)objdump
READELF = $(CROSS_COMPILER)readelf
OBJCOPY = $(CROSS_COMPILER)objcopy
SIZE = $(CROSS_COMPILER)size
CC= $(CROSS_COMPILER)gcc
STRIP = $(CROSS_COMPILER)strip

LINKER_SCRIPT = ../linker-script/qemu-virt32.ld

ifeq ($(DEBUG_GUEST), yes)
	CFLAGS += -O0 -g
else
	CFLAGS += -O2
endif

#GCC flags
CFLAGS += -c -mabi=ilp32 -g -DCPU_SPEED=$(F_CLK) -nostdlib

$(APP): 
	$(LD) $(LDFLAGS) -T../apps/$(APP)/qemu-virt32.ld -Map ../apps/$(APP)/$(APP).map -o ../apps/$(APP)/$(APP).elf ../apps/$(APP)/*.o -N $(LIBS)
#	$(STRIP) ../../apps/$(APP)/$(APP).elf
	$(OBJDUMP) -Dz ../apps/$(APP)/$(APP).elf > ../apps/$(APP)/$(APP).lst
	$(OBJDUMP) -h ../apps/$(APP)/$(APP).elf > ../apps/$(APP)/$(APP).sec
	$(OBJDUMP) -s ../apps/$(APP)/$(APP).elf > ../apps/$(APP)/$(APP).cnt
	$(OBJCOPY) -O binary ../apps/$(APP)/$(APP).elf ../apps/$(APP)/$(APP).bin
	$(SIZE) ../apps/$(APP)/$(APP).elf

conf_liker:
	cp $(TOPDIR)linker-script/qemu-virt32.ld ../apps/$(APP)/
	sed -i -e 's/RAM_SIZE/$(RAM_SIZE)/' ../apps/$(APP)/qemu-virt32.ld
	sed -i -e 's/STACK_SIZE/$(STACK_SIZE)/' ../apps/$(APP)/qemu-virt32.ld
	sed -i -e 's/RAM_BASE/$(RAM_BASE)/' ../apps/$(APP)/qemu-virt32.ld
	
	
clean: 
	rm -f ../apps/$(APP)/*.o ../apps/$(APP)/*.sec ../apps/$(APP)/*.lst ../apps/$(APP)/*.elf 
	rm -f ../apps/$(APP)/*.cnt ../apps/$(APP)/*.bin ../apps/$(APP)/*.map ../apps/$(APP)/*~ ../apps/$(APP)/*.ld

