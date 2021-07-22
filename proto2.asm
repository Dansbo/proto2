!cpu w65c02

!src "cx16.inc"
+SYS_LINE

!src "vera0.9.inc"
!src "farbranch.inc"
!src "globals.inc"
!src "functions.inc"
!src "levelfuncs.inc"
!src "TextUI.inc"

;******************************************************************************
; Main program starts here.
;******************************************************************************
main:
	jsr	load_vtui	; Load and initialize VTUI library
	jsr	VTUI_initialize
	jsr	load_chars	; Load special chars to draw gates into font memory
	jsr	Init_VERA
	jsr	Print_Title
@next_level:
	jsr	load_level

@loop:
	jsr	GETIN
	cmp	#0
	beq	@loop
	jsr	toggle_input
	jsr	check_inputs
	jsr	check_gates
	bcc	@loop

	; Tell that level is solved
	lda	#GAME_GRID_TOP_LEFT_X+10
	ldy	#GAME_GRID_TOP_LEFT_Y-2
	jsr	VTUI_gotoxy		; Place cursor appropriately on screen
	lda	#1
	jsr	VTUI_set_stride		; Ensure VERA stride is set correctly
	bra	@end_solv		; Jump over string to print
@solv:	!pet	"solved - press space"
@end_solv:
	ldy	#@end_solv-@solv	; Length of string
	lda	#<@solv			; low address of string
	sta	r0l
	lda	#>@solv			; high address of string
	sta	r0h
	ldx	#(BLACK<<4)|WHITE	; White text on black background
	jsr	VTUI_print_str		; Print string to let user know level is solved
	; Wait for keypress
-	jsr	GETIN
	cmp	#0
	beq	-
	; Can not jump directly to function that gets next level number as it
	; is a cheap local variable. Jumping to line 121 in TextUI.inc,
	; just below @Choice label
	jsr	Print_Title+$33C	; Ask user for next level to play
	bra	@next_level
@end:
	rts



LEVEL_BIN	!bin	"LEVELS.BIN"
