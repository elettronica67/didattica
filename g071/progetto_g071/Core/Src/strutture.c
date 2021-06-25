/*
 * strutture.c
 *
 *  Created on: Jun 25, 2021
 *      Author: pnenna
 */

#include "main.h"
#include <string.h>

#define	N_BYTE_CHIAVE		(6)
#define	N_CHIAVI_GESTITE	(10)
#define	N_LETTORI_GESTITI	(10)

const byte k_chiavi_gestite[N_CHIAVI_GESTITE][N_BYTE_CHIAVE] = {
		{0xaa, 0x55, 0x00, 0x00, 0x00, 0x00},
		{0xab, 0x55, 0x00, 0x00, 0x00, 0x00},
		{0xac, 0x55, 0x00, 0x00, 0x00, 0x00},
		{0xad, 0x55, 0x00, 0x00, 0x00, 0x00},
		{0xae, 0x55, 0x00, 0x00, 0x00, 0x00},
		{0xaf, 0x55, 0x00, 0x00, 0x00, 0x00},
		{0xb0, 0x55, 0x00, 0x00, 0x00, 0x00},
		{0xb1, 0x55, 0x00, 0x00, 0x00, 0x00},
		{0xb2, 0x55, 0x00, 0x00, 0x00, 0x00},
		{0xb3, 0x55, 0x00, 0x00, 0x00, 0x00},
};

typedef struct
{
	byte	chiave_presente;				//0 = chiave assente; !=0 = chiave presente
	byte	fault;							//0 = nessun guasto, 1 = guasto comunicazione, 2 = guasto lettore chiave
	byte	sabotaggio;						//0 = nessun sabotaggio, 1 = forzatura apertura frutto
	byte	tensione_alimentazione;		//tensione di alimentazione del lettore espressa in decimi di volt
	byte	temperatura_ambiente;		//temperatura letta in decimi di grado con offset di 10 gradi
	byte	dati[N_BYTE_CHIAVE];
} dati_lettore_t;

void lettura_dati_lettori (void);
void leggi_lettore(dati_lettore_t *pointer_lettore);
uint8_t chiave_programmata(dati_lettore_t *pointer_lettore);

dati_lettore_t dati_lettore[N_LETTORI_GESTITI];

//-----------------------------------------------------------------------------
void lettura_dati_lettori (void)
{
	uint8_t i;
	dati_lettore_t *loc_pointer;

	for (i=0, loc_pointer=&dati_lettore[0]; i<N_LETTORI_GESTITI; ++i, loc_pointer++)
	{
		leggi_lettore(loc_pointer);
		if ((loc_pointer->chiave_presente != 0) && (chiave_programmata(loc_pointer) != 0))
		{
				//led acceso
				LL_GPIO_SetOutputPin(pin_led_GPIO_Port, pin_led_Pin);
		}
		else
			//led spento
			LL_GPIO_ResetOutputPin(pin_led_GPIO_Port, pin_led_Pin);
	}
}

//-----------------------------------------------------------------------------
void leggi_lettore(dati_lettore_t *pointer_lettore)
{
	pointer_lettore->chiave_presente = 1;
	pointer_lettore->fault = 0;
	pointer_lettore->sabotaggio = 0;
	pointer_lettore->temperatura_ambiente = 150;
	pointer_lettore->tensione_alimentazione = 125;
	memset(&pointer_lettore->dati[0], 0, sizeof(pointer_lettore->dati));
	pointer_lettore->dati[0] = 0xaa;
	pointer_lettore->dati[1] = 0x55;
}

//-----------------------------------------------------------------------------
uint8_t chiave_programmata(dati_lettore_t *pointer_lettore)
{
	for (uint8_t i=0; i<N_CHIAVI_GESTITE; i++)
	{
		if (!memcmp(&pointer_lettore->dati[0], &k_chiavi_gestite[i][0], N_BYTE_CHIAVE))
		{
			return 1;
		}
	}
	return 0;
}



