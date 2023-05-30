#!/bin/bash

#hypervisor base address
BASE_ADDR=${BASE_ADDR:=0x80000000}

# Determine the list of the VM's flash start addresses.
NUM_TASK=0
for i in $*; do 
    START_ADDR=$(awk '$1=='\"$i\"' {print $3}' include/vms.info)
    address_array[$NUM_TASK]=$(echo $(($START_ADDR)))
    ((NUM_TASK++))
    
done 

if [ $NUM_TASK -ne 0 ]; then
	# Determines the hypervisor padding based on the start address of the first Task.  
	AUX=$(echo $(($BASE_ADDR)))
	PADDING=$(expr ${address_array[0]} - $AUX)
	
	#increment to the first Task address
	BASE_ADDR=$(expr $AUX + $PADDING)

	# fill the Kernel bin file to the padding
	dd if=rtos.bin of=/tmp/rtos.bin bs=$PADDING conv=sync

	#padding of the Task's
	COUNT=1
	for i in $*; do 
		# Determines the Task's padding size based on the start address of the next Task, or 
		#use the size of the Task in case of the last one
		if [ $COUNT -lt $NUM_TASK ]; then 
			PADDING=$(expr ${address_array[$COUNT]} - $BASE_ADDR)
		else
			ram_size=$(awk '$1=='\"$i\"' {print $2}' include/vms.info)
			PADDING=$(echo $(($ram_size)))
		fi
    
		#padding the Task's bin to its maximum size
		dd if=../app/apps/$i/$i.bin of=/tmp/$i.bin bs=$PADDING conv=sync
    
		((COUNT++))
		BASE_ADDR=$(expr $BASE_ADDR + $PADDING)
	done

	rm -rf /tmp/tmp.bin
else
	cp rtos.bin /tmp
fi

cat /tmp/rtos.bin > /tmp/firmware.bin 


for i in $*; do 
    # concatenate all binary files to the firmware.bin file
    cat /tmp/firmware.bin  /tmp/$i.bin >> /tmp/tmp.bin 
    mv /tmp/tmp.bin /tmp/firmware.bin
done

cp /tmp/firmware.bin firmware.bin
