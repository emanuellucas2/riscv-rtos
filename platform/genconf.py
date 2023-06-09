import io, libconf, sys, os

#Intermediate Physical address of the first VM on the RAM
BASE_ADDRESS = 0x80000000

#write the header of config.h
def write_header(file,cfg_name):
	file.write('''/* THIS FILE IS AUTOMATICALLY GENERATED. DO NOT MODIFY IT. */
/* See the input xml {} */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <task.h>

#define MILISECOND ((CPU_FREQ/2)/ 1000)
#define UART_SPEED 115200
#define QUANTUM_SCHEDULER_MS 10 
#define GUEST_QUANTUM_MS 1 

'''.format(cfg_name))

#write the configuration of a task
def write_task(file,task,base_address):
	file.write('''
	{{
		app_name: "{0}",
		priority: {1},
		ram_base: {2},
		size: {3},
	}},'''.format(task['app_name'],task['priority'],hex((base_address)>>12),hex(task['size']*1024)))


def write_vms_info(file,task,base_address):
	file.write('{0}\t{1}\t{2}\n'.format(task['app_name'],task['size']*1024,hex(base_address)))

#open cfg file
address = BASE_ADDRESS



if len(sys.argv)<2:
	print('Usage:',sys.argv[0],'<config file path>\n')
else:
	cfg_file = sys.argv[1]
	with io.open(cfg_file) as f:
   		config = libconf.load(f)

	#open config.h, open vms.info and write the header
	output = open("include/config.h","w")
	output2 = open("include/vms.info","w")
	write_header(output,cfg_file)
	output2.write('VM name\tram_size\taddress_start\n')

	address = address + (config.system['size']*1024)
	
	#write tasks configuration
	output.write('static const struct tasksconf_t const VMCONF[] = {')
	for i in config.tasks:
		write_task(output,i,address)
		write_vms_info(output2,i,address)
		address = address + (i['size']*1024)
	output.write('\n};')

	#write NVMACHINES
	output.write('''

/* Virtual Machine names:''')
	first = True
	for i in config.tasks:
		output.write(' {}'.format(i['app_name']))
		if first:
			print(i['app_name'], end='')
			first = False
		else:
			print(' ',end='')
			print(i['app_name'],end='')
	print()		
	output.write('''*/
#define NVMACHINES ''')
	output.write("{}".format(len(config.tasks)))
	output.write('''

#endif''')

output.close()