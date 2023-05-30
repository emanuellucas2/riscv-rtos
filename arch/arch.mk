arch:
	$(CC) $(CFLAGS) $(INC_DIRS) \
	        $(TOPDIR)arch/boot.S \
	        $(TOPDIR)arch/hal.c \
	        $(TOPDIR)arch/timer.c \
	        $(TOPDIR)arch/exception.c \
	        $(TOPDIR)arch/gpr_context.c \
	        $(TOPDIR)arch/vtimer.c \
	        $(TOPDIR)arch/syscall.c \
	        $(TOPDIR)arch/semaphore.c \
	        $(TOPDIR)arch/shared_mem.c \
	        $(TOPDIR)arch/mutex.c
