		AREA	|.text|, CODE, READONLY, ALIGN=2
		THUMB

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void _bzero( void *s, int n )
; Parameters
;	s 		- pointer to the memory location to zero-initialize
;	n		- a number of bytes to zero-initialize
; Return value
;   none
; r0 = s
; r1 = n
; r2 = 0
		EXPORT	_bzero
_bzero
		MOV     r2, #0             ; Set r2 to 0
Loop_bzero
		CMP     r1, #0             ; Check if n is 0
		BEQ     End_bzero          ; If zero, exit
		STRB    r2, [r0], #1       ; Store 0 in r0, increment r0
		SUB     r1, r1, #1         ; Decrement r1 count
		B       Loop_bzero         ; Loop until r1 is 0
End_bzero
		BX      lr                 ; Return
			
		


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; char* _strncpy( char* dest, char* src, int size )
; Parameters
;   dest 	- pointer to the buffer to copy to
;	src		- pointer to the zero-terminated string to copy from
;	size	- a total of n bytes
; Return value
;   dest
; r0 = dest
; r1 = src
; r2 = size
; r3 = a copy of original dest
; r4 = src[i]
		EXPORT	_strncpy
_strncpy
		MOV     r3, r0             ; Copy dest to r3
Loop_strncpy
		CMP     r2, #0             ; Check if size is 0
		BEQ     End_strncpy        ; Exit if 0
		LDRB    r4, [r1], #1       ; Load byte from src, increment src pointer
		CMP     r4, #0             ; Check if null byte
		IT      EQ                 ; If null set r4 to 0
		MOVEQ   r4, #0             ; Null byte padding
		STRB    r4, [r0], #1       ; Store byte in dest, increment dest pointer
		SUB     r2, r2, #1         ; Decrement size
		B       Loop_strncpy       ; Loop until size is 0
End_strncpy
		MOV     r0, r3             ; copy r3 to dest
		BX      lr                 ; Return
		END




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		END			
