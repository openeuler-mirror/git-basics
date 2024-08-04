#
# hello.s for Kunpeng architecture
#
# as -o hello.o hello.s
# ld -o hello hello.o
#

.text
.global _start
_start:
	mov x0,#0
	ldr x1,=msg
	mov x2,len
	mov x8,64
	svc #0

	mov x0,123
	mov x8,93
	svc #0

.data
msg:
	.ascii "Hello openEuler!\n\0"
len=.-msg
