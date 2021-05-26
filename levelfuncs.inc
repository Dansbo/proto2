	jmp	Levelfuncs_end

;******************************************************************************
; Increment 16 bit memory location
;******************************************************************************
!macro INC16 .wrd {
	inc	.wrd
	bne	+
	inc	.wrd+1
+
}

;******************************************************************************
; Add #.val to 16 bit memory location
;******************************************************************************
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
; OUTPUTS:	.C set if level not found otherwise Cur_level_ptr and LEVEL_PTR
;		will point to the level.
;******************************************************************************
find_level:
	; Load level pointer with start of levels
	lda	#<LEVELS
	sta	LEVEL_PTR
	lda	#>LEVELS
	sta	LEVEL_PTR+1

@get_next:
	; Decrement level, if we have reached 0, check that it is a level
	dey
	beq	@end
	+INC16 LEVEL_PTR	; Skip PAR value
@next_tile:
	lda	(LEVEL_PTR)	; Load X coordinate of tile
	cmp	#$FF
	beq	@end
	cmp	#$0F		; If it is $0F, we have reached end of level
	bne	+
	; Add 2 to pointer to point to beginning of next level
	+ADD16 LEVEL_PTR, 2
	bra	@get_next
+	+ADD16 LEVEL_PTR, 6	; Add $06 to pointer to get to next tile
	bra	@next_tile
@end:
	lda	LEVEL_PTR
	sta	Cur_level_ptr
	lda	LEVEL_PTR+1
	sta	Cur_level_ptr+1
	lda	(LEVEL_PTR)
	cmp	#$FF		; Sets .C if .A >= to $FF
	rts

;******************************************************************************
; Count all levels and return the total number of levels in .A
;******************************************************************************
get_num_levels:
	ldy	#$FF		; Try to load more levels than exist
	jsr	find_level
	tya
	eor	#$FF		; Invert number
	dec
	rts

;******************************************************************************
; Multiply coordinate by 3
;******************************************************************************
!macro CALC_SCREEN_COORD .coord {
	sta	.coord		; Store coordinate
	asl			; Double coordinate
	clc
	adc	.coord		; Add original coordinate
}

;******************************************************************************
; Figure out the route from a tile output to a tile input and draw the wire
;******************************************************************************
; INPUTS:	TMP1 & TMP2 = X&Y coordinates for starting tile
;		TMP4 & TMP5 = X&Y coordinates for next tile
;		TMP6 = Input number on next tile
;******************************************************************************
draw_wire:
@X_COORD=TMP1
@Y_COORD=TMP2
@NEXT_X=TMP4
@NEXT_Y=TMP5
@NEXT_I=TMP6
	lda	@NEXT_X
	+CALC_SCREEN_COORD @NEXT_X	; Calculate X coordinate of next tile
	adc	#GAME_GRID_TOP_LEFT_X
	dec				; Decrement as wire should end before tile
	asl				; Double to get actual VERA memory address
	tax				; Save X coord to go to in .X
	lda	@NEXT_Y
	+CALC_SCREEN_COORD @NEXT_Y	; Caluclate Y coordinate of next tile
	adc	#GAME_GRID_TOP_LEFT_Y
	adc	@NEXT_I			; Add input-no to get coord of input
	tay				; Save Y coord to go to in .Y

	; Calculate coordinate of output for originating tile
	lda	@X_COORD		; Screen coordinate
	clc
	adc	#3			; Add 3 to get to tile to the right
	asl				; Double to get vera memory address
	sta	VERA_ADDR_L
	lda	@Y_COORD		; Screen coordinate
	inc				; Add 1 to Tile Y coord to get output coord.
	sta	VERA_ADDR_M

	lda	#$43			; Horizontal line
@horizontal:
	cpx	VERA_ADDR_L		; If VERA X = @NEXT_X it is time to go vertical
	beq	@start_vertical
	sta	VERA_DATA0		; Draw horizontal line
	inc	VERA_ADDR_L		; Skip color information
	bra	@horizontal
@start_vertical:
	cpy	VERA_ADDR_M
	beq	@end			; If Y coordinates are the same, we are done
	dec	VERA_ADDR_L		; Reset VERA X coordinate
	dec	VERA_ADDR_L
	bcc	+
	lda	#$6E			; Write corner
	sta	VERA_DATA0
	dec	VERA_ADDR_L		; Reset VERA X coordinate
	lda	#$5D
	bra	@vertical_down
+	lda	#$7D			; Write corner
	sta	VERA_DATA0
	dec	VERA_ADDR_L		; Reset VERA X Coordinate
	lda	#$5D
@vertical_up:
	dec	VERA_ADDR_M
	cpy	VERA_ADDR_M		; If we have reached Y coord of target, we are done
	bne	+
	lda	#$70			; Corner
	sta	VERA_DATA0
	inc	VERA_ADDR_L		; Skip color
	bra	@end
+	sta	VERA_DATA0		; Vertical line
	dec	VERA_ADDR_L		; Reset VERA X coordinate
	bra	@vertical_up
@vertical_down:
	inc	VERA_ADDR_M
	cpy	VERA_ADDR_M		; If we have reached Y coord of target, we are done
	bne	+
	lda	#$6D			; Corner
	sta	VERA_DATA0
	inc	VERA_ADDR_L		; Skip color
	bra	@end
+	sta	VERA_DATA0		; Vertical line
	dec	VERA_ADDR_L		; Reset VERA X coordinate
	bra	@vertical_down
@end:
	lda	#$43			; Write the last part of the line
	sta	VERA_DATA0
	rts

;******************************************************************************
; Loads a specific level
;******************************************************************************
; INPUTS:	.Y = Level to be loaded (1=first level)
;******************************************************************************
load_level:
@INPUT_CNT=TMP0
@X_COORD=TMP1
@Y_COORD=TMP2
@GRID_X=TMP3
@NEXT_X=TMP4
@NEXT_Y=TMP5
@NEXT_I=TMP6

	lda	#$10			; Make VERA increment by 1
	sta	VERA_ADDR_H

	lda	#$30			; PETSCII 0 in @INPUT_CNT
	sta	@INPUT_CNT

	jsr	find_level
	+FBCS	@end
	lda	(LEVEL_PTR)		; Read level PAR and store
	sta	Level_par

@load_tile:
	+INC16	LEVEL_PTR
	lda	(LEVEL_PTR)		; Load current tile X coordinate
	sta	@GRID_X
	+CALC_SCREEN_COORD @X_COORD	; Calculate screen coordinate
	adc	#GAME_GRID_TOP_LEFT_X	; Add Game grid offset
	sta	@X_COORD
	+INC16	LEVEL_PTR
	lda	(LEVEL_PTR)		; Load current tile Y coordinate
	+CALC_SCREEN_COORD @Y_COORD	; Calculate screen coordinate
	adc	#GAME_GRID_TOP_LEFT_Y	; add game grid offset
	sta	@Y_COORD

	lda	@GRID_X
	cmp	#$0F
	bne	+
	; Handling of the last tile
	ldx	@X_COORD
	ldy	@Y_COORD
	lda	#$36
	jmp	draw_gate		; return to caller after this

+	+INC16 LEVEL_PTR
	lda	(LEVEL_PTR)		; Load current tile type
	cmp	#00			; If it is input, we write input no.
	beq	@write_input_no
	cmp	#02
	beq	@write_input_no
	bra	@place_tile
@write_input_no:
	lda	@X_COORD		; Load x coodinate and subtract 1 to
	dec				; write the number to the left of the
	asl				; tile
	sta	VERA_ADDR_L
	lda	@Y_COORD		; Load y coordinate and increment to
	inc				; write the number mid of tile
	sta	VERA_ADDR_M
	inc	@INPUT_CNT		; Increment the input count and write it
	lda	@INPUT_CNT
	sta	VERA_DATA0
@place_tile:
	ldx	@X_COORD
	ldy	@Y_COORD
	lda	(LEVEL_PTR)
	jsr	draw_gate

	+INC16 LEVEL_PTR
	lda	(LEVEL_PTR)
	sta	@NEXT_X
	+INC16 LEVEL_PTR
	lda	(LEVEL_PTR)
	sta	@NEXT_Y
	+INC16	LEVEL_PTR
	lda	(LEVEL_PTR)
	sta	@NEXT_I
	jsr	draw_wire
	jmp	@load_tile
@end:
	rts

Levelfuncs_end:
