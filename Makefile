CPU = 16f887

asm:
	gpasm -w 0 -p p$(CPU) dmxr.asm

cc:
	picc --chip=$(CPU) --opt=all dmx.c dmxr.c -Odmxr.hex

dasm:
	gpdasm -p p$(CPU) dmxr.hex

prog:
	pk2cmd -PPIC$(CPU) -Fdmxr.hex -M

pon:
	pk2cmd -PPIC$(CPU) -T

poff:
	pk2cmd -PPIC$(CPU)

clean:
	$(RM) *.as *.cod *.cof *.d *.hex *.hxl *.lnk *.lst *.obj *.p1 *.pre *.rlf *.sdb *.sym funclist
