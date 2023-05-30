lib:
	$(CC) $(CFLAGS) $(INC_DIRS) $(TOPDIR)lib/libc.c -o $(TOPDIR)apps/$(APP)/libc.o
