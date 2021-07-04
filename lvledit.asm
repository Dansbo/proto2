!cpu w65c02

!src "cx16.inc"
+SYS_LINE

!src "vera0.9.inc"
!src "farbranch.inc"
!src "globals.inc"
!src "functions.inc"
!src "levelfuncs.inc"

GATE_SELECT=0
GATE_PLACE=1
WIRE_START=2
WIRE_END=3
CLEAR=4
OUTPUT=5

ARROW_UP=$91
ARROW_DOWN=$11
ARROW_LEFT=$9D
ARROW_RIGHT=$1D
TABULATOR=$09
ENTER=$0D
SPACE=$20
ESCAPE=$03

	jmp	main

Selected_gate_type:
	!byte	$00
Prog_state:
	!byte	$00
Sel_grid_x:
	!byte	$00
Sel_grid_y:
	!byte	$00
Game_grid_x:
	!byte	$00
Game_grid_y:
	!byte	$00
Game_grid_ptr:
	!byte	$00

Grid_array:
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
	!byte	$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

main:
	jsr	load_vtui	; Load and initialize VTUI library
	jsr	VTUI_initialize
	jsr	load_chars	; Load special chars to draw gates into font memory

	stz	Selected_gate_type
	stz	Prog_state
	stz	Game_grid_ptr
	lda	#6
	sta	Sel_grid_x
	lda	#37
	sta	Sel_grid_y
	lda	#32
	sta	Game_grid_x
	lda	#12
	sta	Game_grid_y

	clc
	jsr	VTUI_set_bank

	lda	#' '
	ldx	#BLACK<<4|WHITE
	jsr	VTUI_clr_scr

	; Game grid
	jsr	clear_game_grid

	jsr	select_grid
	+VERA_GOXY ~Sel_grid_x, ~Sel_grid_y
	ldx	#RED<<4
	jsr	sel_grid_color

@loop:
	lda	Prog_state
@select
	cmp	#GATE_SELECT
	bne	@place
	jsr	select_gate
	bra	@loop
@place
	cmp	#GATE_PLACE
	bne	@start_wire
	;place gate
	!byte $db
	bra	@loop
@start_wire:
	cmp	#WIRE_START
	bne	@end_wire
	;start wire
	bra	@loop
@end_wire:
	cmp	#WIRE_END
	bne	@do_clear
	;end wire
	bra	@loop
@do_clear
	cmp	#CLEAR
	bne	@do_output
	jsr	clear_game_grid
	bra	@loop
@do_output:
	cmp	#OUTPUT
	bne	@loop
	;do output
	bra	@loop
	rts

place_gate:
	wai
	jsr	GETIN
	cmp	#0
	beq	place_gate

	rts

; Let the user select a gate
; Global variable Selected_gate_type is updated as the user selects
select_gate:
	wai
	jsr	GETIN
	cmp	#0
	beq	select_gate
	pha
	+VERA_GOXY ~Sel_grid_x, ~Sel_grid_y
	ldx	#GREEN<<4
	jsr	sel_grid_color
	pla
@chk_up:
	cmp	#ARROW_UP
	bne	@chk_down

	lda	Sel_grid_y
	cmp	#40
	bcc	@roll_up

	lda	Sel_grid_y
	sec
	sbc	#3
	sta	Sel_grid_y
	lda	Selected_gate_type
	sec
	sbc	#14
	sta	Selected_gate_type
	jmp	@set_col
@roll_up:
	lda	#46
	sta	Sel_grid_y
	lda	Selected_gate_type
	clc
	adc	#42
	sta	Selected_gate_type
	jmp	@set_col

@chk_down:
	cmp	#ARROW_DOWN
	bne	@chk_left

	lda	Sel_grid_y
	cmp	#46
	bcs	@roll_down

	lda	Sel_grid_y
	clc
	adc	#3
	sta	Sel_grid_y
	lda	Selected_gate_type
	clc
	adc	#14
	sta	Selected_gate_type
	bra	@set_col
@roll_down:
	lda	#37
	sta	Sel_grid_y
	lda	Selected_gate_type
	sec
	sbc	#42
	sta	Selected_gate_type
	bra	@set_col

@chk_left:
	cmp	#ARROW_LEFT
	bne	@chk_right

	lda	Sel_grid_x
	cmp	#9
	bcc	@roll_left

	lda	Sel_grid_x
	sec
	sbc	#3
	sta	Sel_grid_x
	dec	Selected_gate_type
	dec	Selected_gate_type
	bra	@set_col

@roll_left:
	lda	#24
	sta	Sel_grid_x
	lda	Selected_gate_type
	clc
	adc	#12
	sta	Selected_gate_type
	bra	@set_col

@chk_right:
	cmp	#ARROW_RIGHT
	bne	@chk_tab:

	lda	Sel_grid_x
	cmp	#24
	bcs	@roll_right

	lda	Sel_grid_x
	clc
	adc	#3
	sta	Sel_grid_x
	inc	Selected_gate_type
	inc	Selected_gate_type
	bra	@set_col
@roll_right:
	lda	#6
	sta	Sel_grid_x
	lda	Selected_gate_type
	sec
	sbc	#12
	sta	Selected_gate_type
	bra	@set_col

@chk_tab:
	cmp	#TABULATOR
	+FBNE	select_gate
	lda	#GATE_PLACE
	sta	Prog_state
	rts
@set_col:
	+VERA_GOXY ~Sel_grid_x, ~Sel_grid_y
	ldx	#RED<<4
	jsr	sel_grid_color
	jmp	select_gate
	rts

clear_game_grid:
	+VERA_GOXY 32, 12
	lda	#16*3
	sta	r1l
	sta	r2l
	ldx	#GREEN<<4|BLACK
	lda	#' '
	jsr	VTUI_fill_box

	ldx	#(16*16)-1
@loop:
	stz	Grid_array,x
	dex
	bpl	@loop


; Set bg color of select grid field to value in .X
; Value should only be 4 bit and must be in upper nibble
sel_grid_color:
	+VERA_SET_STRIDE 0

	lda	VERA_ADDR_L
	clc
	adc	#5
	sta	VERA_ADDR_L
	inc	VERA_ADDR_M
	lda	VERA_DATA0
	and	#$0F
	stx	VERA_DATA0
	ora	VERA_DATA0
	tax
	sta	VERA_DATA0
	dec	VERA_ADDR_M
	lda	VERA_ADDR_L
	sec
	sbc	#5
	sta	VERA_ADDR_L

	+VERA_SET_STRIDE 2
	inc	VERA_ADDR_L
	ldy	VERA_ADDR_L
	stx	VERA_DATA0
	stx	VERA_DATA0
	stx	VERA_DATA0
	sty	VERA_ADDR_L
	inc	VERA_ADDR_M
	stx	VERA_DATA0
	stx	VERA_DATA0
	stx	VERA_DATA0
	sty	VERA_ADDR_L
	inc	VERA_ADDR_M
	stx	VERA_DATA0
	stx	VERA_DATA0
	stx	VERA_DATA0
	dec	VERA_ADDR_L
	+VERA_SET_STRIDE 1
	rts

select_grid:
	+VERA_GOXY 5, 36

	lda	#(7*3)+2
	sta	r1l
	lda	#(4*3)+2
	sta	r2l
	ldx	#GREEN<<4|BLACK
	lda	#' '
	jsr	VTUI_fill_box

	ldx	#6
	ldy	#37
	lda	#0
	jsr	draw_gate
	ldx	#9
	ldy	#37
	lda	#2
	jsr	draw_gate
	ldx	#12
	ldy	#37
	lda	#4
	jsr	draw_gate
	ldx	#15
	ldy	#37
	lda	#6
	jsr	draw_gate
	ldx	#18
	ldy	#37
	lda	#8
	jsr	draw_gate
	ldx	#21
	ldy	#37
	lda	#10
	jsr	draw_gate
	ldx	#24
	ldy	#37
	lda	#12
	jsr	draw_gate

	ldx	#6
	ldy	#40
	lda	#14
	jsr	draw_gate
	ldx	#9
	ldy	#40
	lda	#16
	jsr	draw_gate
	ldx	#12
	ldy	#40
	lda	#18
	jsr	draw_gate
	ldx	#15
	ldy	#40
	lda	#20
	jsr	draw_gate
	ldx	#18
	ldy	#40
	lda	#22
	jsr	draw_gate
	ldx	#21
	ldy	#40
	lda	#24
	jsr	draw_gate
	ldx	#24
	ldy	#40
	lda	#26
	jsr	draw_gate

	ldx	#6
	ldy	#43
	lda	#28
	jsr	draw_gate
	ldx	#9
	ldy	#43
	lda	#30
	jsr	draw_gate
	ldx	#12
	ldy	#43
	lda	#32
	jsr	draw_gate
	ldx	#15
	ldy	#43
	lda	#34
	jsr	draw_gate
	ldx	#18
	ldy	#43
	lda	#36
	jsr	draw_gate
	ldx	#21
	ldy	#43
	lda	#38
	jsr	draw_gate
	ldx	#24
	ldy	#43
	lda	#40
	jsr	draw_gate

	ldx	#6
	ldy	#46
	lda	#42
	jsr	draw_gate
	ldx	#9
	ldy	#46
	lda	#44
	jsr	draw_gate
	ldx	#12
	ldy	#46
	lda	#46
	jsr	draw_gate
	ldx	#15
	ldy	#46
	lda	#48
	jsr	draw_gate
	ldx	#18
	ldy	#46
	lda	#50
	jsr	draw_gate
	ldx	#21
	ldy	#46
	lda	#52
	jsr	draw_gate
	ldx	#24
	ldy	#46
	lda	#54
	jsr	draw_gate
	rts

LEVEL_BIN:
