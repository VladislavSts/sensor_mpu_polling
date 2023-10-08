/*
 * BspUart.cpp
 *
 *  Created on: Oct 7, 2023
 *      Author: Vladislav
 */

#include "BspUart.h"

//===============================================================================================//

uint8_t RxBuffer[RX_BUFFER_SIZE] = {0};
uint8_t TxBuffer[TX_BUFFER_SIZE] = {0};

//===============================================================================================//
const ConfigUart_t ConfigUart[(int)Uart_e::UART_COUNT] =
{
// UART2 // ********************************************* //
	{
		Handle: {
		  .BaudRate = 115200,
		  .DataWidth = LL_USART_DATAWIDTH_8B,
		  .StopBits = LL_USART_STOPBITS_1,
		  .Parity = LL_USART_PARITY_NONE,
		  .TransferDirection = LL_USART_DIRECTION_TX_RX,
		  .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
		  .OverSampling = LL_USART_OVERSAMPLING_16
		},
		.Tx = Gpio_e::UART_1_TX,
		.Rx = Gpio_e::UART_1_RX,
	}
};
//===============================================================================================//
void UartInit(Uart_e Index)
{
	LL_USART_InitTypeDef USART_InitStruct = {0}; // Подумать как сделать!
	USART_TypeDef *USARTx = nullptr;

	uint32_t Periphs;
	DMA_TypeDef *DMAx = nullptr;
	uint32_t RxChannel;
	uint32_t TxChannel;
	uint32_t RxDirection;
	uint32_t TxDirection;
	uint32_t Priority;
	uint32_t Mode;
	uint32_t PeriphOrM2MSrcIncMode;
	uint32_t MemoryOrM2MDstIncMode;
	uint32_t PeriphOrM2MSrcDataSize;
	uint32_t MemoryOrM2MDstDataSize;

	IRQn_Type RxDmaChannelIRQ;
	IRQn_Type TxDmaChannelIRQ;
	IRQn_Type UsartIRQ;

	switch (Index) {
		case Uart_e::UART_2:
			  /* DMA controller clock enable */
			  __HAL_RCC_DMA1_CLK_ENABLE();
			  RxDmaChannelIRQ = DMA1_Channel6_IRQn;
			  TxDmaChannelIRQ = DMA1_Channel7_IRQn;
			  UsartIRQ = USART2_IRQn;
			break;
	}

	  /* DMA interrupt init */
	  /* interrupt configuration */
	  NVIC_SetPriority(RxDmaChannelIRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	  NVIC_EnableIRQ(RxDmaChannelIRQ);
	  /* interrupt configuration */
	  NVIC_SetPriority(TxDmaChannelIRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	  NVIC_EnableIRQ(TxDmaChannelIRQ);

	  /* interrupt Init */
	  NVIC_SetPriority(UsartIRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	  NVIC_EnableIRQ(UsartIRQ);

	/* Clock configuration */
	switch(Index) {
		case Uart_e::UART_2:

			USARTx = USART2;
			Periphs = LL_APB1_GRP1_PERIPH_USART2;
			DMAx = DMA1;
			RxChannel = LL_DMA_CHANNEL_6;
			TxChannel = LL_DMA_CHANNEL_7;

			RxDirection = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
			TxDirection = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;

			Priority = LL_DMA_PRIORITY_LOW;
			Mode = LL_DMA_MODE_NORMAL;
			PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
			MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
			PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
			MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
			break;
	}

	LL_APB1_GRP1_EnableClock(Periphs);

	LL_DMA_SetDataTransferDirection(DMAx, RxChannel, RxDirection);
	LL_DMA_SetDataTransferDirection(DMAx, TxChannel, TxDirection);

	LL_DMA_SetChannelPriorityLevel(DMAx, RxChannel, Priority);
	LL_DMA_SetMode(DMAx, RxChannel, Mode);
	LL_DMA_SetPeriphIncMode(DMAx, RxChannel, PeriphOrM2MSrcIncMode);
	LL_DMA_SetMemoryIncMode(DMAx, RxChannel, MemoryOrM2MDstIncMode);
	LL_DMA_SetPeriphSize(DMAx, RxChannel, PeriphOrM2MSrcDataSize);
	LL_DMA_SetMemorySize(DMAx, RxChannel, MemoryOrM2MDstDataSize);

	USART_InitStruct.BaudRate = ConfigUart[(int) Index].Handle.BaudRate;
	USART_InitStruct.DataWidth = ConfigUart[(int) Index].Handle.DataWidth;
	USART_InitStruct.StopBits = ConfigUart[(int) Index].Handle.StopBits;
	USART_InitStruct.Parity = ConfigUart[(int) Index].Handle.Parity;
	USART_InitStruct.TransferDirection = ConfigUart[(int) Index].Handle.TransferDirection;
	USART_InitStruct.HardwareFlowControl = ConfigUart[(int) Index].Handle.HardwareFlowControl;
	USART_InitStruct.OverSampling = ConfigUart[(int) Index].Handle.OverSampling;

	LL_USART_Init(USARTx, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(USARTx);
	LL_USART_Enable(USARTx);

	GpioInit(ConfigUart[(int) Index].Tx);
	GpioInit(ConfigUart[(int) Index].Rx);

	//Настройка 6 канала DMA (прием данных)
	LL_USART_EnableIT_IDLE(USARTx);
	LL_DMA_ConfigAddresses(DMAx, RxChannel, LL_USART_DMA_GetRegAddr(USARTx), (uint32_t)RxBuffer,
		  LL_DMA_GetDataTransferDirection(DMAx, RxChannel));
	LL_DMA_SetDataLength(DMAx, RxChannel, RX_BUFFER_SIZE);
	LL_DMA_EnableChannel(DMAx, RxChannel);
	LL_USART_EnableDMAReq_RX(USARTx);

	//Настройка 7 канала DMA (передача данных)
	LL_DMA_EnableIT_TC(DMAx, TxChannel);
	LL_USART_EnableDMAReq_TX(USARTx);

}
//===============================================================================================//
void UartDeInit(Uart_e Uart) {

}


