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
	DDRC=0b00001111;
	DDRA=0b00000011;


	char* temp="z";

	PORTC=0b11110001;

	if (bit_is_set(PINC,4))
		temp="p";
	if (bit_is_set(PINC,5))
		temp="i";
	if (bit_is_set(PINC,6))
		temp="7";
	if (bit_is_set(PINC,7))
		temp="4";
	if (bit_is_set(PINA,7))
		temp="1";
	if (bit_is_set(PINA,6))
		temp=" ";

	PORTC=0b11110010;

	if (bit_is_set(PINC,4))
		temp="!";
	if (bit_is_set(PINC,5))
		temp="k";
	if (bit_is_set(PINC,6))
		temp="8";
	if (bit_is_set(PINC,7))
		temp="5";
	if (bit_is_set(PINA,7))
		temp="2";
	if (bit_is_set(PINA,6))
		temp="0";

	PORTC=0b11110100;

	if (bit_is_set(PINC,4))
		temp="u";
	if (bit_is_set(PINC,5))
		temp="q";
	if (bit_is_set(PINC,6))
		temp="9";
	if (bit_is_set(PINC,7))
		temp="6";
	if (bit_is_set(PINA,7))
		temp="3";
	if (bit_is_set(PINA,6))
		temp=".";

	PORTC=0b11111000;

	if (bit_is_set(PINC,4))
		temp="c";
	if (bit_is_set(PINC,5))
		temp="/";
	if (bit_is_set(PINC,6))
		temp="*";
	if (bit_is_set(PINC,7))
		temp="-";
	if (bit_is_set(PINA,7))
		temp="+";
	if (bit_is_set(PINA,6))
		temp="=";

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


int main()
{
	DDRD=255;
	DDRC=0b00001111;
	LCD_HD44780::init();
	int i,j,ulamek,minus,licznik_ekran;
	float liczba,liczba2;
	float tab[16];
	char ekran[32];

	while(1)
	{
		char* symbol;
		tab[0]=0;
		i=0;
		j=0;
		ulamek=0;
		liczba=0;
		liczba2=0;
		minus=1;
		licznik_ekran=1;

		while(1)
		{

			symbol=getKey();
			if(symbol[0]=='0'|| symbol[0]=='1'|| symbol[0]=='2'|| symbol[0]=='3'|| symbol[0]=='4'|| symbol[0]=='5'|| symbol[0]=='6'|| symbol[0]=='7'|| symbol[0]=='8'|| symbol[0]=='9' )
			{

				if(i==0)
				{
					for(int k=0; k<32; k++)
					ekran[k]=' ';
					tab[0]=0;
					LCD_HD44780::clear();
				}

				if(ulamek==0)
				{
					liczba2=symbol[0]-'0';
					liczba=(liczba*10)+liczba2;
				}
				else
				{
					liczba2=symbol[0]-'0';
					liczba2=liczba2/ulamek;
					liczba=liczba+liczba2;
					ulamek=ulamek*10;
				}


				dodaj_do_ekranu(i,symbol[0],ekran);
				i++;
				_delay_ms(1000);
			}

			if(symbol[0]=='+')
			{
				j--;
				dodaj_do_ekranu(i,symbol[0],ekran);
				i++;
				dodaj_do_ekranu(i,' ',ekran);
				i++;
				liczba=tab[j-1]+tab[j];
				tab[j-1]=liczba;
				tab[j]=0;
				liczba=0;
				_delay_ms(1000);
			}

			if(symbol[0]=='-')
			{
				j--;
				dodaj_do_ekranu(i,symbol[0],ekran);
				i++;
				dodaj_do_ekranu(i,' ',ekran);
				i++;
				liczba=tab[j-1]-tab[j];
				tab[j-1]=liczba;
				tab[j]=0;
				liczba=0;
				_delay_ms(1000);
			}

			if(symbol[0]=='*')
			{
				j--;
				dodaj_do_ekranu(i,symbol[0],ekran);
				i++;
				dodaj_do_ekranu(i,' ',ekran);
				i++;
				liczba=tab[j-1]*tab[j];
				tab[j-1]=liczba;
				tab[j]=0;
				liczba=0;
				_delay_ms(1000);
			}

			if(symbol[0]=='/')
			{
				j--;
				dodaj_do_ekranu(i,symbol[0],ekran);
				i++;
				dodaj_do_ekranu(i,' ',ekran);
				i++;
				liczba=tab[j-1]/tab[j];
				tab[j-1]=liczba;
				tab[j]=0;
				liczba=0;
				_delay_ms(1000);
			}

			if(symbol[0]=='p')
			{
				j--;
				dodaj_do_ekranu(i,'%',ekran);
				i++;
				dodaj_do_ekranu(i,' ',ekran);
				i++;
				liczba=tab[j-1]*tab[j]/100;
				tab[j-1]=liczba;
				tab[j]=0;
				liczba=0;
				_delay_ms(1000);
			}

			if(symbol[0]=='q')
			{
				j--;
				dodaj_do_ekranu(i,'s',ekran);
				i++;
				dodaj_do_ekranu(i,'q',ekran);
				i++;
				dodaj_do_ekranu(i,'r',ekran);
				i++;
				dodaj_do_ekranu(i,'t',ekran);
				i++;
				dodaj_do_ekranu(i,' ',ekran);
				i++;

				liczba=sqrt(tab[j]);
				tab[j]=liczba;
				j++;
				liczba=0;
				_delay_ms(1000);
			}

			if(symbol[0]=='k')
			{
				j--;
				dodaj_do_ekranu(i,'^',ekran);
				i++;
				dodaj_do_ekranu(i,'2',ekran);
				i++;
				dodaj_do_ekranu(i,' ',ekran);
				i++;

				liczba=pow(tab[j],2);
				tab[j]=liczba;
				j++;
				liczba=0;
				_delay_ms(1000);
			}

			if(symbol[0]=='i')
			{
				j--;
				dodaj_do_ekranu(i,'1',ekran);
				i++;
				dodaj_do_ekranu(i,'/',ekran);
				i++;
				dodaj_do_ekranu(i,'x',ekran);
				i++;
				dodaj_do_ekranu(i,' ',ekran);
				i++;
				liczba=1/tab[j];
				tab[j]=liczba;
				j++;
				liczba=0;
				_delay_ms(1000);
			}

			if(symbol[0]=='!')
			{
				j--;
				dodaj_do_ekranu(i,'!',ekran);
				i++;
				dodaj_do_ekranu(i,' ',ekran);
				i++;
				liczba=1;
				for(int k=1; k<=tab[j]; k++)
				liczba=liczba*k;

				tab[j]=liczba;
				j++;
				liczba=0;
				_delay_ms(1000);
			}

			if(symbol[0]=='u')
			{
				dodaj_do_ekranu(i,'-',ekran);
				i++;
				minus=(-1);
				_delay_ms(1000);
			}

			if(symbol[0]==' ')
			{
				dodaj_do_ekranu(i,' ',ekran);
				i++;
				tab[j]=liczba*minus;
				liczba=0;
				ulamek=0;
				j++;
				_delay_ms(1000);
			}

			if(symbol[0]=='.')
			{
				dodaj_do_ekranu(i,'.',ekran);
				i++;
				ulamek=10;
				_delay_ms(1000);
			}

			if(symbol[0]=='c')
			{
				for(int k=0; k<32; k++)
					ekran[k]=' ';
				LCD_HD44780::clear();
				break;
				_delay_ms(1000);
			}


			if(symbol[0]=='=')
			{
				LCD_HD44780::goTo(0, 1);
				LCD_HD44780::writeText("=");
				LCD_HD44780::goTo(2, 1);
				char *text;
				char znaki[16];
				text=dtostrf(tab[0],1,3,znaki);
				LCD_HD44780::writeText(text);
				_delay_ms(1000);
				break;
			}
			else
			{
				LCD_HD44780::goTo(0, 0);
				LCD_HD44780::writeText(ekran);
			}
		}
	}
}



