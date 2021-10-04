/*
 * File:   prelab.c
 * Author: Lourdes Ruiz
 *
 * Created on September 25, 2021, 5:40 PM
 * 
 * Descripcion: TIMER0 aumenta PORTC y y dos botones en PORTB aumenta PORTA
 */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000
/*
 * Constantes
 */

#define _tmr0_value 60

/*
 * Variables 
 */

/*
 * Prototipos de funciones
 */

void setup(void);


/*
 * Interrupcion 
 */

void __interrupt() isr (void)
{
    if(PIR1bits.ADIF) {
        if(ADCON0bits.CHS == 5)
            PORTA = ADRESH;
        PIR1bits.ADIF = 0;
    }
}

/*
 * Codigo Principal
 */
void main (void)
{
    setup();
    ADCON0bits.GO =1; //empezar una conversion inicial
    while(1)
    {
        // loop principal
        if (ADCON0bits.GO == 0) {
            ADCON0bits.CHS = 5;
            ADCON0bits.GO =1;
        }
            
    }
}
/*
 * Funciones
 */
void setup(void) 
{
    // Configuraciones de entradas y salidas 
    ANSEL = 0b0010000;
    ANSELH = 0;
    
    TRISA = 0;
    TRISC = 0;
    TRISE = 0b001;
    
    
    //Valores iniciales
    PORTA = 0;
    PORTC = 0;
    PORTE = 0;
    
    //Configuracion de oscilador
    OSCCONbits.IRCF = 0b0110; //4MHz
    OSCCONbits.SCS = 1; //ocsilador interno
    
    //Configuracion del ADC
    // 4MHz --> Fosc/8 se recomienda con 2us
    ADCON1bits.ADFM = 0; //justificado a la izquierda
    ADCON1bits.VCFG0 = 0; //VDD 
    ADCON1bits.VCFG1 = 0; //tierra
    
    ADCON0bits.ADCS = 0b01; //Fosc/8
    ADCON0bits.CHS = 5; //Canal 5 (AN5)
    ADCON0bits.ADON = 1; //change selection on 
    __delay_us(50);   //delay para que se cargue el capacitor en e modulo 
    
    //Configuracion de las interrupciones
    PIR1bits.ADIF = 0; //apagar la bandera de ADC
    PIE1bits.ADIE = 1; //habilitar interrupcion analogica
    INTCONbits.PEIE = 1; //interrupciones perifericas
    INTCONbits.GIE  = 1;
    
    return;
}

