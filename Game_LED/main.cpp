#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <util/delay.h>
#include "LCD_HD44780.h"

 
#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif
#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
#endif



char* getKey()
{
	DDRC=0;
	PORTC=255;


	char* temp="z";

	if (!bit_is_set(PINC,7))
		temp="g";

	if (!bit_is_set(PINC,6))
		temp="d";

	return temp;
}

void dodaj_do_ekranu(int i, char znak, char (&ekran)[32])
{

	if(i>=16)
	{
		ekran[16]=znak;
		for(int k=0;k<16;k++)
		{
			ekran[k]=ekran[k+1];
		}
	}
	else
	ekran[i]=znak;
}


void inicjalizacja_rzedu(char (&ekran)[32])
{
	for(int i=0; i<30; i++)
	ekran[i]=' ';
}

void komunikat(int Punkty)
{
	char* symbol;
	LCD_HD44780::clear();
	LCD_HD44780::goTo(0, 0);
	LCD_HD44780::writeText("Game over");
	_delay_ms(10000);
	LCD_HD44780::clear();
	LCD_HD44780::goTo(0, 0);
	LCD_HD44780::writeText("Your points:");
	char *text;
	char znaki[16];
	text=dtostrf(Punkty,1,0,znaki);
	LCD_HD44780::goTo(0, 1);
	LCD_HD44780::writeText(znaki);
	_delay_ms(5000);
	_delay_ms(5000);
	_delay_ms(5000);
	_delay_ms(5000);
	_delay_ms(5000);
	_delay_ms(5000);
	_delay_ms(5000);
	LCD_HD44780::goTo(0, 0);
	LCD_HD44780::writeText("Push button");
	LCD_HD44780::goTo(0, 1);
	LCD_HD44780::writeText("to restart");
	while(1)
	{
		symbol=getKey();
		if(symbol[0]=='g'||symbol[0]=='d')
			break;
	}
}

int main()
{
	while(1)
	{
	//srand(3);
	int i=0, j=0;
	int Odstep=0;
	int gora=0;
	int ptak=0;
	int Punkty=0;
	int Poziom_trudnosci=50;
	int Poziom_trudnosci_przeszkod=3;
	int Ostatnia_przeszkoda;
	int Przeszkody_z_rzedu = 0;
	double Q;
	char ekran1[32];
	char ekran2[32];
	char* symbol;

	LCD_HD44780::init();
	LCD_HD44780::goTo(0, 0);
	LCD_HD44780::writeText("Push button");
	LCD_HD44780::goTo(0, 1);
	LCD_HD44780::writeText("to start");
	while(1)
	{
		symbol=getKey();
		if(symbol[0]=='g'||symbol[0]=='d')
			break;
	}
	LCD_HD44780::clear();
	LCD_HD44780::goTo(0, 0);
	LCD_HD44780::writeText("Watch out!");
	_delay_ms(6000);

	inicjalizacja_rzedu(ekran1);
	inicjalizacja_rzedu(ekran2);
	dodaj_do_ekranu(0,'>', ekran2);

	while(1)
	{
		Q=rand()/double(RAND_MAX);
		char* przeszkoda="|";
		symbol=getKey();

		char *text;
		char znaki[16];
		text=dtostrf(Q,1,3,znaki);

		if(Poziom_trudnosci<40)
		{
			Poziom_trudnosci_przeszkod=2;
			if(Poziom_trudnosci<30)
			{
				Poziom_trudnosci_przeszkod=1;
				if(Poziom_trudnosci<20)
				{
					Poziom_trudnosci_przeszkod=0;
				}
			}
		}

		if(symbol[0]=='g')
		{
			gora=1;
		}

		if(symbol[0]=='d')
		{
			gora=0;
		}

		if(gora==1)
		{
			if(ekran2[0]=='>')
			{
				if(ekran1[0]==' ')
				{
					ekran1[0]=ekran2[0];
					ekran2[0]=' ';
					ptak=1;
				}
				else
				{
					komunikat(Punkty);
					break;
				}
			}
		}
		else
		{
			if(ekran1[0]=='>')
			{
				if(ekran2[0]==' ')
				{
					ekran2[0]=ekran1[0];
					ekran1[0]=' ';
					ptak=0;
				}
				else
				{
					komunikat(Punkty);
					break;
				}
			}
		}



		if(i>Poziom_trudnosci)
		{
			i=0;
			if(Odstep > Poziom_trudnosci_przeszkod)
			{
				if(Q > 0.5)
				{
					dodaj_do_ekranu(16,przeszkoda[0], ekran1);
					Ostatnia_przeszkoda=1;
					dodaj_do_ekranu(16,' ', ekran2);
				}
				else
				{
					dodaj_do_ekranu(16,przeszkoda[0], ekran2);
					Ostatnia_przeszkoda=0;
					dodaj_do_ekranu(16,' ', ekran1);
				}
				Odstep=0;
			}
			else
			{
				if(Ostatnia_przeszkoda==1 && Q>0.5 && Przeszkody_z_rzedu<2 && Odstep==0)
				{
				dodaj_do_ekranu(16,'|', ekran1);
				dodaj_do_ekranu(16,' ', ekran2);
				Przeszkody_z_rzedu++;
				Ostatnia_przeszkoda=1;
				}
				else if(Ostatnia_przeszkoda==0 && Q<0.5 && Przeszkody_z_rzedu<2 && Odstep==0)
				{
				dodaj_do_ekranu(16,' ', ekran1);
				dodaj_do_ekranu(16,'|', ekran2);
				Przeszkody_z_rzedu++;
				Ostatnia_przeszkoda=0;
				}
				else
				{
					dodaj_do_ekranu(16,' ', ekran1);
					dodaj_do_ekranu(16,' ', ekran2);
					Odstep++;
					Przeszkody_z_rzedu=0;
				}

			}
		}



		if(ptak==1)
		{
			if(ekran1[0]=='|')
			{
				komunikat(Punkty);
				break;
			}
			else
			dodaj_do_ekranu(0,'>', ekran1);
		}
		else
		{
			if(ekran2[0]=='|')
			{
				komunikat(Punkty);
				break;
			}
			else
			dodaj_do_ekranu(0,'>', ekran2);
		}

		LCD_HD44780::goTo(0, 0);
		LCD_HD44780::writeText(ekran1);

		LCD_HD44780::goTo(0, 1);
		LCD_HD44780::writeText(ekran2);

		_delay_ms(100);
		i++;
		Punkty++;
		j++;
		if(j%100==0 && Poziom_trudnosci>18)
			Poziom_trudnosci--;

	}
	}
}
