.MODEL SMALL
.STACK 200
.DATA
a DW 00000000h
b DW 00000000h
c DW 00000000h
.CODE
.STARTUP
MOV EAX, 3
MOV a, EAX
MOV EAX, 4
MOV b, EAX
MOV EAX, 2
MOV EBX, b
ADD EAX, EBX
MOV EBX, 13
MOV EBX, 3
MUL EBX
MOV EBX, 15
MOV EBX, a
SUB EBX, EAX
MOV EAX, 14
MOV a, EAX
MOV EBX, a
MOV EBX, 0
CMP EAX, EBX
JG label_0
MOV EAX, 1
MOV a, EAX
label_2:
MOV EBX, a
MOV EBX, 6
CMP EAX, EBX
JG label_3
MOV EAX, a
MOV EBX, b
ADD EAX, EBX
MOV EBX, 13
MOV b, EAX
MOV EAX, b
MOV a, EAX
MOV EAX, 1
MOV EBX, a
ADD EAX, EBX
MOV EBX, 13
MOV a, EAX
J label_2
label_3:
J label_1
label_0:
MOV EBX, a
MOV EBX, 0
CMP EAX, EBX
JG label_4
MOV EAX, 1
MOV EBX, b
ADD EAX, EBX
MOV EBX, 13
MOV b, EAX
MOV EAX, 1
MOV EBX, a
SUB EBX, EAX
MOV EAX, 14
MOV a, EAX
MOV EAX, b
MOV EBX, 0
SUB EBX, EAX
MOV EAX, 14
MOV a, EAX
label_4:
label_1:
.EXIT
END
