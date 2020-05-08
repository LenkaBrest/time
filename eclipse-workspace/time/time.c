/**
 * @file time.c
 * @brief Biblioteka sa vremenskim funkcijama
 * @author Lenka
 * @date 07/05/2020
 * @version 1.0
 */
#include <avr/io.h>
#include <avr/interrupt.h>

///sistemsko vreme
unsigned long sys_time = 0;

ISR(TIMER0_COMPA_vect)
{
	sys_time++;
}

/**
 * Funkcija inicijalizuje sistemsko vrme i generise prekid na svakih 1ms
 */
void initTime()
{
	TCCR0A = 0x02; //Timer0 u CTC modu

#if F_CPU > 2000000
#error "Frekvencija takta mora biti manja od 20MHz!"
#endif

	unsigned long n = F_CPU / 1000;
	unsigned char clksel=1;

	while(n > 255){
		n /= 8;
		clksel++;
	}

	TCCR0B = clksel;
	OCR0A = (unsigned char)(n & 0xff) - 1;
	TIMSK0 = 0x02;

	sei();

}

/**
 * Funkcija vraca proteklo vreme
 * @return sistemsko vreme
 */
unsigned long millis()
{
	unsigned long tmp;

	cli();
	tmp = sys_time;
	sei();

	return tmp;
}

/**
 * Funkcija generise kasnjenje odredjenog trajanja
 * @param d Trajanje kasnjenja u ms
 */
void delay(unsigned long d)
{
	unsigned long t0 = millis();

	while(millis() - t0 < d);
}

