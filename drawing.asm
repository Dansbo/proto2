!cpu w65c02

!src "../cx16stuff/cx16.inc"

+SYS_LINE
!src "../cx16stuff/vera0.9.inc"

!macro PRINT_STR .str {
	bra	+
!ct "asc2vera.ct" {
.locstr	!text	.str
}
.locend
+	lda	#<.locstr
	sta	r0l
	lda	#>.locstr
	sta	r0h
	ldx	#0
	ldy	#.locend-.locstr

-	lda	.locstr,x
	sta	VERA_DATA0
	inx
	dey
	bne	-
}

main:
	lda	#PET_WHITE
	jsr	CHROUT
	lda	#PET_SWAP_FGBG
	jsr	CHROUT
	lda	#PET_BLACK
	jsr	CHROUT

	lda	#147
	jsr	CHROUT

	jsr	load_font

	+VERA_SET_STRIDE 2

	+VERA_SET_ADDR $00202
	lda	#255
	sta	VERA_DATA0
	+PRINT_STR " - $FF INPUT/OUTPUT NODE"

	+VERA_SET_ADDR $00402
	lda	#$DD
	sta	VERA_DATA0
	+PRINT_STR " - $DD AND TOP LEFT"

	+VERA_SET_ADDR $00602
	lda	#$DE
	sta	VERA_DATA0
	+PRINT_STR " - $DE AND TOP LEFT INVERTED"

	+VERA_SET_ADDR $00802
	lda	#$DF
	sta	VERA_DATA0
	+PRINT_STR " - $DF AND MID LEFT"

	+VERA_SET_ADDR $00A02
	lda	#$E0
	sta	VERA_DATA0
	+PRINT_STR " - $E0 AND BOTTOM LEFT"

	+VERA_SET_ADDR $00C02
	lda	#$E1
	sta	VERA_DATA0
	+PRINT_STR " - $E1 AND BOTTOM LEFT INVERTED"

	+VERA_SET_ADDR $00E02
	lda	#$E2
	sta	VERA_DATA0
	+PRINT_STR " - $E2 AND TOP MIDDLE"

	+VERA_SET_ADDR $01002
	lda	#$E3
	sta	VERA_DATA0
	+PRINT_STR " - $E3 AND BOTTOM MIDDLE"

	+VERA_SET_ADDR $01202
	lda	#$E4
	sta	VERA_DATA0
	+PRINT_STR " - $E4 AND TOP RIGHT"

	+VERA_SET_ADDR $01402
	lda	#$E5
	sta	VERA_DATA0
	+PRINT_STR " - $E5 AND BOTTOM RIGHT"

	+VERA_SET_ADDR $01602
	lda	#$E6
	sta	VERA_DATA0
	+PRINT_STR " - $E6 AND RIGHT MIDDLE"

	+VERA_SET_ADDR $01802
	lda	#$E7
	sta	VERA_DATA0
	+PRINT_STR " - $E7 AND RIGHT MID INVERTED (NAND)"

	+VERA_SET_ADDR $01A02
	lda	#$E8
	sta	VERA_DATA0
	+PRINT_STR " - $E8 OR TOP LEFT"

	+VERA_SET_ADDR $01C02
	lda	#$E9
	sta	VERA_DATA0
	+PRINT_STR " - $E9 OR TOP LEFT INVERTED"

	+VERA_SET_ADDR $01E02
	lda	#$EA
	sta	VERA_DATA0
	+PRINT_STR " - $EA OR LEFT MIDDLE"

	+VERA_SET_ADDR $02002
	lda	#$EB
	sta	VERA_DATA0
	+PRINT_STR " - $EB OR BOTTOM LEFT"

	+VERA_SET_ADDR $02202
	lda	#$EC
	sta	VERA_DATA0
	+PRINT_STR " - $EC OR BOTTOM LEFT INVERTED"

	+VERA_SET_ADDR $02402
	lda	#$ED
	sta	VERA_DATA0
	+PRINT_STR " - $ED OR TOP MIDDLE"

	+VERA_SET_ADDR $02602
	lda	#$EE
	sta	VERA_DATA0
	+PRINT_STR " - $EE OR BOTTOM MIDDLE"

	+VERA_SET_ADDR $02802
	lda	#$EF
	sta	VERA_DATA0
	+PRINT_STR " - $EF OR TOP RIGHT"

	+VERA_SET_ADDR $02A02
	lda	#$F0
	sta	VERA_DATA0
	+PRINT_STR " - $F0 OR BOTTOM RIGHT"

	+VERA_SET_ADDR $02C02
	lda	#$F1
	sta	VERA_DATA0
	+PRINT_STR " - $F1 OR RIGHT MIDDLE"

	+VERA_SET_ADDR $02E02
	lda	#$F2
	sta	VERA_DATA0
	+PRINT_STR " - $F2 OR RIGHT MID INVERTED (NOR)"

	+VERA_SET_ADDR $00250
	lda	#$F3
	sta	VERA_DATA0
	+PRINT_STR " - $F3 XOR TOP LEFT"

	+VERA_SET_ADDR $00450
	lda	#$F4
	sta	VERA_DATA0
	+PRINT_STR " - $F4 XOR TOP LEFT INVERTED"

	+VERA_SET_ADDR $00650
	lda	#$F5
	sta	VERA_DATA0
	+PRINT_STR " - $F5 XOR BOTTOM LEFT"

	+VERA_SET_ADDR $00850
	lda	#$F6
	sta	VERA_DATA0
	+PRINT_STR " - $F6 XOR BOTTOM LEFT INVERTED"

	+VERA_SET_ADDR $00A50
	lda	#$F7
	sta	VERA_DATA0
	+PRINT_STR " - $F7 XOR TOP MIDDLE"

	+VERA_SET_ADDR $00C50
	lda	#$F8
	sta	VERA_DATA0
	+PRINT_STR " - $F8 XOR BOTTOM MIDDLE"

	+VERA_SET_ADDR $00E50
	lda	#$F9
	sta	VERA_DATA0
	+PRINT_STR " - $F9 XOR CENTER"

	+VERA_SET_ADDR $01050
	lda	#$FA
	sta	VERA_DATA0
	+PRINT_STR " - $FA INVERTER TOP"

	+VERA_SET_ADDR $01250
	lda	#$FB
	sta	VERA_DATA0
	+PRINT_STR " - $FB INVERTER BOTTOM"

	+VERA_SET_ADDR $01450
	lda	#$FC
	sta	VERA_DATA0
	+PRINT_STR " - $FC INVERTER LEFT"

	+VERA_SET_ADDR $01650
	lda	#$FE
	sta	VERA_DATA0
	+PRINT_STR " - $FE INVERTER RIGHT"

	+VERA_SET_ADDR $01850
	lda	#$43
	sta	VERA_DATA0
	+PRINT_STR " - $43 HORIZONTAL WIRE"

	+VERA_SET_ADDR $01A50
	lda	#$5D
	sta	VERA_DATA0
	+PRINT_STR " - $5D VERTICAL WIRE"

	+VERA_SET_ADDR $01C50
	lda	#$70
	sta	VERA_DATA0
	+PRINT_STR " - $70 TOP LEFT CORNER"

	+VERA_SET_ADDR $01E50
	lda	#$6D
	sta	VERA_DATA0
	+PRINT_STR " - $6D BOTTOM LEFT CORNER"

	+VERA_SET_ADDR $02050
	lda	#$6E
	sta	VERA_DATA0
	+PRINT_STR " - $6E TOP RIGHT CORNER"

	+VERA_SET_ADDR $02250
	lda	#$7D
	sta	VERA_DATA0
	+PRINT_STR " - $7D BOTTOM RIGHT CORNER"

	+VERA_SET_ADDR $02450
	lda	#$73
	sta	VERA_DATA0
	+PRINT_STR " - $73 LEFT TEE"

	+VERA_SET_ADDR $02650
	lda	#$6B
	sta	VERA_DATA0
	+PRINT_STR " - $6B RIGHT TEE"

	+VERA_SET_ADDR $02850
	lda	#$71
	sta	VERA_DATA0
	+PRINT_STR " - $71 TOP TEE"

	+VERA_SET_ADDR $02A50
	lda	#$72
	sta	VERA_DATA0
	+PRINT_STR " - $72 BOTTOM TEE"

	jsr	CHRIN

	rts



load_font:
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

CHARS
; ******************* AND/NAND *************************
; Top left of AND
	!byte	%........
	!byte	%....####
	!byte	%....####
	!byte	%######..
	!byte	%######..
	!byte	%....##..
	!byte	%....##..
	!byte	%....##..

; Top left of AND inverted input
	!byte	%........
	!byte	%....####
	!byte	%.##.####
	!byte	%#..###..
	!byte	%#..###..
	!byte	%.##.##..
	!byte	%....##..
	!byte	%....##..

; Left mid of AND
	!byte	%....##..
	!byte	%....##..
	!byte	%....##..
	!byte	%....##..
	!byte	%....##..
	!byte	%....##..
	!byte	%....##..
	!byte	%....##..

; Bottom Left of AND
	!byte	%....##..
	!byte	%....##..
	!byte	%....##..
	!byte	%######..
	!byte	%######..
	!byte	%....####
	!byte	%....####
	!byte	%........

; Bottom Left of AND inverted input
	!byte	%....##..
	!byte	%....##..
	!byte	%.##.##..
	!byte	%#..###..
	!byte	%#..###..
	!byte	%.##.####
	!byte	%....####
	!byte	%........

; Top mid of AND
	!byte	%........
	!byte	%######..
	!byte	%########
	!byte	%......##
	!byte	%.......#
	!byte	%........
	!byte	%........
	!byte	%........

; Bottom mid of AND
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%.......#
	!byte	%......##
	!byte	%########
	!byte	%######..
	!byte	%........

; Top right of AND
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%#.......
	!byte	%##......
	!byte	%.##.....
	!byte	%.##.....

; Bottom right of AND
	!byte	%.##.....
	!byte	%.##.....
	!byte	%##......
	!byte	%#.......
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%........

; Right mid of AND
	!byte	%..##....
	!byte	%..##....
	!byte	%...#....
	!byte	%...#####
	!byte	%...#####
	!byte	%...#....
	!byte	%..##....
	!byte	%..##....

; Right mid of NAND
	!byte	%..##....
	!byte	%..##....
	!byte	%...#.##.
	!byte	%...##..#
	!byte	%...##..#
	!byte	%...#.##.
	!byte	%..##....
	!byte	%..##....

; ******************* OR/NOR *************************
; Top left OR
	!byte	%........
	!byte	%....####
	!byte	%....####
	!byte	%######..
	!byte	%#######.
	!byte	%.....##.
	!byte	%......##
	!byte	%......##

; Top left OR inverted input
	!byte	%........
	!byte	%....####
	!byte	%.##.####
	!byte	%#..###..
	!byte	%#..####.
	!byte	%.##..##.
	!byte	%......##
	!byte	%......##

; Left mid of OR
	!byte	%......##
	!byte	%......##
	!byte	%......##
	!byte	%......##
	!byte	%......##
	!byte	%......##
	!byte	%......##
	!byte	%......##


; Bottom left OR
	!byte	%......##
	!byte	%......##
	!byte	%.....##.
	!byte	%#######.
	!byte	%######..
	!byte	%....####
	!byte	%....####
	!byte	%........

; Bottom left OR inverted input
	!byte	%......##
	!byte	%......##
	!byte	%.##..##.
	!byte	%#..####.
	!byte	%#..###..
	!byte	%.##.####
	!byte	%....####
	!byte	%........

; Top mid of OR
	!byte	%........
	!byte	%#######.
	!byte	%########
	!byte	%.......#
	!byte	%.......#
	!byte	%........
	!byte	%........
	!byte	%........

; Bottom mid of OR
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%.......#
	!byte	%.......#
	!byte	%########
	!byte	%#######.
	!byte	%........

; Top right OR
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%#.......
	!byte	%#.......
	!byte	%#.......
	!byte	%##......

; Bottom right OR
	!byte	%##......
	!byte	%#.......
	!byte	%#.......
	!byte	%#.......
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%........

; Right mid of OR
	!byte	%.#......
	!byte	%.##.....
	!byte	%..#.....
	!byte	%...#####
	!byte	%...#####
	!byte	%..#.....
	!byte	%.##.....
	!byte	%.#......

; Right mid of NOR
	!byte	%.#......
	!byte	%.##.....
	!byte	%..#..##.
	!byte	%...##..#
	!byte	%...##..#
	!byte	%..#..##.
	!byte	%.##.....
	!byte	%.#......

; ******************* XOR ******************************
; Top left of XOR
	!byte	%........
	!byte	%....#..#
	!byte	%....##.#
	!byte	%######..
	!byte	%#######.
	!byte	%.....##.
	!byte	%......##
	!byte	%......##

; Top left of XOR inverted input
	!byte	%........
	!byte	%....#..#
	!byte	%.##.##.#
	!byte	%#..###..
	!byte	%#..####.
	!byte	%.##..##.
	!byte	%......##
	!byte	%......##

; Bottom left of XOR
	!byte	%......##
	!byte	%......##
	!byte	%.....##.
	!byte	%#######.
	!byte	%######..
	!byte	%....##.#
	!byte	%....#..#
	!byte	%........

; Bottom left of XOR inverted input
	!byte	%......##
	!byte	%......##
	!byte	%.##..##.
	!byte	%#..####.
	!byte	%#..###..
	!byte	%.##.##.#
	!byte	%....#..#
	!byte	%........

; Top mid of XOR
	!byte	%........
	!byte	%#######.
	!byte	%########
	!byte	%#......#
	!byte	%##.....#
	!byte	%.#......
	!byte	%.##.....
	!byte	%.##.....

; Bottom mid of XOR
	!byte	%.##.....
	!byte	%.##.....
	!byte	%.#......
	!byte	%##.....#
	!byte	%#......#
	!byte	%########
	!byte	%#######.
	!byte	%........

; Mid of XOR
	!byte	%.##.....
	!byte	%.##.....
	!byte	%.##.....
	!byte	%.##.....
	!byte	%.##.....
	!byte	%.##.....
	!byte	%.##.....
	!byte	%.##.....

; ******************* NOT/BUFFER ******************************
; Top of NOT
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%....#...
	!byte	%....##..
	!byte	%....###.
	!byte	%....####

; Bottom of NOT
	!byte	%....####
	!byte	%....###.
	!byte	%....##..
	!byte	%....#...
	!byte	%........
	!byte	%........
	!byte	%........
	!byte	%........

; Left of NOT
	!byte	%....##.#
	!byte	%....##..
	!byte	%....##..
	!byte	%######..
	!byte	%######..
	!byte	%....##..
	!byte	%....##..
	!byte	%....##.#

; Left of NOT inverted input
	!byte	%....##.#
	!byte	%....##..
	!byte	%.##.##..
	!byte	%#..###..
	!byte	%#..###..
	!byte	%.##.##..
	!byte	%....##..
	!byte	%....##.#

; Right of NOT
	!byte	%#.......
	!byte	%##......
	!byte	%.##..##.
	!byte	%..###..#
	!byte	%..###..#
	!byte	%.##..##.
	!byte	%##......
	!byte	%#.......

; INPUT/BUTTOM
	!byte	%...##...
	!byte	%..####..
	!byte	%.######.
	!byte	%########
	!byte	%########
	!byte	%.######.
	!byte	%..####..
	!byte	%...##...
