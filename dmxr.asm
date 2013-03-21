include p16f887.inc

	__CONFIG _DEBUG_OFF & _LVP_OFF & _FCMEN_OFF & _IESO_OFF & _BOR_OFF & _CPD_OFF & _CP_OFF & _MCLRE_OFF & _PWRTE_OFF & _WDT_OFF & _INTOSC

rvec: ORG 0000h
	goto start

W_temp EQU 70h
STATUS_temp EQU 71h

DMXaddr EQU 20h
DMXaddrOnBus EQU 21h
DMXval EQU 30h

ivec: ORG 0004h
	;Save regs
	movwf W_temp
	swapf STATUS, W
	movwf STATUS_temp

	;Check for UART receive flag
	banksel PIR1
	btfss PIR1, RCIF
	goto not_uart

	banksel RCSTA
	btfsc RCSTA, FERR
	goto frame_error
	btfsc RCSTA, OERR
	goto overrun_error
;	goto received

received:
	banksel DMXaddrOnBus
	incf DMXaddrOnBus, f
	movf DMXaddrOnBus, W
	banksel DMXaddr
	subwf DMXaddr, W
	btfss STATUS, Z
	goto otheraddr

ouraddr:
	banksel RCREG
	movf RCREG, W
	banksel DMXval
	movwf DMXval
	goto iret

otheraddr:
	banksel RCREG
	movf RCREG, W
	banksel PORTD
	bsf PORTD, 4
	goto iret

overrun_error:
	banksel PORTD
	bsf PORTD, 0
	banksel RCSTA
	bcf RCSTA, SPEN
	bsf RCSTA, SPEN
	goto iret

frame_error:
	banksel PORTD
	bsf PORTD, 1

	banksel RCREG
	movf RCREG, W
	btfss STATUS, Z
	goto rcreg_not_0

rcreg_is_0:
	banksel DMXaddrOnBus
	clrf DMXaddrOnBus
	decf DMXaddrOnBus, f
	banksel PORTD
	bsf PORTD, 2
	goto iret

rcreg_not_0:
	banksel PORTD
	bsf PORTD, 3
	goto iret

not_uart:
	banksel PORTD
	bsf PORTD, 0
;	goto iret

iret:
	banksel PORTD
	clrf PORTD

	;Restore regs
	swapf STATUS_temp, W
	movwf STATUS
	swapf W_temp, F
	swapf W_temp, W

	retfie

start:
	;EUSART setup
	banksel BAUDCTL
	bsf BAUDCTL, BRG16
	banksel TXSTA
	bsf TXSTA, BRGH
	movlw 0x03
	movwf SPBRG
	movlw 0x00
	movwf SPBRGH

	banksel RCSTA
	bsf RCSTA, SPEN

	banksel PIE1
	bsf PIE1, RCIE
	bsf INTCON, PEIE

	banksel RCSTA
	bsf RCSTA, CREN

	;I/O setup
	banksel TRISD
	clrf TRISD

	banksel PORTD
	clrf PORTD

	banksel DMXaddr
	movlw 0x03
	movwf DMXaddr
	bsf INTCON, GIE

loop:
	banksel DMXval
	btfss DMXval, 0
	goto loop

	banksel PORTD
	bsf PORTD, 5

	goto loop

END
