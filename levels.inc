	jmp	Levels_inc

; The game uses a board that is 16x16 fields. When designing a level there
; are certain rules that must be followed.
; 1: Tiles in column 0 (Y=0) can only be of the Input type, either on or off
; 2: There can only be a single tile in column F (Y=F).
; 3: The tile in column F can only be output so the type is not defined in
;    the level
; 4: All inputs and outputs must be connected.
; 5: Levels must be designed so no wires cross
; 6: Coordinates for the Output tile in column F must be the last  in a level

;   Representation of the game board
;     0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; 0 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; 1 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; 2 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; 3 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; 4 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; 5 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; 6 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; 7 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; 8 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; 9 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; A |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; B |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; C |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; D |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; E |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
; F |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
;   +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+

;   List of tile types
;	$00 - Input, OFF
;	$02 - Input, ON
;	$04 - AND gate
;	$06 - AND gate inverted top input
;	$08 - AND gate inverted botoom input
;	$0A - AND gate both inputs inverted
;	$0C - NAND gate
;	$0E - NAND gate inverted top input
;	$10 - NAND gate inverted bottom input
;	$12 - NAND gate both inputs inverted
;	$14 - OR gate
;	$16 - OR gate inverted top input
;	$18 - OR gate inverted bottom input
;	$1A - OR gate both inputs inverted
;	$1C - NOR gate
;	$1E - NOR gate inverted top input
;	$20 - NOR gate inverted bottom input
;	$22 - NOR gate both inputs inverted
;	$24 - XOR gate
;	$26 - XOR gate inverted top input
;	$28 - XOR gate inverted bottom input
;	$2A - XOR gate both inputs inverted
;	$2C - XNOR gate
;	$2E - XNOR gate inverted top input
;	$30 - XNOR gate inverted bottom input
;	$32 - XNOR gate both inputs inverted
;	$34 - NOT gate (inverter)
;	$36 - Output

;   List of tile input placements
;	0 - Top
;	1 - Middle
;	2 - Bottom

; When defining a level for the games, it is done be defining tiles and their
; placement on the board.
; For each tile, the following information must be defined.
; * X coordinate of tile on game board (0-F)
; * Y coordinate of tile on game board (0-F)
; * Tile type
; * X coordinate of tile that has an input connected to this tile-output
; * Y coordinate of tile that has an input connected to this tile-output
; * Input placment on tile connected to this tile-output

;   Example level with two inputs, 1 On and 1 off, an AND gate and an output
	; First Input
;	!byte	$00	; X coordinate of tile
;	!byte	$05	; Y coordinate of tile
;	!byte	$00	; Tile-type = Input, Off
;	!byte	$07	; X coordinate of next tile
;	!byte	$06	; Y coordinate of next tile
;	!byte	$00	; Output of this tile goes to top input of next tile
	; Second Input
;	!byte	$00	; X coordinate of tile
;	!byte	$07	; Y coordinate of tile
;	!byte	$01	; Tile-type = Input, On
;	!byte	$07	; X coordinate of next tile
;	!byte	$06	; Y coordinate of next tile
;	!byte	$02	; Output of this tile goes to bottom input of next tile
	; AND gate
;	!byte	$07	; X coordinate of tile
;	!byte	$06	; Y coordinate of tile
;	!byte	$04	; Tile-Type = AND gate
;	!byte	$0F	; X coordinate of next tile
;	!byte	$06	; Y coordinate of next tile
;	!byte	$01	; Output of this tile goes to middle input of next tile
	; OUTPUT
;	!byte	$0F	; X coordinate of tile
;	!byte	$06	; Y coordinate of tile
;	***** Since this tile is placed in column F, it can only be of output
;       ***** type and does not have any more information associated with it.
;	***** This is the end of a level
Levels:
	; Level 1 - 2 off inputs, 1 AND gate
 ;		 X   Y  Type X   Y   Input
	!byte	$00,$05,$00,$07,$06,$00
	!byte	$00,$07,$00,$07,$06,$02
	!byte	$07,$06,$04,$05,$06,$01
	!byte	$05,$06

Levels_inc: