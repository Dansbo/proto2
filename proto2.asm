!cpu w65c02

!src "cx16.inc"
+SYS_LINE

!src "vera0.9.inc"
!src "globals.inc"
!src "functions.inc"
!src "TextUI.inc"

;******************************************************************************
; Main program starts here.
;******************************************************************************
main:
	jsr load_chars	; Load special chars to draw gates into font memory
	jsr Init_VERA
	jsr Print_Title
	rts



LEVEL_BIN	!bin	"LEVELS.BIN"
