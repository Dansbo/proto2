	jmp TextUI_end

Init_VERA:
	lda #1
	jsr Screen_set_mode

	lda #PET_BLACK
	jsr CHROUT

	lda #PET_SWAP_FGBG
	jsr CHROUT

	lda #PET_WHITE
	jsr CHROUT

	lda #PET_CLEAR
	jsr CHROUT

	rts

TextUI_end
