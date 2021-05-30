	jmp globals_end

GAME_GRID_TOP_LEFT_X=20
GAME_GRID_TOP_LEFT_Y=9
GAME_GRID_COLOR=(GREEN<<4)|BLACK
GATE_COLOR=(GREEN<<4)|BLACK
WIRE_OFF_COLOR=(GREEN<<4)|BLACK
WIRE_ON_COLOR=(GREEN<<4)|YELLOW
WIN_COLOR=(GREEN<<4)|RED
GAME_GRID_X_WIDTH=16
GAME_GRID_Y_WIDTH=16

LEVELS=LEVEL_BIN+2
LEVEL_PTR=TMP_PTR7

Level_par:	!byte	$00
Cur_level_ptr:	!word	$0000

Title0:		!PET PET_PURPLE,	"*****   *****    ****   ******   ****    **  **   ****",0
Title1: 	!PET PET_LIGHTBLUE,	"**  **  **  **  **  **    **    **  **   **  **  **  **",0
Title2: 	!PET PET_CYAN,		"**  **  **  **  **  **    **    **  **  ********     **",0
Title3: 	!PET PET_GREEN,		"*****   *****   **  **    **    **  **   **  **     **",0
Title4: 	!PET PET_YELLOW,	"**      ****    **  **    **    **  **  ********  **",0
Title5: 	!PET PET_ORANGE,	"**      ** **   **  **    **    **  **   **  **  **",0
Title6:		!PET PET_RED,		"**      **  **   ****     **     ****    **  **  ******",0

Welcome:	!PET PET_WHITE,		"welcome to this boolean logic circuit trainer",0

Hell0:		!PET PET_RED,		"******  *****    ****   **   **",0
Hell1:		!PET 			"**      **  **  **  **  *** ***",0
Hell2:		!PET			"**      **  **  **  **  *******",0
Hell3:		!PET			"****    *****   **  **  ** * **",0
Hell4:		!PET			"**      ****    **  **  **   **",0
Hell5:		!PET    		"**      ** **   **  **  **   **",0
Hell6:		!PET			"**      **  **   ****   **   **",0

Hell7:		!PET			"**  **  ******  **      **",0
Hell8:		!PET			"**  **  **      **      **",0
Hell9:		!PET			"**  **  **      **      **",0
Hell10:		!PET			"******  ****    **      **",0
Hell11:		!PET			"**  **  **      **      **",0
Hell12:		!PET			"**  **  **      **      **",0
Hell13:		!PET			"**  **  ******  ******  ******",0

Choose:		!PET PET_WHITE,		"do you want to play this game? (yes/no/other)",0

Create0:	!PET PET_PURPLE,	" *****   *****   ******     *     ******  ******  ****",0
Create1:	!PET PET_LIGHTBLUE,	"**   **  **  **  **       ** **     **    **      ** **",0
Create2:	!PET PET_CYAN,		"**       **  **  **      **   **    **    **      **  **",0
Create3:	!PET PET_GREEN,		"**       *****   ****    *******    **    ****    **  **",0
Create4:	!PET PET_YELLOW,	"**       ****    **      **   **    **    **      **  **",0
Create5:	!PET PET_ORANGE,	"**   **  ** **   **      **   **    **    **      ** **",0
Create6:	!PET PET_RED,		" *****   **  **  ******  **   **    **    ******  ****",0

By0:		!PET 			"******   ***  ***",0
By1:		!PET			"**  ***  ***  ***",0
By2:		!PET			"**  ***  ***  ***",0
By3:		!PET			"******     ****",0
By4:		!PET			"**  ***     **",0
By5:		!PET			"**  ***     **",0
By6:		!PET			"******      **",0

Copr0:		!PET PET_DARKGRAY,	"****       *     **  **   ****   ******   *****    ****    ****",0
Copr1:		!PET			"** **    ** **   **  **  **  **  **  ***  **  **  **  **  **  **",0
Copr2:		!PET			"**  **  **   **  *** **  **      **  ***  **  **  **  **  **",0
Copr3:		!PET			"**  **  *******  ******   ****   ******   *****   **  **   ****",0
Copr4:		!PET			"**  **  **   **  ** ***      **  **  ***  ****    **  **      **",0
Copr5:		!PET			"** **   **   **  **  **  **  **  **  ***  ** **   **  **  **  **",0
Copr6:		!PET			"****    **   **  **  **   ****   ******   **  **   ****    ****",0

Year:		!PET PET_WHITE,		"2021",0

Why0:		!PET			$F4,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$05,0
Why1:		!PET			$47," then why bother load the game? ",$A6,0
Why2:		!PET			$C3,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$77,$A7,0

Chars:
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


globals_end:
