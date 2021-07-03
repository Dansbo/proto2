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
	stz TMP0

@loop
	jsr GETIN
	beq @loop

	cmp #'N'		;N for no
	beq @Print_no
	cmp #'O'		;O for other
	beq @Print_other
	cmp #' '		;space to print title
	+FBEQ Print_Title
	cmp #'Y'		;Y for Yes
	+FBEQ @Choice
	cmp #'Q'		;Q for quit
	bne +
	jmp	@end
+	bra	@loop

@Print_no:
	lda TMP0
	bne +
	+Write_XY 24, 28, Why0
	+Write_XY 24, 29, Why1
	+Write_XY 24, 30, Why2
	+Write_XY 24, 31, Why3
	lda #1
	sta TMP0
+	jmp 	@loop

@Print_other:
	lda TMP0
	bne +
	+Write_XY 24, 28, Undec0
	+Write_XY 24, 29, Undec1
	+Write_XY 24, 30, Undec2
	+Write_XY 24, 31, Undec3
	+Write_XY 24, 32, Undec4
	lda #1
	sta TMP0
+	jmp	@loop

@Choice:
	lda TMP0
	beq +
	jmp @loop
+	lda 	#PET_CLEAR
	jsr 	CHROUT

	+Write_XY 21, 29, Lvl
	jsr	get_num_levels
	sta	lvl
	stz	Dec10
	jsr	Bin2bcd
	lda	Bcd+1
	jsr	Print_byte
	lda	Bcd
	jsr	Print_byte
	lda	#":"
	JSR	CHROUT
	lda	#" "
	JSR	CHROUT
	ldy	#0			;Clear .Y so it can be used to call level
	ldx	#0			;Load X with 0 so we can count numbers pressed
@get_char
	JSR	CHRIN
	inx
	cmp	#$0D			;Has user pressed return?
	beq	@return
	cmp	#$29			;User pressed something else than a number
	+FBMI	@num_error
	cmp	#$40
	+FBPL	@num_error
	and	#$0F			;Now we are sure we have a number
	cpx	#1
	bne	@chk2
	sta	char1
	jmp	@get_char
@chk2	cpx	#2
	bne	@chk3
	sta	char2
	jmp	@get_char
@chk3	cpx	#4
	bpl	@thumbs			;If user has pressed more than 3 numbers show thumbs error
	sta	char3
	jmp	@get_char

@return
	cpx	#1
	beq	@default
	cpx	#2
	bne	@tens
	lda	char1
	jsr	Add1s
	jmp	@end

@tens
	cpx	#3
	bne	@hundreds
	lda	char1
	jsr	Multiply10x
	lda	char2
	jsr	Add1s
	jmp	@end

@hundreds
	lda	char1
	cmp	#3
	+FBPL	@Choice
	jsr	Multiply100x
	lda	char2
	jsr	Multiply10x
	lda	char3
	jsr	Add1s
	jmp	@end

@default
	ldy	#1
	sty	result

@clear
	lda	#PET_CLEAR
	jsr	CHROUT
	rts

@thumbs
	JSR	@clear

	+Write_XY 23, 28, Toom0
	+Write_XY 23, 29, Toom1
	+Write_XY 23, 30, Toom2
	+Write_XY 23, 31, Toom3

	jmp	@error_loop

@num_error
	jsr	@clear

	+Write_XY 23, 28, Numb0
	+Write_XY 23, 29, Numb1
	+Write_XY 23, 30, Numb2
	+Write_XY 23, 31, Numb3

	jmp	@error_loop

@not_zero
	JSR	@clear

	+Write_XY 23, 28, Nozero0
	+Write_XY 23, 29, Nozero1
	+Write_XY 23, 30, Nozero2
	+Write_XY 23, 31, Nozero3

	jmp	@error_loop

@error_loop
	jsr	GETIN
	beq	@error_loop

	cmp	#' '
	bne	@error_loop
	jmp	@Choice

@end	inc	lvl
	ldy	result
	cpy	#0
	+FBEQ	@not_zero
	cpy	lvl
	+FBPL	@Choice
	rts
TextUI_end
