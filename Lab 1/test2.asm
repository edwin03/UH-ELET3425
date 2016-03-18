*#file C:\bipom\devtools\microc\8051io.h
*5:
*6:
*10:
*11:extern register printf(), sprintf(), concat();
*12:
*#file C:\bipom\devtools\microc\8051bit.h
*26:
*27:
*31:
*35:
*39:
*41:
*#file C:\bipom\devtools\microc\8051reg.h
*6:
*7:
*8:extern register unsigned char PSW, SP, DPH, DPL, P0, P1, P2, P3,
*9:	IP, IE, TMOD, TCON, TH0, TL0, TH1, TL1, SCON, SBUF, PCON,
*10:	T2CON, TH2, TL2, RCAP2H, RCAP2L;
*11:
*#file test2.c
*4:
*5:int global_var;
*6:
*7:void main()
*8:{
*9:	int i, x, y;
*10:	serinit(9600);
*#fun main 6 ?1
main MOV R7,#6
 LCALL ?adjstk
 MOV A,#128
 MOV B,#37
 PUSH A
 PUSH B
 LCALL serinit
 DEC SP
 DEC SP
*11:	x=0;
 MOV A,#0
 MOV B,#0
 MOV R0,#-5
 LCALL ?auto0
 MOV [R0],A
 INC R0
 MOV [R0],B
*12:	y=0;
 MOV A,#0
 MOV B,#0
 INC R0
 MOV [R0],A
 INC R0
 MOV [R0],B
*13:	for(i=0; i<10 ; i++)
 MOV A,#0
 MOV B,#0
 MOV R0,#-7
 LCALL ?auto0
 MOV [R0],A
 INC R0
 MOV [R0],B
?1 EQU *
 MOV R0,#-7
 LCALL ?auto0
 MOV A,[R0]
 INC R0
 MOV B,[R0]
 MOV R3,#10
 MOV R4,#0
 LCALL ?lt
 ORL A,B
 JNZ ?3
 LJMP ?2
?3 EQU *
 SJMP ?4
?5 EQU *
 MOV R0,#-7
 LCALL ?auto0
 MOV A,[R0]
 INC R0
 MOV B,[R0]
 LCALL ?inc
 DEC R0
 MOV [R0],A
 INC R0
 MOV [R0],B
 SJMP ?1
?4 EQU *
*14:	{
*15:	global_var = i;
 MOV R0,#-7
 LCALL ?auto0
 MOV A,[R0]
 INC R0
 MOV B,[R0]
 MOV DPTR,#global_var
 MOVX [DPTR],A
 INC DPTR
 XCH A,B
 MOVX [DPTR],A
 XCH A,B
*16:	x = x+3;
 INC R0
 MOV A,[R0]
 INC R0
 MOV B,[R0]
 ADD A,#3
 XCH A,B
 ADDC A,#0
 XCH A,B
 DEC R0
 MOV [R0],A
 INC R0
 MOV [R0],B
*17:	printf("Value of x is = %d\n", x);
 MOV A,#?0+0&255
 MOV B,#?0+0/256
 PUSH A
 PUSH B
 DEC R0
 MOV A,[R0]
 INC R0
 MOV B,[R0]
 PUSH A
 PUSH B
 MOV A,#2
 MOV B,#0
 LCALL printf
 DEC SP
 DEC SP
 DEC SP
 DEC SP
*18:	y=x;
 MOV R0,#-5
 LCALL ?auto0
 MOV A,[R0]
 INC R0
 MOV B,[R0]
 INC R0
 MOV [R0],A
 INC R0
 MOV [R0],B
*19:	printf("The updated value of y is = %d\n", y);
 MOV A,#?0+20&255
 MOV B,#?0+20/256
 PUSH A
 PUSH B
 DEC R0
 MOV A,[R0]
 INC R0
 MOV B,[R0]
 PUSH A
 PUSH B
 MOV A,#2
 MOV B,#0
 LCALL printf
 DEC SP
 DEC SP
 DEC SP
 DEC SP
*20:	}
 LJMP ?5
?2 EQU *
*21:	for(;;);
?6 EQU *
 LJMP ?6
?7 EQU *
*22:}
 MOV R7,#-6
 LJMP ?exit
*#lcl y 32768 4
*#lcl x 32768 2
*#lcl i 32768 0
*#end
?0 DB 86,97,108,117,101,32,111,102,32,120,32,105,115,32,61,32
 DB 37,100,10,0,84,104,101,32,117,112,100,97,116,101,100,32
 DB 118,97,108,117,101,32,111,102,32,121,32,105,115,32,61,32
 DB 37,100,10,0
$EX:printf
$DD:global_var 2
$EX:serinit
$RL:16640
*#gbl printf 37936 0
*#gbl sprintf 5168 0
*#gbl concat 5168 0
*#gbl PSW 5504 0
*#gbl SP 5504 0
*#gbl DPH 5504 0
*#gbl DPL 5504 0
*#gbl P0 5504 0
*#gbl P1 5504 0
*#gbl P2 5504 0
*#gbl P3 5504 0
*#gbl IP 5504 0
*#gbl IE 5504 0
*#gbl TMOD 5504 0
*#gbl TCON 5504 0
*#gbl TH0 5504 0
*#gbl TL0 5504 0
*#gbl TH1 5504 0
*#gbl TL1 5504 0
*#gbl SCON 5504 0
*#gbl SBUF 5504 0
*#gbl PCON 5504 0
*#gbl T2CON 5504 0
*#gbl TH2 5504 0
*#gbl TL2 5504 0
*#gbl RCAP2H 5504 0
*#gbl RCAP2L 5504 0
*#gbl global_var 32768 0
*#gbl main 560 0
*#gbl serinit 36912 0
