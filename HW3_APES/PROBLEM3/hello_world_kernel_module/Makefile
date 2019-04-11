LINUXDIR :=~/Advanced-Practical-Embedded-Software/HW2_APES/PROBLEM3/buildroot/output/build/linux-4796173fc58688055a99a1cef19a839174067220
obj-m :=hello_lkm.o
all:
	make ARCH=arm CROSS_COMPILE=arm-linux- -C $(LINUXDIR) M=$(shell pwd)
clean:
	make -C $(LINUXDIR) M=$(shell pwd) clean
