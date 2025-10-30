		AREA	|.text|, CODE, READONLY, ALIGN=2
		THUMB

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call Table
SYSTEMCALLTBL	EQU		0x20007B00
SYS_EXIT		EQU		0x0		; address 20007B00
SYS_MALLOC		EQU		0x1		; address 20007B04
SYS_FREE		EQU		0x2		; address 20007B08

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call Table Initialization
		EXPORT	_systemcall_table_init 
_systemcall_table_init
		LDR		r0, = SYSTEMCALLTBL
		
		; Initialize SYSTEMCALLTBL[0] = _sys_exit
		LDR		r1, = _sys_exit
		STR		r1, [r0]

		; Initialize_SYSTEMCALLTBL[1] = _sys_malloc
		; add your code here
		; your code may be of 2 to 6 lines
		LDR 	r1, = _sys_malloc
		STR		r1, [r0, #4]
	
		; Initialize_SYSTEMCALLTBL[2] = _sys_free
		; add your code here
		; your code may be of 2 to 6 lines
		LDR 	r1, = _sys_free
		STR		r1, [r0, #8]
		
		BX		lr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call Table Jump Routine
; this is the function that will be callsed by SVC
        EXPORT	_systemcall_table_jump
_systemcall_table_jump
		LDR		r11, = SYSTEMCALLTBL	; load the starting address of SYSTEMCALLTBL
		MOV		r10, r7			; copy the system call number into r10
		LSL		r10, r10, #0x2		; system call number * 4
		; complete the rest of the code
		; your code may be of 4 to 8 lines
		ADD		r11, r11, r10 ; Add offset to base address of systemcall_table
		LDR		r0, [r11]	  ; Load address of system call function to r0
		
		BLX		r0 ; Branch to system call
		BX		lr				; return to SVC_Handler

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call 
; provided for you to use

_sys_exit
		PUSH 	{lr}		; save lr
		BLX		r11	
		POP 	{lr}		; resume lr
		BX		lr
		
_sys_malloc
		IMPORT	_kalloc
		LDR		r11, = _kalloc	
		PUSH 	{lr}		; save lr
		BLX		r11			; call the _kalloc function 
		POP 	{lr}		; resume lr
		BX		lr
		
_sys_free
		IMPORT	_kfree
		LDR		r11, = _kfree	
		PUSH 	{lr}		; save lr
		BLX		r11			; call the _kfree function 
		POP 	{lr}		; resume lr
		BX		lr
		
		END


		