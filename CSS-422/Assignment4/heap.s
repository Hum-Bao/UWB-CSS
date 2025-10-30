		AREA	|.text|, CODE, READONLY, ALIGN=2
		THUMB

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call Table
HEAP_TOP	EQU		0x20001000
HEAP_BOT	EQU		0x20004FE0
MAX_SIZE	EQU		0x00004000		; 16KB = 2^14
MIN_SIZE	EQU		0x00000020		; 32B  = 2^5
	
MCB_TOP		EQU		0x20006800      ; 2^10B = 1K Space
MCB_BOT		EQU		0x20006BFE
MCB_ENT_SZ	EQU		0x00000002		; 2B per entry
MCB_TOTAL	EQU		512				; 2^9 = 512 entries
	
INVALID		EQU		-1				; an invalid id
	
;
; Each MCB Entry
; FEDCBA9876543210
; 00SSSSSSSSS0000U					S bits are used for Heap size, U=1 Used U=0 Not Used

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Memory Control Block Initialization
; void _kinit( )
; this routine must be called from Reset_Handler in startup_TM4C129.s
; before you invoke main( ) in driver_keil
		EXPORT	_kinit
_kinit
		PUSH {r1-r3, lr}
		LDR r1, =HEAP_TOP       ; Top of heap
		LDR r2, =HEAP_BOT       ; End of heap
		MOV r3, #0              ; Set heap to 0

loop_heap
		CMP r1, r2				; Check if heap is initialized
		BGE end_heap
		STR r3, [r1], #4        ; Write in 4 byte increments
		B loop_heap

end_heap
		; Initialize MCB entries
		LDR r1, =MCB_TOP        ; Top of MCB
		LDR r2, =MCB_BOT        ; End of MCB
		LDR r3, =MAX_SIZE       ; Set first entry to max size
		STR r3, [r1], #2        ; Store val and move to next

		MOV r3, #0             ; Initialize remaining entries

loop_mcb
		CMP r1, r2				; Check if all entries are initialized
		BGE end_mcb
		STR r3, [r1], #2 		; Zero entry adn move to next
		B loop_mcb

end_mcb
		POP {r1-r3, lr}
		BX lr
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Recursive _kalloc
_ralloc
	PUSH {r3-r12, lr}
	SUB r10, r2, r1            ; Calculate total MCB space
    LDR r8, =MCB_ENT_SZ
    ADD r3, r10, r8            ; Cache entire_mcb_addr_space
    LSR r4, r3, #1             ; half_mcb_addr_space
    ADD r5, r1, r4             ; midpoint_mcb_addr
    MOV r6, #0                 ; Initialize heap_addr to 0
    LSL r9, r3, #4             ; act_entire_heap_size
    LSL r10, r4, #4            ; act_half_heap_size

    CMP r0, r10                ; Compare size with half_heap_size
    BLE left_ralloc           ; If less than or equal, branch left

    LDR r8, [r1]               ; Load left_mcb_addr
    TST r8, #0x01              ; Test LSB (used flag)
    BNE return_zero_ralloc     ; If set, return 0

    CMP r8, r9                 ; Check heap size validity
    BLT return_zero_ralloc     ; If less than, return 0

    ORR r8, r9, #0x01          ; Mark entire space as used
    STR r8, [r1]               ; Store back to left_mcb_addr
    B return_heap_ralloc	
		
left_ralloc
		MOV r8, r0				; Store requested size
		PUSH {r1-r5, r8-r10, lr}
		LDR r8, =MCB_ENT_SZ
		SUB r2, r5, r8			; End address -> midpoint
		BL _ralloc
		MOV r6, r0				; Save result
		POP {r1-r5, r8-r10, lr} 
		CMP r6, #0				; Check if allocation succeeded
		BEQ left_ralloc_zero
		
		LDR r8, [r5]			; Load midpoint MCB
		TST r8, #0x1
		BNE end_ralloc
		STR r10, [r5]			; Update midpoint MCB
		B end_ralloc
	
left_ralloc_zero
		MOV r0, r8
		PUSH {r1-r5, r9-r10, lr}
		MOV r1, r5				; Update start address to midpoint
		BL _ralloc
		POP {r1-r5, r9-r10, lr}
		B end_ralloc 
		
return_zero_ralloc
		MOV r0, #0
		B end_ralloc
		
return_heap_ralloc
		LDR r8, =MCB_TOP
		SUB r1, r1, r8			; MCB index
		LSL r1, r1, #4
		LDR r8, =HEAP_TOP
		ADD r0, r1, r8			; Add to heap address
		B end_ralloc

end_ralloc
		POP {r3-r12, lr}
		BX lr 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Kernel Memory Allocation
; void* _k_alloc( int size )
		EXPORT	_kalloc
_kalloc
		PUSH {r1-r2, lr}
		LDR r1, =MCB_TOP
		LDR r2, =MCB_BOT
		BL	_ralloc
		POP {r1-r2, lr}
		BX	lr	
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Recursive _kfree		
_rfree
		PUSH {r1-r12, lr}
		LDR r1, [r0]			; Load MCB
		LDR r8, =MCB_TOP		; Load MCV top
		SUB r2, r0, r8			; MCB index
		LSR r1, r1, #4
		MOV r3, r1				; Block size
		LSL r1, r1, #4
		MOV r4, r1			
		
		STR r1, [r0]			; Update MCB entry
		
		MOV r5, #2
		SDIV r8, r2, r3			; Divide MCB indedx by block size
		SDIV r9, r8, r5
		MLS r10, r5, r9, r8
		CMP r10, #0				; Check if block is left
		BEQ free_left
		B 	free_right
		
free_left
		ADD r8, r0, r3			; Calculate right buddy address
		LDR r9, = MCB_BOT
		CMP r8, r9				; Check if buddy is within bounds
		BGE return_zero_rfree
		ADD r9, r0, r3 			; Right buddy address
		LDR r8, [r9]
		
		AND r10, r8, #0x0001	; Check if buddy is in use
		CMP r10, #0
		BNE end_rfree
		
		AND	r8, r8, #0xFFFFFFE0	; Cleary buddy lower bits
		MOV r10, #0
		STR r10, [r9]			; Mark buddy as free
		LSL r4, r4, #1			; Double block size
		STR r4, [r0]
		PUSH {r1-r12, lr}
		BL _rfree
		POP {r1-r12, lr}
		B end_rfree
		
free_right
		SUB r8, r0, r3			; Calculate left buddy address
		LDR r9, = MCB_TOP
		CMP r8, r9				; Check if buddy is within bounds
		BLT return_zero_rfree
		
		SUB r9, r0, r3 			; Left buddy address
		LDR r8, [r9]
		
		AND r10, r8, #0x0001	; Check if buddy is in use
		CMP r10, #0
		BNE end_rfree

		AND r8, r8, #0xFFFFFFE0 ; Clear buddy lower bits
		MOV r10, #0
		STR r10, [r0]			; Mark block as free
		LSL r4, r4, #1			; Double block size
		STR r4, [r9]
		MOV r0, r9
		PUSH {r1-r12, lr}
		BL _rfree
		POP {r1-r12, lr}
		B end_rfree
return_zero_rfree
		MOV r0, #0
		B end_rfree

end_rfree	
		POP {r1-r12, lr}
		BX lr
		

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Kernel Memory De-allocation
; void *_kfree( void *ptr )
		EXPORT	_kfree
_kfree
		MOV r3, r0 				; Save heap address
		LDR r1, =HEAP_TOP
		LDR r2, =HEAP_BOT
		CMP r0, r1				; Check if address is < HEAP_TOP
		BLT invalid
		CMP r0, r2				; Check if address is > HEAP_BOT
		BGT invalid
		
		SUB r0, r0, r1			; Offset from HEAP_TOP
		LSR r0, r0, #4
		LDR r1, =MCB_TOP		
		ADD r0, r0, r1			; MCB address

		PUSH {lr}
		BL _rfree
		POP	{lr}
		
		CMP r0, #0				; Check if _rfree failed
		BEQ invalid
		MOV r0, r3
		BX	lr
		
invalid
		LDR r0, =INVALID
		BX	lr
	
		END