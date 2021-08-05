; some notes
WORLD: .string "Hello World"
label:   .string "!@#$%^&*())(*&^%$#@"
	.extern label1
ABCDEFG: .data 1	 ,  	2 	,	 3  ,  4,	5 	
.entry ABCDEFG

; cmp 012,012
cmp r0, r1
cmp #37, r0
  cmp r0, label
		cmp label, label1
cmp newlabel, r0
;!@#$%^&*(); complex comment

; mov 013,13
mov #0, label
mov #-1, r0
mov r7, r1
mov r0, label00
mov label00, label
mov label00, r0
label00: add r1,r0


; add 32, r7
add #11, label
add r7, label
add label, r6

sub #-1, r0
sub r2, r3
sub veryveryveryverylonglabel, label11
sub label00, r6

lea label00, veryveryveryverylonglabel
clr yahoooooooooooooooooo
lea label11, r4
not r6
not X
X: add #379,     	r0

dec r0
dec veryveryveryverylonglabel

C: jmp label
jmp &C
; bne 12
abcd: bne X
bne &abcd

yes: .string "yes"
.entry yes


; jsr 12
jsr &A
A: jsr X

; red 13
red r4
red label

; prn 012
prn #8
prn r5
inc r7

rts
rts

.extern veryveryveryverylonglabel
stop

label11: .data 9
yahoooooooooooooooooo: .string " "
.entry yahoooooooooooooooooo
newlabel: .data -000000, +000000, +000001, -000004
anothernewlabel: .string "WEIRDLABELNAME!*123456789(1"
	
