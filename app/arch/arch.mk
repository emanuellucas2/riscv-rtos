arch:
	$(CC) $(CFLAGS) $(INC_DIRS) $(TOPDIR)arch/crt0.s -o $(TOPDIR)apps/$(APP)/crt.o

