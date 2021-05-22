	jmp	Functions_end

;******************************************************************************
; Loads characters used for drawing gates into the existing charset
;******************************************************************************
load_chars:
	lda	#$10
	sta	VERA_ADDR_H	; Increment by 1, bank 0
	lda	#$FE		; Set address to
	sta	VERA_ADDR_M	; $F800+($dd*$8)=$FEE8
	lda	#$E8		; This places the custom chars
	sta	VERA_ADDR_L	; at the end of the font table

	lda	#<CHARS		; Save address of CHARS in ZP
	sta	TMP0
	lda	#>CHARS
	sta	TMP0+1

	ldy	#0
	ldx	#35		; number of characters to replace in font
@outloop:
	stx	TMP2		; Save .X in ZP location
	ldx	#8		; Counter for bytes in a character
-	lda	(TMP0),y
	sta	VERA_DATA0
	inc	TMP0		; Add 1 to 16 bit address stored in ZP
	bne	@noinc
	inc	TMP0+1
@noinc:
	dex
	bne	-		; Jump back and do next byte of character
	ldx	TMP2		; Restore .X from ZP
	dex
	bne	@outloop	; While we have not done all chars, jump back
	rts

;******************************************************************************
;Move text cursor
;******************************************************************************
Go_XY:
	phx            		;Switching X and Y around so X is
	phy            		;Horizontal position and Y is
	plx            		;Vertical as we know from graphing
	ply

	clc
	jsr PLOT
	rts

;******************************************************************************
;Print string function
;******************************************************************************
Print_Str:
	stx TMP1
	sty TMP2
	ldy #0

@Doprint
	lda (TMP1), Y
	beq @Printdone
	jsr CHROUT
	iny
	jmp @Doprint

@Printdone
	rts

Functions_end:
