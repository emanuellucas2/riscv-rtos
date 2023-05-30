kernel: 
	$(CC) $(CFLAGS) $(INC_DIRS) \
		 $(TOPDIR)sys/kernel/task.c \
		 $(TOPDIR)sys/kernel/scheduler.c