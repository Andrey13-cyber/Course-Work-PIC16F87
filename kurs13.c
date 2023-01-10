#include <pic.h>

#define byte unsigned char
#define Freq 20 // =  main frequency  (MHz)

#define testbit(var, bit) ((var) & (1 << (bit)))
#define setbit(var, bit) ((var) |= (1 << (bit)))
#define clrbit(var, bit) ((var) &= ~(1 << (bit)))
__CONFIG(0x3F72);
void init_I2C(void);
byte IN_BYTE_I2C(void);
byte IN_BYTE_ACK_I2C(void);       // IN_BYTE+ACK
byte IN_BYTE_NACK_STOP_I2C(void); // IN_BYTE+NACK+STOP
void OUT_BYTE_I2C(byte);
void ACK_I2C(void);
void NACK_I2C(void);
void START_I2C(void);
void STOP_I2C(void);
void LOW_SCL_I2C(void);
void HIGH_SCL_I2C(void);
void LOW_SDA_I2C(void);
void HIGH_SDA_I2C(void);
void CLOCK_PULSE_I2C(void);
void Init_WRITE_I2C(unsigned int);
void Init_READ_I2C(unsigned int);
void Check_ACK_I2C(void);
void OUT_BYTE_PAGE_I2C(byte);
#define SCL 3
#define SDA 4
byte Ch_ACK;
#define SET_I2C_EEPROM Slave_ADR_RW_I2C = 0xA0
#define DIP_40
#define cache_size_I2C 0x40
#define BIDEN 64
#define SPACE 20

byte Current_ind;
byte Key_Press;

byte Check_buttons(void);

void Delay(unsigned int);
void Delay_Fast(byte);
void Delay_Long_Break(unsigned int);
void Delay_L(unsigned int);

void on_buttons(void);
void off_buttons(void);
void on_click_right_button();
void on_click_up_button();
void on_click_down_button();
void show_eigth_num();
static const char str_BLANK[] = "                ";
static const char RAW[] =
"=";
static const char PLUS[] =
"+";
static const char STEPEN[] =
"*8";
static const char STEPEN1[] =
"^";
static const char WAIT[] =
"WAIT";
static const char EXIT[] =
"EXIT";
static const char END[] =
"END EEPROM      ";
static const char OSN8[] =
"(8)";
static const char OSN10[] =
"(10)";
static const char AWNSWER1[] =
"WRIT TRANS?(RB4)";
static const char AWNSWER2[] =
"ONLY TRANS?(RB5)";
static const char CALCULATE[] =
"CALCULATE";
static const char CALCULATE1[] =
"CALCULATE&WRITE ";
static const char HAVE[] =
"it numb now have";
static const char TUTORIAL1[] =
"For skip tutor cl";
static const char TUTORIAL[] =
"ick any RB       ";
static const char TUTORIAL2[] =
"RB4 - drive curs";
static const char TUTORIAL3[] =
"RB5/6 inc/dec   ";
static const char TUTORIAL4[] =
"element str1 =) ";
static const char TUTORIAL5[] =
"RB7 translateStr1";
static const char TUTORIAL6[] =
"RB4(2sec)readEEPR";

void Init_LCD(void);
void Show_String_LCD(const char*);
void Send_Byte_LCD(byte);
void Clr_LCD(void);
void Send_Command_LCD(byte);
void Set_Coord_LCD(byte, byte);
void WriteSpace(void);
void WriteSpace1(void);
void ShowData(void);
void UpdateLCD(const char*, const char*);
void WaitClick(void);
void StartNum(void);
void GetTen(void);
void ShowTenNum(void);
void Clr_str(void);
void ReadEEPROM(void);
void WriteNumber(int*);
void WriteRaw(void);
void WriteSumRes(void);
void show_int(int, int);
void Manual(void);
void ClrStr(void);
long long mpow_LCD(long long, long long);
#define LENGTH_NUMBER 5
#define TIMER 100000
byte tmp_buffer_I2C, Slave_ADR_RW_I2C, tmp_I2C;
unsigned int Adr_I2C;

byte current_cursor_pos;
unsigned char str1[16] = { '0', '0', '0', '0','0',' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ',' ' };
unsigned char str2[16] = { '0', '0', '0', '0','0',' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ',' ',' ' };
int number1[5]   = { 0,0,0,0,0 };
int number2[5]   = { 0,0,0,0,0 };


//=======================

void main(void) {
    unsigned int i, j;
    byte tmp, tmp_1, tmp_2, tmp_3, tmp_4;
    current_cursor_pos = 0;
    Delay(255);
    Init_LCD();
    Manual();
Update_LCD:
    UpdateLCD(str1, str2);
    Set_Coord_LCD(0, current_cursor_pos);
    LOOP:
    WaitClick();
    tmp = Check_buttons();
    long int clock = 0;
    while (Check_buttons()) clock++;
    Delay_Long_Break(1);
    if (clock > TIMER) { 
        switch (tmp)
        {
        case 1: {
            ReadEEPROM();
        }
        default:goto LOOP;
        }

    }
    else {
        switch (tmp) {
        case 1: { // rb4
            on_click_right_button(); break;
        }
        case 2: { // rb5
            on_click_up_button(); break;
        }
        case 4: { // rb6
            on_click_down_button(); break;
        }
        case 8: {
            Clr_LCD();
            Show_String_LCD(AWNSWER1);
            Show_String_LCD(AWNSWER2);
            LOOP1:
            WaitClick();
            tmp_1 = Check_buttons();
            while (Check_buttons()); // waiting for unpress button
            Delay_Long_Break(1);
            GetTen();
            switch (tmp_1) {
            case 1: {
                Adr_I2C = 0;
                TRISD = 0; PORTD = 0;
                SET_I2C_EEPROM; TRISC = 0x9B;
                init_I2C();

            NEXT_READ1:
                Init_READ_I2C(Adr_I2C);
                tmp_2 = IN_BYTE_ACK_I2C();
                PORTD = tmp_2;
                IN_BYTE_NACK_STOP_I2C();
                if (tmp_2 == BIDEN) {
                    Adr_I2C++;
                    int tmp[5];
                    Init_READ_I2C(Adr_I2C);
                    for (i = 0; i < LENGTH_NUMBER; i++) {
                        tmp[i] = IN_BYTE_ACK_I2C();
                        PORTD = tmp[i];
                    }
                    IN_BYTE_NACK_STOP_I2C();
                    Adr_I2C += 5;
                    tmp_1=0;
                    for (i = 0; i < LENGTH_NUMBER; i++) {
                        if (tmp[i] == SPACE)tmp[i] = 0;
                        if (tmp[i] == number1[i]) continue;
                        else {
                            tmp_1 = 1; break;
                        }
                    }
                    if (tmp_1 == 1) { Adr_I2C += 56; goto NEXT_READ1; }
                    else {
                        Clr_LCD();
                        Show_String_LCD(HAVE);
                        Delay_L(666666); Delay_L(666666);
                        goto Update_LCD;
                    }
                }
                Clr_LCD();
                Show_String_LCD(CALCULATE1);
                Delay_Long_Break(1);
                Init_WRITE_I2C(Adr_I2C); 
                OUT_BYTE_PAGE_I2C(BIDEN);
                PORTD = BIDEN;
                STOP_I2C();

                Init_WRITE_I2C(Adr_I2C);
                tmp_2 = 0;
                for (i = 0; i < LENGTH_NUMBER; i++) {
                    if (number1[i] == 0 && tmp_2 == 0 && (i != LENGTH_NUMBER - 1)) { WriteSpace(); continue; }
                    OUT_BYTE_PAGE_I2C(number1[i]);
                    PORTD = number1[i];
                    tmp_2 = 1;
                }
                STOP_I2C();
                tmp_2 = 0;
                Init_WRITE_I2C(Adr_I2C);
                for (i = 0; i < LENGTH_NUMBER; i++) {
                    if (number2[i] == 0 && tmp_2 == 0 && (i != LENGTH_NUMBER - 1)) { WriteSpace(); continue; }
                    OUT_BYTE_PAGE_I2C(number2[i]);
                    PORTD = number2[i];
                    tmp_2 = 1;
                }
                STOP_I2C();
                WriteRaw();
                tmp_2 = 0;
                Init_WRITE_I2C(Adr_I2C);
                for (i = 0; i < LENGTH_NUMBER; i++) {
                    if (number1[i] == 0 && tmp_2 == 0 && (i != LENGTH_NUMBER - 1)) { WriteSpace1(); continue; }
                    tmp_2 = 1;
                    OUT_BYTE_PAGE_I2C('0'+number1[i]);
                    PORTD = '0'+number1[i];
                    for (j = 0; j < 2; j++) {
                        OUT_BYTE_PAGE_I2C(STEPEN[j]);
                        PORTD = STEPEN[j];
                    }
                    OUT_BYTE_PAGE_I2C(STEPEN1[0]);
                    PORTD = STEPEN1[0];
                    OUT_BYTE_PAGE_I2C('0'+(4-i));
                    PORTD = '0' + (4 - i);
                    if (i != 4) {
                        OUT_BYTE_PAGE_I2C(PLUS[0]);
                        PORTD = PLUS[0];
                    }
                    else {
                        STOP_I2C();
                        WriteRaw();
                    }
                }
                long long data = 0;
                unsigned int st = 1;
                for (int k = 4; k >= 0; k--) {
                    data = number1[k] * st; 
                    st *= 8;
                    show_int(data, k);

                }
                goto Update_LCD;
            }
            case 2: {
                goto Update_LCD;
            }
            default :goto LOOP1;
            }
        }
        default: goto LOOP;
        }
       
    }
    goto Update_LCD;
}
//============================
void GetTen(void) {
    Clr_LCD();
    Show_String_LCD(CALCULATE);
    Delay_Long_Break(1);
    unsigned int st = 1;
    unsigned int result = 0;
    for (int i = 4; i >= 0; i--) {
        result += number1[i] * st;
        st *= 8;
    }
    number2[0] = result / 10000;
    number2[1] = result % 10000 / 1000;
    number2[2] = result % 1000 / 100;
    number2[3] = result % 100 / 10;
    number2[4] = result % 10;
    ShowTenNum();
}
void WriteRaw(void) {
    Init_WRITE_I2C(Adr_I2C);
    OUT_BYTE_PAGE_I2C('=');
    PORTD = '=';
    STOP_I2C();
}
void WriteSumRes(void) {

}

void ClrStr(void) {
    for (char i = 0; i < 16; i++) {
        str1[i] = ' ';
        str2[i] = ' ';
    }
}

void Manual(void) {
Main_LOOP:

    Clr_LCD(); Show_String_LCD(TUTORIAL1);
    Show_String_LCD(TUTORIAL);

    Delay_Long_Break(Freq); if (Key_Press)goto EXIT;
    Clr_LCD(); Show_String_LCD(TUTORIAL2);
    Show_String_LCD(TUTORIAL5);


    Delay_Long_Break(Freq); if (Key_Press)goto EXIT;
    Clr_LCD(); Show_String_LCD(TUTORIAL3);
    Show_String_LCD(TUTORIAL4);

    Delay_Long_Break(Freq); if (Key_Press)goto EXIT;
    Clr_LCD(); Show_String_LCD(TUTORIAL6);

    Delay_Long_Break(Freq);
    if (!Key_Press)goto Main_LOOP;
EXIT:;
}

void show_int(int data, int step) {
    Init_WRITE_I2C(Adr_I2C);
    if (data == 0 && step!=4) { for (char j = 0; j < (4-step); j++) WriteSpace(); }
    int size = 0, td = data;
    while (td > 0) { td /= 10; size++; }
    if (data == 0) {
        OUT_BYTE_PAGE_I2C('0');
        PORTD = ('0');
    }

    while (td < size) {
        if (td == 0) {
            OUT_BYTE_PAGE_I2C('0' + (data / mpow_LCD(10, size - 1)));
            PORTD = ('0' + (data / mpow_LCD(10, size - 1)));
        }
        else {
            OUT_BYTE_PAGE_I2C('0' + (data / mpow_LCD(10, size - 1 - td) % 10));
            PORTD = ('0' + (data / mpow_LCD(10, size - 1 - td) % 10));
        }
        td++;
    }
    //if (data == 0 && step!=0) WriteSpace();

        if (step != 0) {
            OUT_BYTE_PAGE_I2C('+');
            PORTD = ('+');
        }
        else {
            OUT_BYTE_PAGE_I2C('=');
            PORTD = ('=');
        }
    STOP_I2C();
}

long long mpow_LCD(long long data, long long y) {
    long long tmp = 1;
    for (long long i = 0; i < y; i++) tmp *= data;
    return tmp;
}


void ReadEEPROM(void) {
    byte tmp_3, tmp, tmp_1, tmp_2;
    int i, j;
    char ind_Str =1;
    unsigned int start_prev_log;
    off_buttons();
    Clr_LCD();
    Show_String_LCD(WAIT);
    Delay_Long_Break(1);
    Adr_I2C = 0;
    TRISD = 0;
    PORTD = 0;
    SET_I2C_EEPROM;
    TRISC = 0x9B;
    init_I2C();
START_PREV_READ2:;
    start_prev_log = Adr_I2C;
    Init_READ_I2C(Adr_I2C);
    tmp_3 = IN_BYTE_ACK_I2C();
    PORTD = tmp_3;
    IN_BYTE_NACK_STOP_I2C();

    if (tmp_3 == BIDEN)
    {
        Clr_str();
        Adr_I2C++;
        Init_READ_I2C(Adr_I2C);
        for (i = 0; i < LENGTH_NUMBER; i++) {
            number1[i] = IN_BYTE_ACK_I2C();
            PORTD = number1[i];
        }
        for (i = 0; i < LENGTH_NUMBER; i++) {
            number2[i] = IN_BYTE_ACK_I2C();
            PORTD = number2[i];
        }
        IN_BYTE_NACK_STOP_I2C();
        Adr_I2C += 9;
      
        unsigned int prev_adr = Adr_I2C;
        goto EXIT_TRANS;
    PREV_LOGS:
        Clr_str();
        ind_Str = 0;
        i = 0;
        while (number1[i] == SPACE) i++;
        tmp = i;
        for (j = 0; j < (LENGTH_NUMBER - i); j++) { str1[j] = '0' + number1[tmp]; tmp++; }
        goto START_READ;
    NEXT_LOGS:
        Clr_str();
        ind_Str = 1;
        j = 0;
    START_READ:
        Adr_I2C = (ind_Str == 0 ? prev_adr : Adr_I2C);
        Init_READ_I2C(Adr_I2C);
        for (i = j; i < 16; i++) {
            tmp = IN_BYTE_ACK_I2C();
            PORTD = tmp;
            if (((Adr_I2C - prev_adr) > 52)) break;
            if (tmp != SPACE && tmp != 0x27 && tmp !=BIDEN) str1[i] = tmp; 
            else i=i-1;
            Adr_I2C++;
        }
        char ind = (i == 16 ? 0 : i);
        if (ind == i) {
            for (j = ind; j < 16; j++) {
                tmp = IN_BYTE_ACK_I2C();
                PORTD = tmp;
                if (((Adr_I2C - prev_adr) > 52)) break;
                if (tmp != SPACE && tmp != 0x27 && tmp != BIDEN) str1[j] = tmp;
                else j--;
                Adr_I2C++;
            }
            for (j = 0; j < 16; j++) {
                tmp = IN_BYTE_ACK_I2C();
                PORTD = tmp;
                if (((Adr_I2C - prev_adr) > 52)) break;
                if (tmp != SPACE&& tmp != 0x27 && tmp != BIDEN) str2[j] = tmp;
                else j--;
                 Adr_I2C++;
            }
        }
        else {
            for (j = 0; j < 16; j++) {
                tmp = IN_BYTE_ACK_I2C();
                PORTD = tmp;
                if (((Adr_I2C - prev_adr) > 52)) break;
                if (tmp != SPACE&& tmp != 0x27 && tmp != BIDEN) str2[j] = tmp;
                else j--;
                Adr_I2C++;
            }
        }
        IN_BYTE_NACK_STOP_I2C();
        if(ind_Str ==1){
            i = 0;
            while (number2[i] == SPACE)i++;
            tmp = i;
            for (j = 11; j < (16 - i); j++) { str2[j] = '0' + number2[tmp]; tmp++;}
        }
        UpdateLCD(str1, str2); goto WAITING1;
    EXIT_TRANS:
        ClrStr();
        i = 0;
        while (number1[i] == SPACE) i++;
        tmp = i;
        for (j = 0; j < (LENGTH_NUMBER - i); j++) { str1[j] = '0' + number1[tmp]; tmp++; }
        i = 0;
        while (number2[i] == SPACE) i++;
        tmp = i;
        for (j = 0; j < (LENGTH_NUMBER - i); j++) { str2[j] = '0' + number2[tmp]; tmp++; }
        UpdateLCD(str1, str2);
    WAIT:
        on_buttons();
        Delay_Long_Break(1);
        WaitClick();
        tmp = Check_buttons();
        Delay_Long_Break(1);
        off_buttons();
        switch (tmp)
        {
        case 1: goto EXIT_READ;
        case 2: {
        WAITING1:
            on_buttons();
            Delay_Long_Break(1);
            WaitClick();
            tmp = Check_buttons();
            Delay_Long_Break(1);
            off_buttons();
            switch (tmp)
            {
            case 2: if (ind_Str == 1) goto PREV_LOGS; else goto NEXT_LOGS;
            case 1: ind_Str = 1; goto EXIT_TRANS;
            default:goto WAITING1;
            }
        }
        case 4: Adr_I2C = start_prev_log-62; goto START_PREV_READ2; //prev text
        case 8: Adr_I2C = start_prev_log+62; goto START_PREV_READ2; //next text
        default:goto WAIT;
        }
    }
    else {
      
        Clr_LCD();
        Show_String_LCD(END);
        Show_String_LCD(EXIT);
        WaitClick();
        goto EXIT_READ;
    }
EXIT_READ:
StartNum();
on_buttons();
}

void Clr_str(void) {
    for (char i = 0; i < 16; i++) {
        str2[i] = ' ';
        str1[i] = ' ';
    }
}

void ShowTenNum(void) {
    for (char i = 0; i < 5; i++) {
        str2[i] = 0x30 + number2[i];
    }
}
void ShowData(void) {
    char check = 0;
    for (int tmp_3 = 0; tmp_3 < LENGTH_NUMBER; tmp_3++) {
        if (number2[tmp_3] == 0 &&check==0&& (tmp_3 != LENGTH_NUMBER - 1)) continue;
        Send_Byte_LCD('0' + number2[tmp_3]);
        if (check == 0) check = 1;
    };
}
void WriteSpace1(void) { for (char j = 0; j < LENGTH_NUMBER+1; j++) { OUT_BYTE_PAGE_I2C(SPACE); PORTD = SPACE; } }
void WriteSpace(void) { OUT_BYTE_PAGE_I2C(SPACE); PORTD = SPACE; }
byte Check_buttons(void)
{
    byte tmp, tmp_PORTB = PORTB, tmp_TRISB = TRISB;
    TRISB &= 0x0F;
    PORTB |= 0xF0;
    tmp = PORTB ^ 0xFF;
    PORTB = tmp_PORTB;
    TRISB = tmp_TRISB;
    return (tmp >> 4);
}

void on_buttons(void) { PORTB = 0xF0; }

void off_buttons(void) { PORTB = 0; }

void on_click_right_button()
{
    if (current_cursor_pos < LENGTH_NUMBER - 1)
    {
        current_cursor_pos++;
    }
    else if (current_cursor_pos == LENGTH_NUMBER - 1)
    {
        current_cursor_pos = 0;
    }
}

void on_click_up_button()
{
    number1[current_cursor_pos]++;
    if (number1[current_cursor_pos] == 8)number1[current_cursor_pos] = 0;
    show_eigth_num();
}

void on_click_down_button()
{
    if (number1[current_cursor_pos] == 0)number1[current_cursor_pos] = 7;
    else number1[current_cursor_pos]--;
    show_eigth_num();
}


void show_eigth_num()
{
    for (char i = 0; i < LENGTH_NUMBER; i++)
    {
        str1[i] = '0' + number1[i];   
    }
}

void StartNum(void) {
    Clr_str();
    for (char i = 0; i < LENGTH_NUMBER; i++) {
        number1[i] = 0;
        number2[i] = 0;
        str1[i] = '0';
        str2[i] = '0';
    }
}

void WaitClick(void) {
LOOP:
    Delay_Long_Break(Freq);
    if (Key_Press)
        goto Select_BUTTONS;
    Delay_Long_Break(Freq);
    if (!Key_Press)
        goto LOOP;

Select_BUTTONS:;
}

void UpdateLCD(const char* str_1, const char* str_2) {
    Set_Coord_LCD(0, 0);
    Show_String_LCD(str_BLANK);

    Show_String_LCD(str_BLANK);
    Set_Coord_LCD(0, 0);

    for (char i = 0; i < 16; i++) {
        Send_Byte_LCD(str1[i]);
    }
    Set_Coord_LCD(1, 0);
    for (char i = 0; i < 16; i++) {
        Send_Byte_LCD(str2[i]);
    }
}

void Delay(unsigned int tmp) // (tmp=1000) ~  11 mS (4MHz)
{
    while (tmp--)  return;
}

void Pulse_LCD(unsigned int x) {
    unsigned int x1 = x, x2 = x;
    RB2 = 1; while (x1--); RB2 = 0; while (x2--);
}
//---------------------------
void Init_LCD(void)
{
    #asm
        Reg_1 EQU 20h
        Reg_2 EQU 21h
        Reg_3 EQU 22h
        Time  EQU 23h
        ORG 0
        #endasm
        Delay_L(200 * Freq); TRISB = 0; PORTB = 0x30;
    Pulse_LCD(20 * Freq); Pulse_LCD(20 * Freq); Pulse_LCD(20 * Freq);

    PORTB = 0x20;  Pulse_LCD(20 * Freq);

    Send_Command_LCD(0x28); //4p, 2 str, 5x8
    Send_Command_LCD(0x0F); //disp on, curs on, blink on
    Send_Command_LCD(0x06); // inc
    Send_Command_LCD(0x02); //go home
}

void Send_B_LCD(byte tmp)
{
    while (Check_buttons())Delay(500 * Freq);
    PORTB = (PORTB & 0x0F) + (tmp & 0xF0);
    Pulse_LCD(2 * Freq);
    PORTB = (PORTB & 0x0F) + (tmp << 4);
    Pulse_LCD(2 * Freq);
}
//---------------------------------
void Send_Byte_LCD(byte tmp)
{
    Send_B_LCD(tmp);
    Current_ind++;
    if (Current_ind == 16)Set_Coord_LCD(1, 0);
    //if(Current_ind==32)Set_Coord_LCD(0,0);
}

void Set_Coord_LCD(byte i, byte j)
{
    if (i == 0) { Current_ind = j; Send_Command_LCD(0x80 + j); }
    else { Current_ind = 16 + j; Send_Command_LCD(0xC0 + j); };
    return;
}

void Show_String_LCD(const char* mySTRING)
{
    while (*mySTRING) { Send_Byte_LCD(*(mySTRING++)); };
}

void Send_Command_LCD(byte tmp)
{
    RB1 = 0; Send_B_LCD(tmp); RB1 = 1;
    Delay(250 * Freq);
}
//==================
void Clr_LCD(void)
{
    Set_Coord_LCD(0, 0);
    Show_String_LCD(str_BLANK);

    Show_String_LCD(str_BLANK);
    Set_Coord_LCD(0, 0);

}
void Delay_Fast(byte i)
{while (i--);}

void Delay_L(unsigned int tmp) // (tmp=1000) ~  3 mS (20MHz)
{
    byte tmp_1 = PORTB, tmp_2 = TRISB;
    Key_Press = 0;
    TRISB &= 0x0F;
    PORTB |= 0xF0;

    while (tmp--)
    {
        if ((PORTB & 0xF0) != 0xF0)
        {
            Key_Press = 1;
            break;
        }
    }
    PORTB = tmp_1;
    TRISB = tmp_2;

    return;
}

byte Indic = 1 << 4;
void Delay_Long_Break(unsigned int k)
{
    while (k--)
    {
        PORTC = Indic; // PORTC+1+128;
        if ((Indic & 128) == 0)
            Indic += 128;
        else
        {
            Indic -= 128;

            if (Indic == 1)
                Indic = 1 << 4;
            else
            {

                if (Indic == 0)
                    Indic = 1;

                if (Indic == (127 - (1 << 5) - (1 << 6) - (1 << 2)))
                    Indic = 0;

                if (Indic == ((1 << 5)))
                    Indic = 127 - (1 << 5) - (1 << 6) - (1 << 2);

                if (Indic == (1 + (1 << 5)))
                    Indic = (1 << 5);

                if (Indic == (1 + (1 << 1) + (1 << 6)))
                    Indic = 1 + (1 << 5);

                if (Indic == (1 + (1 << 3)))
                    Indic = 1 + (1 << 1) + (1 << 6);

                if (Indic == ((1 << 2) + (1 << 3)))
                    Indic = 1 + (1 << 3);

                if (Indic == (127 - (1 << 2) - (1 << 5)))
                    Indic = (1 << 3) + (1 << 2);

                if (Indic == (1 << 4))
                    Indic = 127 - (1 << 2) - (1 << 5);
            }
        }
        Delay_L(65000);
        if (Key_Press)break;
    }
}

// Simple I2C    ===================================
//==================================================

void init_I2C()
{
    RC4 = 0;
    /* set the SDA pin LOW. The SDA pin is then set
          HIGH by the TRIS command
    */
    TRISC3 = 0; // set SCL line to an OUPUT
    TRISC4 = 0; // set SDA line to an INPUT just to be sure!
}

//=======================================================
//=======================================================

void Init_WRITE_I2C(unsigned int Adr_begin)
{
    Adr_I2C = Adr_begin;
rep:
    START_I2C();
    Slave_ADR_RW_I2C &= 0xFE;
    OUT_BYTE_I2C(Slave_ADR_RW_I2C);
    if (Ch_ACK)
    {
        STOP_I2C();
        goto rep;
    }
    OUT_BYTE_I2C(Adr_begin >> 8);
    if (Ch_ACK)
    {
        STOP_I2C();
        goto rep;
    }
    OUT_BYTE_I2C(Adr_begin);
    if (Ch_ACK)
    {
        STOP_I2C();
        goto rep;
    }
}
//------------------------------------
void Init_READ_I2C(unsigned int Adr_begin)
{
    Init_WRITE_I2C(Adr_begin);
    START_I2C();
    Slave_ADR_RW_I2C |= 1;
    OUT_BYTE_I2C(Slave_ADR_RW_I2C);
}

//--------------------------------------
void LOW_SCL_I2C(void)
{ // clrbit(PORTC,SCL);
    clrbit(PORTC, SCL);
    Delay_Fast(5);
}
//-----------------------------------------
void HIGH_SCL_I2C(void)
{ // setbit(TRISC,SCL)
    setbit(PORTC, SCL);
    Delay_Fast(5);
}
//---------------------------------------
void LOW_SDA_I2C(void)
{
    clrbit(PORTC, SDA);
    clrbit(TRISC, SDA);
    Delay_Fast(5);
}
//-----------------------------------------
void HIGH_SDA_I2C(void)
{
    setbit(TRISC, SDA);
    Delay_Fast(5);
}
//---------------------------------------
void CLOCK_PULSE_I2C(void)
{
    HIGH_SCL_I2C();
    LOW_SCL_I2C();
}
//--------------------------------------
void STOP_I2C(void)
{
    LOW_SDA_I2C();
    LOW_SCL_I2C();
    HIGH_SCL_I2C();
    HIGH_SDA_I2C();
    LOW_SCL_I2C();
}
//----------------------------------------
void START_I2C(void)
{
    HIGH_SDA_I2C();
    HIGH_SCL_I2C();
    LOW_SDA_I2C();
    LOW_SCL_I2C();
}
//---------------------------------------
void ACK_I2C(void)
{
    LOW_SDA_I2C();
    CLOCK_PULSE_I2C();
}
//---------------------------------------
void NACK_I2C(void)
{
    HIGH_SDA_I2C();
    CLOCK_PULSE_I2C();
}
//----------------------------------
void Check_ACK_I2C(void)
{
    HIGH_SCL_I2C();
    if (testbit(PORTC, SDA))
        Ch_ACK = 1;
    else
        Ch_ACK = 0;
    LOW_SCL_I2C();
}
//---------------------------------------
void OUT_BYTE_I2C(byte t)
{
    byte tmp;
    tmp = 8;
    while (tmp--)
    {
        if (t & 0x80)
            HIGH_SDA_I2C();
        else
            LOW_SDA_I2C();
        CLOCK_PULSE_I2C();
        t += t;
    }
    HIGH_SDA_I2C();
    Check_ACK_I2C();
}
//----------------------------------------
//-----------------------------------------------
byte IN_BYTE_I2C(void)
{
    byte t, tmp = 8;
    t = 0;
    HIGH_SDA_I2C();
    while (tmp--)
    {
        t += t;
        HIGH_SCL_I2C();
        if (testbit(PORTC, SDA))
            t++;
        LOW_SCL_I2C();
    }
    return (t);
}
//----------------
byte IN_BYTE_NACK_STOP_I2C(void)
{
    byte t;
    t = IN_BYTE_I2C();
    NACK_I2C();
    STOP_I2C();
    return (t);
}
//------------------
byte IN_BYTE_ACK_I2C(void)
{
    byte t;
    t = IN_BYTE_I2C();
    ACK_I2C();
    return (t);
}
//-------------------------------------------
void OUT_BYTE_PAGE_I2C(byte tmp)
{
    OUT_BYTE_I2C(tmp);
    Adr_I2C++;
    if ((cache_size_I2C - 1) & Adr_I2C)
        return;
    STOP_I2C();
    Init_WRITE_I2C(Adr_I2C);
    return;
}

//================================