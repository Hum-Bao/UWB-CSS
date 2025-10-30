		AREA	|.text|, CODE, READONLY, ALIGN=2
		THUMB

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void _bzero( void *s, int n )
; Parameters
;	s 		- pointer to the memory location to zero-initialize
;	n		- a number of bytes to zero-initialize
; Return value
;   none
		EXPORT	_bzero
_bzero
		; r0 = s
		; r1 = n
		PUSH {r1-r12,lr}
		; you need to add some code here for part 1 implmentation
		MOV r2, #0
loop_bzero		
		CMP	r1, #0
		BEQ end_bzero
		STRB r2, [r0], #1
		SUB	 r1,r1,#1
		B loop_bzero
end_bzero
		POP {r1-r12,lr}	
		BX		lr


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; char* _strncpy( char* dest, char* src, int size )
; Parameters
;   dest 	- pointer to the buffer to copy to
;	src		- pointer to the zero-terminated string to copy from
;	size	- a total of n bytes
; Return value
;   dest
		EXPORT	_strncpy
_strncpy
		; r0 = dest
		; r1 = src
		; r2 = size
		; r3 = a copy of original dest
		; r4 = src[i]
		PUSH {r1-r12,lr}		
		; you need to add some code here for part 1 implmentation
		MOV     r3, r0             ; Copy dest to r3
loop_strncpy
		CMP     r2, #0             ; Check if size is 0
		BEQ     end_strncpy        ; Exit if 0
		LDRB    r4, [r1], #1       ; Load byte from src, increment src pointer
		CMP     r4, #0             ; Check if null byte
		IT      EQ                 ; If null set r4 to 0
		MOVEQ   r4, #0             ; Null byte padding
		STRB    r4, [r0], #1       ; Store byte in dest, increment dest pointer
		SUB     r2, r2, #1         ; Decrement size
		B       loop_strncpy       ; Loop until size is 0
end_strncpy
		MOV r0, r3
		POP {r1-r12,lr}	
		BX		lr        
        

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; char* _strncat( char* dest, char* src, int size )
; Parameters
;   dest 	- pointer to the destination array
;	src		- pointer to string to be appended
;	size	- Maximum number of characters to be appended
; Return value
;   dest
		EXPORT	_strncat
_strncat
		; r0 = dest
		; r1 = src
		; r2 = size
		PUSH {r1-r12,lr}		
		; you need to add some code here for part 1 implmentation
		POP {r1-r12,lr}	
		BX		lr
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void* _malloc( int size )
; Parameters
;	size	- #bytes to allocate
; Return value
;   void*	a pointer to the allocated space
		EXPORT	_malloc
_malloc
		; r0 = size
		PUSH {r1-r12,lr}		
		; you need to add two lines of code here for part 2 implmentation
		MOV	r7, #1 ; set r7 to system call num for malloc (SYSTEMCALLTBL[1])
		SVC #0x0 ; Invoke SVC_Handler
		
		POP {r1-r12,lr}	
		BX	lr ; Return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void _free( void* addr )
; Parameters
;	size	- the address of a space to deallocate
; Return value
;   none
		EXPORT	_free
_free
		; r0 = addr
		PUSH {r1-r12,lr}		
		; you need to add two lines of code here for part 2 implmentation
		MOV	r7, #2 ; set r7 to system call num for malloc (SYSTEMCALLTBL[2])
		SVC	#0x0 ; Invoke SVC handler
		
		POP {r1-r12,lr}	
		BX		lr
		
		END
