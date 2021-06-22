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
	lda #PET_CLEAR
	jsr CHROUT
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
	+Write_XY 31, 42, By0
	+Write_XY 31, 43, By1
	+Write_XY 31, 44, By2
	+Write_XY 31, 45, By3
	+Write_XY 31, 46, By4
	+Write_XY 31, 47, By5
	+Write_XY 31, 48, By6
	+Write_XY 7, 51, Copr0
	+Write_XY 7, 52, Copr1
	+Write_XY 7, 53, Copr2
	+Write_XY 7, 54, Copr3
	+Write_XY 7, 55, Copr4
	+Write_XY 7, 56, Copr5
	+Write_XY 7, 57, Copr6
	+Write_XY 75, 59, Year

@loop
	jsr GETIN
	cmp #0
	beq @loop

	cmp #'N'		;N for no
	beq @Print_no
	cmp #'O'		;O for other
	beq @Print_other
	cmp #' '		;space to print title
	jmp Print_Title
	cmp #'Y'		;Y for Yes
	bne +
	jmp @Choice
+	cmp #'Q'		;Q for quit
	bne +
	jmp	@end
+	bra	@loop

@Print_no:
	+Write_XY 24, 28, Why0
	+Write_XY 24, 29, Why1
	+Write_XY 24, 30, Why2
	+Write_XY 24, 31, Why3
	jmp 	@loop

@Print_other:
	+Write_XY 24, 28, Undec0
	+Write_XY 24, 29, Undec1
	+Write_XY 24, 30, Undec2
	+Write_XY 24, 31, Undec3
	+Write_XY 24, 32, Undec4
	jmp	@loop

@Choice:
	lda 	#PET_CLEAR
	jsr 	CHROUT

	+Write_XY 24, 29, Lvl
	JSR	GETIN


@end	rts
TextUI_end
