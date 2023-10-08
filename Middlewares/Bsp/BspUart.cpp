///*
// * BspUart.cpp
// *
// *  Created on: Oct 7, 2023
// *      Author: Vladislav
// */
//
//#include "BspUart.h"
////===============================================================================================//
//const ConfigUart_t ConfigUart[(int)Uart_e::UART_COUNT] =
//{
//// UART1 // ********************************************* //
//	{
//		Handle: {
//		  .BaudRate = 115200,
//		  .DataWidth = LL_USART_DATAWIDTH_8B,
//		  .StopBits = LL_USART_STOPBITS_1,
//		  .Parity = LL_USART_PARITY_NONE,
//		  .TransferDirection = LL_USART_DIRECTION_TX_RX,
//		  .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
//		  .OverSampling = LL_USART_OVERSAMPLING_16
//		},
//		.Tx = Gpio_e::UART_1_TX,
//		.Rx = Gpio_e::UART_1_RX,
//	}
//};
////===============================================================================================//
//void UartInit(Uart_e Index)
//{
//	LL_USART_InitTypeDef USART_InitStruct = {0}; // Подумать как сделать!
//
//	switch(Index) {
//		case Uart_e::UART_1:
//			__HAL_RCC_USART1_CLK_ENABLE();
//			break;
//
//		case Uart_e::UART_2:
//			__HAL_RCC_USART2_CLK_ENABLE();
//			break;
//	}
//
//	USART_InitStruct.BaudRate = ConfigUart[(int) Index].Handle.BaudRate;
//	USART_InitStruct.DataWidth = ConfigUart[(int) Index].Handle.DataWidth;
//	USART_InitStruct.StopBits = ConfigUart[(int) Index].Handle.StopBits;
//	USART_InitStruct.Parity = ConfigUart[(int) Index].Handle.Parity;
//	USART_InitStruct.TransferDirection = ConfigUart[(int) Index].Handle.TransferDirection;
//	USART_InitStruct.HardwareFlowControl = ConfigUart[(int) Index].Handle.HardwareFlowControl;
//	USART_InitStruct.OverSampling = ConfigUart[(int) Index].Handle.OverSampling;
//
//	LL_USART_Init(USART2, &USART_InitStruct);
//	LL_USART_ConfigAsyncMode(USART2);
//	LL_USART_Enable(USART2);
//
//	GpioInit(ConfigUart[(int) Index].Tx);
//	GpioInit(ConfigUart[(int) Index].Rx);
//
//}
////===============================================================================================//
//void UartDeInit(Uart_e Uart) {
//
//}
