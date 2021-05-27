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
	jsr load_chars	; Load special chars to draw gates into font memory
	jsr Init_VERA
	jsr Print_Title

; **** Demo of game logic ****
	jsr	reset_game_grid
	ldy	#1
	jsr	load_level

@loop:
	jsr	GETIN
	cmp	#0
	beq	@loop:
	jsr	toggle_input
	jsr	check_inputs
	jsr	check_gates
	bcc	@loop
@end:
; **** Demo ends ****
	rts



LEVEL_BIN	!bin	"LEVELS.BIN"
