CPU = 16f887

asm:
	gpasm -w 0 -p p$(CPU) dmxr.asm

dasm:
	gpdasm -p p$(CPU) dmxr.hex

prog:
	pk2cmd -PPIC$(CPU) -Fdmxr.hex -M

pon:
	pk2cmd -PPIC$(CPU) -T

poff:
	pk2cmd -PPIC$(CPU)
