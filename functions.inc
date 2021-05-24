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

	lda	#<Chars		; Save address of CHARS in ZP
	sta	TMP0
	lda	#>Chars
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

;******************************************************************************
; Repaints the entire gamegrid with value in GAME_GRID_COLOR
;******************************************************************************
reset_game_grid:
	lda	#$10
	sta	VERA_ADDR_H	; Increment by 1, High addr = 0
	lda	#GAME_GRID_TOP_LEFT_Y
	sta	VERA_ADDR_M
	lda	#GAME_GRID_TOP_LEFT_X<<1
	sta	VERA_ADDR_L

	ldy	#(GAME_GRID_Y_WIDTH*3)
@yloop:
	ldx	#(GAME_GRID_X_WIDTH*3)
@xloop:	lda	#' '
	sta	VERA_DATA0
	lda	#GAME_GRID_COLOR
	sta	VERA_DATA0
	dex
	bne	@xloop
	inc	VERA_ADDR_M
	lda	#GAME_GRID_TOP_LEFT_X<<1
	sta	VERA_ADDR_L
	dey
	bne	@yloop
	rts

!macro INC16 .wrd {
	inc	.wrd
	bne	+
	inc	.wrd+1
+
}

!macro ADD16 .wrd, .val {
	lda	.wrd
	clc
	adc	#.val
	sta	.wrd
	lda	.wrd+1
	adc	#0
	sta	.wrd+1
}

;******************************************************************************
; Find a specific level
;******************************************************************************
; INPUTS:	.Y = Level to be loaded (1=first level)
; OUTPUTS:	.C set if level not found otherwise LEVEL_PTR will point to
;		the level.
;******************************************************************************
find_level:!byte $db
	lda	#<LEVELS
	sta	LEVEL_PTR
	lda	#>LEVELS
	sta	LEVEL_PTR+1

@get_next:
	dey
	beq	@end
	+INC16 LEVEL_PTR	; Skip PAR value
@next_tile:
	lda	(LEVEL_PTR)
	cmp	#$0F
	bne	+
	+ADD16 LEVEL_PTR, 2
	bra	@get_next
+	+ADD16 LEVEL_PTR, 6
	bra	@next_tile
@end:
	clc
	lda	(LEVEL_PTR)
	cmp	#$FF
	bne	+
	sec
+	rts

;******************************************************************************
; Loads a specific level
;******************************************************************************
; INPUTS:	.Y = Level to be loaded (1=first level)
;******************************************************************************
load_level:
	jsr	find_level


	rts

;******************************************************************************
; Function to draw a gate-tile at specific coordinates.
;******************************************************************************
; INPUTS:	.X & .Y - Coordinates of top left corner of gate-tile
;		.A - Type of Gate
;	$00 - Input, OFF
;	$02 - Input, ON
;	$04 - AND gate
;	$06 - AND gate inverted top input
;	$08 - AND gate inverted botoom input
;	$0A - AND gate both inputs inverted
;	$0C - NAND gate
;	$0E - NAND gate inverted top input
;	$10 - NAND gate inverted bottom input
;	$12 - NAND gate both inputs inverted
;	$14 - OR gate
;	$16 - OR gate inverted top input
;	$18 - OR gate inverted bottom input
;	$1A - OR gate both inputs inverted
;	$1C - NOR gate
;	$1E - NOR gate inverted top input
;	$20 - NOR gate inverted bottom input
;	$22 - NOR gate both inputs inverted
;	$24 - XOR gate
;	$26 - XOR gate inverted top input
;	$28 - XOR gate inverted bottom input
;	$2A - XOR gate both inputs inverted
;	$2C - XNOR gate
;	$2E - XNOR gate inverted top input
;	$30 - XNOR gate inverted bottom input
;	$32 - XNOR gate both inputs inverted
;	$34 - NOT gate (inverter)
;	$36 - Output
;******************************************************************************
draw_gate:
	pha
	+VERA_SET_STRIDE 1
	txa
	asl
	sta	VERA_ADDR_L
	sty	VERA_ADDR_M
	ply
	lda	@draw_table,y
	sta	TMP_PTR0
	lda	@draw_table+1,y
	sta	TMP_PTR0+1
	ldx	#(GREEN<<4)|BLACK
	ldy	VERA_ADDR_L
	jmp	(TMP_PTR0)

@input_off:
	inc	VERA_ADDR_M
	inc	VERA_ADDR_L
	inc	VERA_ADDR_L
	lda	#$FF
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$43
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@input_on:
	inc	VERA_ADDR_M
	inc	VERA_ADDR_L
	inc	VERA_ADDR_L
	lda	#$FF
	sta	VERA_DATA0
	lda	#(GREEN<<4)|YELLOW
	sta	VERA_DATA0
	lda	#$43
	sta	VERA_DATA0
	lda	#(GREEN<<4)|YELLOW
	sta	VERA_DATA0
	rts
@and_gate:
	lda	#$DD		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E2		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E4		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$DF		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$E6		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$E0		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E3		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E5		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@and_top_inv:
	lda	#$DE		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E2		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E4		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$DF		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$E6		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$E0		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E3		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E5		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@and_bot_inv:
	lda	#$DD		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E2		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E4		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$DF		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$E6		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$E1		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E3		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E5		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@and_both_inv:
	lda	#$DE		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E2		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E4		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$DF		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$E6		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$E1		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E3		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E5		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@nand_gate:
	lda	#$DD		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E2		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E4		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$DF		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$E7		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$E0		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E3		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E5		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@nand_top_inv:
	lda	#$DE		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E2		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E4		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$DF		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$E7		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$E0		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E3		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E5		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@nand_bot_inv:
	lda	#$DD		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E2		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E4		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$DF		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$E7		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$E1		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E3		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E5		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@nand_both_inv:
	lda	#$DE		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E2		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E4		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$DF		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$E7		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$E1		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E3		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$E5		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@or_gate:
	lda	#$E8		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$ED		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$F1		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EB		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EE		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@or_top_inv:
	lda	#$E9		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$ED		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$F1		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EB		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EE		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@or_bot_inv:
	lda	#$E8		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$ED		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$F1		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EC		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EE		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@or_both_inv:
	lda	#$E9		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$ED		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$F1		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EC		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EE		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@nor_gate:
	lda	#$E8		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$ED		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$F2		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EB		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EE		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@nor_top_inv:
	lda	#$E9		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$ED		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$F2		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EB		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EE		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@nor_bot_inv:
	lda	#$E8		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$ED		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$F2		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EC		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EE		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@nor_both_inv:
	lda	#$E9		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$ED		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	VERA_DATA0
	lda	VERA_DATA0
	lda	#$F2		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EC		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EE		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@xor_gate:
	lda	#$F3		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F7		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F9
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F1		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$F5		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F8		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@xor_top_inv:
	lda	#$F4		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F7		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F9
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F1		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$F5		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F8		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@xor_bot_inv:
	lda	#$F3		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F7		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F9
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F1		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$F6		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F8		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@xor_both_inv:
	lda	#$F4		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F7		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F9
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F1		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$F6		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F8		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@xnor_gate:
	lda	#$F3		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F7		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F9
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F2		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$F5		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F8		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@xnor_top_inv:
	lda	#$F4		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F7		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F9
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F2		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$F5		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F8		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@xnor_bot_inv:
	lda	#$F3		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F7		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F9
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F2		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$F6		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F8		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@xnor_both_inv:
	lda	#$F4		; Top Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F7		; Top middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$EF		; Top right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$EA		; Middle left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F9
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F2		; Middle right
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$F6		; Bottom Left
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F8		; Bottom middle
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$F0		; Bottom Right
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@not_gate:
	lda	#$FA
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$FC
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$FE
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$43
	sta	VERA_DATA0
	stx	VERA_DATA0
	inc	VERA_ADDR_M
	sty	VERA_ADDR_L
	lda	#$FB
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@output:
	inc	VERA_ADDR_M
	lda	#$43
	sta	VERA_DATA0
	stx	VERA_DATA0
	lda	#$FF
	sta	VERA_DATA0
	stx	VERA_DATA0
	rts
@draw_table:
	!word	@input_off:
	!word	@input_on:
	!word	@and_gate:
	!word	@and_top_inv:
	!word	@and_bot_inv:
	!word	@and_both_inv:
	!word	@nand_gate:
	!word	@nand_top_inv:
	!word	@nand_bot_inv:
	!word	@nand_both_inv:
	!word	@or_gate:
	!word	@or_top_inv:
	!word	@or_bot_inv:
	!word	@or_both_inv:
	!word	@nor_gate:
	!word	@nor_top_inv:
	!word	@nor_bot_inv:
	!word	@nor_both_inv:
	!word	@xor_gate:
	!word	@xor_top_inv:
	!word	@xor_bot_inv:
	!word	@xor_both_inv:
	!word	@xnor_gate:
	!word	@xnor_top_inv:
	!word	@xnor_bot_inv:
	!word	@xnor_both_inv:
	!word	@not_gate:
	!word	@output:
	rts

Functions_end:
