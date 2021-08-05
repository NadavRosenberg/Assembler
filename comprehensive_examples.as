; examples for white spaces handeling 
add r1,r3
 	   add r1,r3
add  	  r1,r3
add r1  	  ,r3
add r1, 	   r3
add r1,r3  	  	

; examples for comments
; this is a legal comment
  	;this is illegal comment

; example for empty rows
   	

; examples for more operands then allowed
add r1, r2, r3
clr r1, r7
stop r5
.string "abcd", "ab"

; examples for less operands then allowed
add r1
clr
.data

; examples for illegal structure of a sentence
add r1, 
add r1 r2
.string "abc
.string abc"
.string 

; examples for addressing method
add r1, #-1
sub &main
red #2
dec &op0
lea &op0, r0
sub &op0, op1
add r0, &op1

; examples for labels
main: add r1, r2
	 label: add r1, r2
main  :
main:
yes:
1wrongname: add r1, r2
VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_LONG_LABEL_NAME: .data 0
.string "SOME VISIBLE TEXT !@*%#@#$^&*(!@#$%^&*()"
main: .data -2,-0
LABEL: add r1,r0
.extern LABEL
.extern futureLabel
futurelabel: add #-2, r6
.extern VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_LONG_LABEL_NAME
.extern NOTTOOLOOOOOOOOOOOOOOOOOOOLABEL
.extern 1nvalidLabel
.extern L@bel

; examples for general errors
VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_LONG_ROW: .data 0
mov VERY_VERY_VERY_VERY_VERY_VERY_VERY_VERY_LONG_OPERAND_NAME
.entry VERYVERYVERYVERYVERYVERYVERYVERYLONGOPERANDNAME
