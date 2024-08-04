
# Step 1: Compile the source code
make clean
make

# Step 2: Install the module and test
insmod module_1.ko my_array=1,2,3,4,5,6,7
insmod module_2.ko

lsmod | grep module

cat /proc/hello_proc

# Step 3: Uninstall the module
rmmod module_2.ko
rmmod module_1.ko

# Step 4: Review the system log information
clear
dmesg | tail -n80
