/*
 * comunicazione.c
 *
 *  Created on: Jun 13, 2021
 *      Author: elett
 */

//-----------------------------------------------------------------------------
#include "main.h"
#include "cmsis_os.h"

//-----------------------------------------------------------------------------
//funzioni locali
void clear_usart6_error_flags (void);

//-----------------------------------------------------------------------------
void init_communication (void)
{
	LL_USART_ClearFlag_TC(USART6);
	LL_USART_DisableIT_TC(USART6);
	LL_USART_EnableDirectionTx(USART6);
}

//-----------------------------------------------------------------------------
void main_communication (void)
{
	//cancello tutti i flag di errore
	clear_usart6_error_flags();
	//disbilito la ricezione
	LL_USART_DisableDirectionRx(USART6);
	LL_USART_DisableIT_RXNE(USART6);
	//interrupt su fine carattere
	LL_USART_DisableIT_TXE(USART6);
	LL_USART_ClearFlag_TC(USART6);
	if (LL_GPIO_IsInputPinSet(pin_button_GPIO_Port, pin_button_Pin))
		LL_USART_TransmitData8(USART6, 0x00);
	else
		LL_USART_TransmitData8(USART6, 0x01);
	LL_USART_EnableIT_TC(USART6);
	LL_USART_EnableDirectionTx(USART6);
}

//-----------------------------------------------------------------------------
void tx_usart6_interrupt (void)
{
	LL_GPIO_TogglePin(pin_debug_GPIO_Port, pin_debug_Pin);
	LL_USART_DisableIT_TC(USART6);
}

//------------------------------------------------------------------------------
void clear_usart6_error_flags (void)
{
	LL_USART_ClearFlag_PE(USART6);
	LL_USART_ClearFlag_FE(USART6);
	LL_USART_ClearFlag_NE(USART6);
	LL_USART_ClearFlag_ORE(USART6);
}
