all: unique parity unique-sorted parity-sorted
unique: unique.c unsorted.c
	gcc -o unique unique.c unsorted.c
parity: parity.c unsorted.c
	gcc -o parity parity.c unsorted.c
unique-sorted: unique.c sorted.c
	gcc -o unique-sorted unique.c sorted.c
parity-sorted: parity.c sorted.c
	gcc -o parity-sorted parity.c sorted.c