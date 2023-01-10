void Pulse_LCD(unsigned int x){ RB2=1; Delay(x); RB2=0;Delay(x);}
//---------------------------
void Init_LCD(void)
{
Delay(200*Freq); TRISB=0; PORTB=0x30;
Pulse_LCD(20*Freq); Pulse_LCD(20*Freq); Pulse_LCD(20*Freq);

PORTB=0x20;  Pulse_LCD(20*Freq);

Send_Command_LCD  (0x28);
Send_Command_LCD  (0x0C);
Send_Command_LCD  (0x06);
Send_Command_LCD  (0x02);
}

//==========================================
void Send_B_LCD  (byte tmp)
{while (Check_buttons())Delay(500*Freq);
PORTB=(PORTB&0x0F)+ (tmp&0xF0);
Pulse_LCD(2*Freq);
PORTB=(PORTB&0x0F)+ (tmp<<4);
Pulse_LCD(2*Freq);
}
//---------------------------------
void Send_Byte_LCD  (byte tmp)
{Send_B_LCD(tmp);
Current_ind++;
if(Current_ind==16)Set_Coord_LCD(1,0);
//if(Current_ind==32)Set_Coord_LCD(0,0);
}
//======================================
void Send_Command_LCD  (byte tmp)
{ 
RB1=0; Send_B_LCD(tmp);RB1=1; 
Delay(250*Freq);
}
//==================
void Clr_LCD(void)
{
Set_Coord_LCD(0,0);
Show_String_LCD(str_BLANK); 
Show_String_LCD(str_BLANK); 
Set_Coord_LCD(0,0);
}
//========================================================
void Set_Coord_LCD(byte i,byte j)
{
    if(i==0){Current_ind=j;Send_Command_LCD(0x80+j);} 
    else {Current_ind=16+j;Send_Command_LCD(0xC0+j);};
	return;
}
//=========================================================
void Show_String_LCD(const char * mySTRING)
{while(*mySTRING){Send_Byte_LCD(*(mySTRING++));}; }

//=====================================

