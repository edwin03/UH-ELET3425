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
*#file test.c
*4:
*5:void main()
*6:{
*7:}
*#fun main 0 ?1
main EQU *
 RET
*#end
$RL:0
*#gbl printf 5168 0
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
*#gbl main 560 0
