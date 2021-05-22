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

Print_Title:
	ldx #10
	ldy #1
	jsr Go_XY

	ldx #<Title0
	ldy #>Title0
	jsr Print_Str

	ldx #10
	ldy #2
	jsr Go_XY

	ldx #<Title1
	ldy #>Title1
	jsr Print_Str

	ldx #10
	ldy #3
	jsr Go_XY

	ldx #<Title2
	ldy #>Title2
	jsr Print_Str

	ldx #10
	ldy #4
	jsr Go_XY

	ldx #<Title3
	ldy #>Title3
	jsr Print_Str

	ldx #10
	ldy #5
	jsr Go_XY

	ldx #<Title4
	ldy #>Title4
	jsr Print_Str

	ldx #10
	ldy #6
	jsr Go_XY

	ldx #<Title5
	ldy #>Title5
	jsr Print_Str

	ldx #10
	ldy #7
	jsr Go_XY

	ldx #<Title6
	ldy #>Title6
	jsr Print_Str

	rts
TextUI_end
