	jmp TextUI_end

Init_VERA:
	;Force 80x60 screen mode
	lda #1
	jsr Screen_set_mode

	;Black BG, white text, clear screeen
	lda #PET_BLACK
	jsr CHROUT
	lda #PET_SWAP_FGBG
	jsr CHROUT
	lda #PET_WHITE
	jsr CHROUT
	lda #PET_CLEAR
	jsr CHROUT

	rts

!macro Write_XY .xc, .yc, .strlabel {
	ldx	#.xc
	ldy	#.yc
	jsr	Go_XY
	ldx	#<.strlabel
	ldy	#>.strlabel
	jsr	Print_Str
}

Print_Title:
	+Write_XY 10, 1, Title0
	+Write_XY 10, 2, Title1
	+Write_XY 10, 3, Title2
	+Write_XY 10, 4, Title3
	+Write_XY 10, 5, Title4
	+Write_XY 10, 6, Title5
	+Write_XY 10, 7, Title6
	rts
TextUI_end
