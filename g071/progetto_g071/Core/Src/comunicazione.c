/*
 * comunicazione.c
 *
 *  Created on: Jun 13, 2021
 *      Author: elett
 */

//-----------------------------------------------------------------------------
void clear_usart1_error_flags (void);

//-----------------------------------------------------------------------------
#include "main.h"

//-----------------------------------------------------------------------------
void init_communication (void)
{
	LL_USART_ClearFlag_TC(USART1);
	LL_USART_DisableIT_TC(USART1);
	LL_USART_DisableDirectionTx(USART1);
	LL_USART_EnableDirectionRx(USART1);
	//disabilito gli interrupt di trasmissione
	LL_USART_DisableIT_TXE(USART1);
	//abilito interrupt di ricezione
	LL_USART_EnableIT_RXNE_RXFNE(USART1);
	LL_USART_RequestRxDataFlush(USART1);
	clear_usart1_error_flags();
}

//-----------------------------------------------------------------------------
void main_communication (void)
{
}

//-----------------------------------------------------------------------------
void tx_usart1_rx_interrupt (void)
{
	uint8_t rec, errors;
	rec = LL_USART_ReceiveData8(USART1);
	errors = (USART1->ISR & (USART_ISR_PE | USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE)) != 0;
	if (errors)
	{
		clear_usart1_error_flags();
		return;
	}
	LL_GPIO_TogglePin(pin_debug_GPIO_Port, pin_debug_Pin);
	if (!rec)
		LL_GPIO_ResetOutputPin(pin_led_GPIO_Port, pin_led_Pin);
	else
		LL_GPIO_SetOutputPin(pin_led_GPIO_Port, pin_led_Pin);
	init_communication();
}

//------------------------------------------------------------------------------
void clear_usart1_error_flags (void)
{
	LL_USART_ClearFlag_PE(USART1);
	LL_USART_ClearFlag_FE(USART1);
	LL_USART_ClearFlag_NE(USART1);
	LL_USART_ClearFlag_ORE(USART1);
}
