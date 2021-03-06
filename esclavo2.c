/* 
 * File:   esclavo_2.c
 * Author: Brandon Cruz
 *
 * Created on 23 de mayo de 2022, 04:19 PM
 */

//Configuracion 1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF               // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF              // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF              // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                 // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF              // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF               // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF              // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF                // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

//Configuracion 2
#pragma config BOR4V = BOR40V           // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                // Flash Program Memory Self Write Enable bits (Write protection off)


/* 
 *Librer?as
 */
#include <xc.h>
#include <stdint.h>

/* 
 *Constantes
 */
#define _XTAL_FREQ 1000000     
#define FLAG_SPI 0xFF         

/* 
 *Variables
 */
uint8_t contador;   

/* 
 *Prototipo de Funciones
 */
void setup(void);

/* 
 *Interrupciones
 */
void __interrupt() isr (void)
{
    if(INTCONbits.RBIF){              
        if(!PORTBbits.RB0)
        {            
            contador++;                
        }
        else if(!PORTBbits.RB1)
        {        
            contador--;                
        }
        INTCONbits.RBIF = 0;            
    }
    if (PIR1bits.SSPIF)
    {           
        SSPBUF = contador;              
        PIR1bits.SSPIF = 0;           
    }
    return;
}

/* 
 *Ciclo Principal
 */
void main(void) 
{
    setup();
    while(1)
    {        
    }
    return;
}

/* 
 Configuracion
 */
void setup(void)
{   
    TRISC = 0b00011000;        
    PORTCbits.RC5 = 0;
       
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Reloj interno
    
    ANSEL = 0;
    ANSELH = 0x00;              // I/O digitales
        
    TRISA = 0b00100000;         // SS como entrada
    TRISB = 0b00000011;         // RB0 y RB1 como entradas
    TRISD = 0;               // PORTD como salida
    
    PORTA = 0;               // Limpieza del PORTA
    PORTB = 0;               // Limpieza del PORTB
    PORTC = 0;               // Limpieza del PORTC
    PORTD = 0;               // Limpieza del PORTD
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.RBIE = 1;        // Habilitaci?n de interrupciones del PORTB
    INTCONbits.RBIF = 0;        // Limpieza bandera de interrupci?n del PORTB
    
    IOCBbits.IOCB0 = 1;         // Habilitaci?n de interrupci?n por cambio de estado para RB0
    IOCBbits.IOCB1 = 1;         // Habilitaci?n de interrupci?n por cambio de estado para RB1
    OPTION_REGbits.nRBPU = 0;   // Habilitaci?n de resistencias de pull-up del PORTB
    WPUBbits.WPUB0 = 1;         // Habilitaci?n de resistencia de pull-up de RB0
    WPUBbits.WPUB1 = 1;         // Habilitaci?n de resistencia de pull-up de RB1

    SSPCONbits.SSPM = 0b0100;   
    SSPCONbits.CKP = 0;         
    SSPCONbits.SSPEN = 1;       
    SSPSTATbits.CKE = 1;      
    SSPSTATbits.SMP = 0;       

    PIR1bits.SSPIF = 0;        
    PIE1bits.SSPIE = 1;         // Habilitar interrupciones de SPI
    INTCONbits.GIE = 1;         // Habilitar interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitar interrupciones de perif?ricos
}