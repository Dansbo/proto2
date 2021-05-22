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

	rts
TextUI_end
