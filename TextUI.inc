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
	+Write_XY 12, 1, Title0
	+Write_XY 12, 2, Title1
	+Write_XY 12, 3, Title2
	+Write_XY 12, 4, Title3
	+Write_XY 12, 5, Title4
	+Write_XY 12, 6, Title5
	+Write_XY 12, 7, Title6
	+Write_XY 16, 10, Welcome
	+Write_XY 0, 13, Hell0
	+Write_XY 0, 14, Hell1
	+Write_XY 0, 15, Hell2
	+Write_XY 0, 16, Hell3
	+Write_XY 0, 17, Hell4
	+Write_XY 0, 18, Hell5
	+Write_XY 0, 19, Hell6
	+Write_XY 39, 21, Hell7
	+Write_XY 39, 22, Hell8
	+Write_XY 39, 23, Hell9
	+Write_XY 39, 24, Hell10
	+Write_XY 39, 25, Hell11
	+Write_XY 39, 26, Hell12
	+Write_XY 39, 27, Hell13
	+Write_XY 16, 30, Choose
	+Write_XY 12, 33, Create0
	+Write_XY 12, 34, Create1
	+Write_XY 12, 35, Create2
	+Write_XY 12, 36, Create3
	+Write_XY 12, 37, Create4
	+Write_XY 12, 38, Create5
	+Write_XY 12, 39, Create6
	rts
TextUI_end
