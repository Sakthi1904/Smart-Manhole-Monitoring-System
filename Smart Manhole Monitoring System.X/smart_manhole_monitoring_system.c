#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 20000000  // Assuming 20 MHz clock frequency

// CONFIGURATION BITS
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

// Pin Definitions
#define TRIG RB0
#define ECHO RB1
#define TILT_SENSOR RB2

#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define EN PORTCbits.RC2

// UART Pin Definitions (assuming we're using TX on RC6 and RX on RC7)
#define TX RC6
#define RX RC7

// Function Prototypes
void lcd_initialize(void);
void lcd_command(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_string(const unsigned char* str, unsigned char num);

void HC_SR04_Init(void);
int HC_SR04_Read(void);
void Tilt_Sensor_Init(void);
int Tilt_Sensor_Read(void);

void UART_Init(void);
void UART_Write_Text(const char *text);
void UART_Write(char data);

void main(void)
{
    int distance;
    char waterLevel[16];
    char tiltStatus[16];

    
    

    // Pin setup
    TRISB0 = 0;  // TRIG as output
    TRISB1 = 1;  // ECHO as input
    TRISB2 = 1;  // Tilt sensor as input
    TRISD = 0x00;  // LCD data (RD0-RD7) as output
    TRISC = 0x00;  // RS, RW, EN (RC0-RC2) as output
    TRISCbits.TRISC6 = 0;  // TX as output
    TRISCbits.TRISC7 = 1;  // RX as input

    lcd_initialize();       // LCD init
    HC_SR04_Init();         // Ultrasonic sensor init
    Tilt_Sensor_Init();     // Tilt sensor init
    UART_Init();            // UART init

    while (1)
    {
        // Read tilt sensor
       
        if (Tilt_Sensor_Read())  // If tilt sensor is activated (high)
        {
            sprintf(tiltStatus, "Tilt: Opened ");  // If the tilt sensor is triggered
        }
        else
        {
            sprintf(tiltStatus, "Tilt: Closed ");  // If the tilt sensor is not triggered
        }
        
         const char* tiltStr = (Tilt_Sensor_Read()) ? "Opened" : "Closed";
        // Read distance from ultrasonic sensor
        distance = HC_SR04_Read();

        // Check the water level based on the distance
        if (distance < 30)  // If the distance is less than 30 cm, water level is high
        {
            sprintf(waterLevel, "Level: High   ");
        }
        else  // Otherwise, water level is normal
        {
            sprintf(waterLevel, "Level: Normal ");
        }

        // Display water level on line 1 of the LCD
        lcd_command(0x80);  // Move to row 1, col 1
        lcd_string("                ", 16);  // Clear the first line
        lcd_command(0x80);  // Return to row 1, col 1
        lcd_string(waterLevel, 16);  // Display water level on row 1

        // Display tilt status on line 2 of the LCD
        lcd_command(0xC0);  // Move to row 2, col 1
        lcd_string("                ", 16);  // Clear the second line
        lcd_command(0xC0);  // Return to row 2, col 1
        lcd_string(tiltStatus, 16);  // Display tilt status on row 2

        // Send data to UART
        char uartBuffer[64];
        sprintf(uartBuffer, "Tilt: %s , Distance: %d, %s\n", tiltStr, distance, waterLevel);
        UART_Write_Text(uartBuffer);  // Send the message over UART

        __delay_ms(500);  // Wait for 500 ms before updating again
    }
}

// ====== LCD FUNCTIONS ======

void lcd_initialize()
{
    lcd_command(0x38);  // 8-bit, 2 line, 5x7 characters
    lcd_command(0x06);  // Increment cursor
    lcd_command(0x0C);  // Display ON, cursor OFF
    lcd_command(0x01);  // Clear display
}

void lcd_command(unsigned char cmd)
{
    PORTD = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_data(unsigned char data)
{
    PORTD = data;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_string(const unsigned char* str, unsigned char num)
{
    unsigned char i;
    for (i = 0; i < num && str[i] != '\0'; i++)
    {
        lcd_data(str[i]);
    }
}

// ====== ULTRASONIC FUNCTIONS ======

void HC_SR04_Init()
{
    TRIG = 0;  // Set TRIG as output
}

int HC_SR04_Read()
{
    unsigned int time;
    int distance;

    TRIG = 1;
    __delay_us(10);
    TRIG = 0;

    while (!ECHO);  // Wait for echo to go HIGH
    TMR1H = 0;
    TMR1L = 0;
    T1CKPS0 = 0; T1CKPS1 = 0;
    T1OSCEN = 0;
    TMR1CS = 0;
    TMR1ON = 1;

    while (ECHO);  // Wait for echo to go LOW

    TMR1ON = 0;
    time = (TMR1L | (TMR1H << 8));
    distance = (int)(time / 294);  // Conversion for 20MHz
    return distance;
}

// ====== TILT SENSOR FUNCTIONS ======

void Tilt_Sensor_Init()
{
    // Already set as input in main
}

int Tilt_Sensor_Read()
{
    return TILT_SENSOR;  // Return the state of the tilt sensor (1 if high, 0 if low)
}

// ====== UART FUNCTIONS ======

void UART_Init(void)
{
    SPBRG = 31; // Accurate 9600 baud for 20MHz with BRGH = 1
    TXSTAbits.BRGH = 1; // Enable high-speed mode

    // Initialize UART (assuming 9600 baud rate, 8-bit, 1 stop bit)
    //SPBRG = 31;  // Set Baud Rate to 9600 for 20 MHz Fosc
    TXSTAbits.TXEN = 1;  // Enable the transmitter
    RCSTAbits.SPEN = 1;  // Enable the serial port (RX/TX)
}

void UART_Write_Text(const char *text)
{
    while (*text)  // Send each character of the text string
    {
        UART_Write(*text);
        text++;
    }
}

void UART_Write(char data)
{
    while (!TXSTAbits.TRMT);  // Wait for the transmit buffer to be empty
    TXREG = data;  // Load data into the transmit register
}