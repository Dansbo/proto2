	jmp	Functions_end

;******************************************************************************
;Multiply .A with 100
;.A is input, result is OUTPUT, Modifies .Y
;******************************************************************************
Multiply100x:
	tay
	lda	@tbl100,y
	sta	result
	rts
@tbl100:
	!byte	0,100,200,0,0,0,0,0,0,0

;******************************************************************************
;Multiply .A with 10 and add to result
;.A is input, result is OUTPUT, modifies .X, .Y
;******************************************************************************
Multiply10x:
	tay
	lda	@tbl10,y
	clc
	adc	result
	sta	result
	rts
@tbl10:
	!byte	0,10,20,30,40,50,60,70,80,90

;******************************************************************************
;Adds .A to result
;.A is input, result is OUTPUT
;******************************************************************************
Add1s:
	clc
	adc	result
	sta	result		;Result = Result + .A
	rts

;******************************************************************************
;Convert Binary number to decimal
;.A is input , OUTPUT = Bcd, Bcd+1
;******************************************************************************
Bin2bcd:
	STA	Bin		;Store .A in Bin
	stz	Bcd		;Clear output
	stz	Bcd+1
	ldx 	#8
	sed

@convert:
	asl	Bin
	lda	Bcd
	adc	Bcd
	sta	Bcd
	lda	Bcd+1
	adc	Bcd+1
	sta	Bcd+1
	dex
	bne	@convert
	cld

	rts
;******************************************************************************
;Print BCD value on screen
;Input value Bcd in bcd format
;******************************************************************************
Print_byte:
	pha			;Save original value on stack
	lsr			;Shift right to just get top nibble
	lsr
	lsr
	lsr
	tay			;Transfer .A to .Y
	clc			;clear carry bit
	adc Dec10		;Add to Dec10
	sta Dec10		;Store new value to Dec10
	cmp #1			;If Dec10 is less than 1 then skip
	bcc +
	tya			;Transfer original value back to .A
	ora #$30
	jsr CHROUT
+	pla			;Pull original value from stack
	and #$0F
	tay
	clc
	adc Dec10
	sta Dec10
	cmp #1
	bcc +
	tya
	ora #$30
	jsr CHROUT

+	rts
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

	lda	#GAME_GRID_TOP_LEFT_X
	ldy	#GAME_GRID_TOP_LEFT_Y
	jsr	VTUI_gotoxy

	lda	#GAME_GRID_WIDTH*3
	sta	r1l
	lda	#GAME_GRID_HEIGHT*3
	sta	r2l
	ldx	#GAME_GRID_COLOR
	lda	#' '
	jmp	VTUI_fill_box

;******************************************************************************
; Use KERNAL functions to load the VTUI library to Golden RAM ($0400)
;******************************************************************************
load_vtui:
	lda	#1			; Logical file number (must be unique)
	ldx	#8			; Device number (8 local filesystem)
	ldy	#0			; Secondary command 0 = dont use addr in file
	jsr	SETLFS			; SETLFS
	lda	#(End_file_name-Vtui_file_name)	; Length of filename
	ldx	#<Vtui_file_name	; Address of filename
	ldy	#>Vtui_file_name
	jsr	SETNAM			; SETNAM
	lda	#0			; 0=load, 1=verify
	ldx	#<VTUI_START		; Load file to LIBSTART
	ldy	#>VTUI_START
	jsr	LOAD			; LOAD
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
	pha				; Save gate type on stack
	+VERA_SET_STRIDE 1
	txa				; Calculate X address from X coordinate
	asl
	sta	VERA_ADDR_L		; Store X address
	sty	VERA_ADDR_M		; Store Y Address
	ply				; Use gate type as index to jmp table
	lda	@draw_table,y		; Copy address to zero page
	sta	TMP_PTR6
	lda	@draw_table+1,y
	sta	TMP_PTR6+1
	ldx	#GATE_COLOR		; Store GATE_COLOR in .X register
	ldy	VERA_ADDR_L		; Save X address in .Y register
	jmp	(TMP_PTR6)		; Indirect jump

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
	lda	#WIRE_ON_COLOR
	sta	VERA_DATA0
	lda	#$43
	sta	VERA_DATA0
	lda	#WIRE_ON_COLOR
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
