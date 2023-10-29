/*
 * BspUart.cpp
 *
 *  Created on: Oct 7, 2023
 *      Author: Vladislav
 */

#include "BspUart.h"
#include "Buffer.h"

//===============================================================================================//
LineBuffer_c<char, 256> RxBufferUart2;
LineBuffer_c<char, 256> TxBufferUart2;
//===============================================================================================//
LL_USART_InitTypeDef ConfigUart2 =
{
// UART2 // ********************************************* //
	  .BaudRate = 115200,
	  .DataWidth = LL_USART_DATAWIDTH_8B,
	  .StopBits = LL_USART_STOPBITS_1,
	  .Parity = LL_USART_PARITY_NONE,
	  .TransferDirection = LL_USART_DIRECTION_TX_RX,
	  .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
	  .OverSampling = LL_USART_OVERSAMPLING_16
};

const Bsp_Uart_Config BspUart2 =
{
	.Tx = Gpio_e::UART_2_TX,
	.Rx = Gpio_e::UART_2_RX,
};
//===============================================================================================//

Uart_c Usart2(USART2, &ConfigUart2, &BspUart2);

//===============================================================================================//
void Uart_c::Init()
{
	LL_USART_InitTypeDef USART_InitStruct = {0};
	USART_TypeDef *USARTx = nullptr;

	uint32_t Periphs;
	DMA_TypeDef *DMAx = nullptr;
	uint32_t RxChannel, TxChannel,
	RxDirection, TxDirection,
	Priority,
	RxMode, TxMode,
	PeriphOrM2MSrcIncMode,
	MemoryOrM2MDstIncMode,
	PeriphOrM2MSrcDataSize,
	MemoryOrM2MDstDataSize;

	IRQn_Type RxDmaChannelIRQ, TxDmaChannelIRQ, UsartIRQ;

	if (UsartX == USART2) {
		__HAL_RCC_DMA1_CLK_ENABLE();
		RxDmaChannelIRQ = DMA1_Channel6_IRQn;
		TxDmaChannelIRQ = DMA1_Channel7_IRQn;
		UsartIRQ = USART2_IRQn;
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

	if (UsartX == USART2) {
		USARTx = UsartX;
		Periphs = LL_APB1_GRP1_PERIPH_USART2;
		DMAx = DMA1;
		RxChannel = LL_DMA_CHANNEL_6;
		TxChannel = LL_DMA_CHANNEL_7;

		RxDirection = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
		TxDirection = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;

		Priority = LL_DMA_PRIORITY_LOW;
		RxMode = LL_DMA_MODE_CIRCULAR;
		TxMode = LL_DMA_MODE_CIRCULAR;

		PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
		MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
		PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
		MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	}

	LL_APB1_GRP1_EnableClock(Periphs);

	LL_DMA_SetDataTransferDirection(DMAx, RxChannel, RxDirection);
	LL_DMA_SetDataTransferDirection(DMAx, TxChannel, TxDirection);

	LL_DMA_SetChannelPriorityLevel(DMAx, RxChannel, Priority);
	LL_DMA_SetChannelPriorityLevel(DMAx, TxChannel, Priority);

	LL_DMA_SetMode(DMAx, RxChannel, RxMode);
	LL_DMA_SetMode(DMAx, TxChannel, TxMode);

	LL_DMA_SetPeriphIncMode(DMAx, RxChannel, PeriphOrM2MSrcIncMode);
	LL_DMA_SetPeriphIncMode(DMAx, TxChannel, PeriphOrM2MSrcIncMode);

	LL_DMA_SetMemoryIncMode(DMAx, RxChannel, MemoryOrM2MDstIncMode);
	LL_DMA_SetMemoryIncMode(DMAx, TxChannel, MemoryOrM2MDstIncMode);

	LL_DMA_SetPeriphSize(DMAx, RxChannel, PeriphOrM2MSrcDataSize);
	LL_DMA_SetPeriphSize(DMAx, TxChannel, PeriphOrM2MSrcDataSize);

	LL_DMA_SetMemorySize(DMAx, RxChannel, MemoryOrM2MDstDataSize);
	LL_DMA_SetMemorySize(DMAx, TxChannel, MemoryOrM2MDstDataSize);

	USART_InitStruct.BaudRate = UartHdl->BaudRate;
	USART_InitStruct.DataWidth = UartHdl->DataWidth;
	USART_InitStruct.StopBits = UartHdl->StopBits;
	USART_InitStruct.Parity = UartHdl->Parity;
	USART_InitStruct.TransferDirection = UartHdl->TransferDirection;
	USART_InitStruct.HardwareFlowControl = UartHdl->HardwareFlowControl;
	USART_InitStruct.OverSampling = UartHdl->OverSampling;

	LL_USART_Init(USARTx, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(USARTx);
	LL_USART_Enable(USARTx);

	GpioInit(UartCfg->Tx);
	GpioInit(UartCfg->Rx);

	//Настройка 6 канала DMA (прием данных)
	LL_USART_EnableIT_IDLE(USARTx);
	//Настройка 7 канала DMA (передача данных)
	LL_DMA_EnableIT_TC(DMAx, TxChannel);

	/* TODO Подумать, как сделать лучше */
	uint32_t RxDstAddress, TxDstAddress;
	uint32_t RxNbData, TxNbData;

	if (UsartX == USART2) {
		RxDstAddress = (uint32_t)RxBufferUart2.GetAddressBuffer(); // TODO прямой доступ, нарушение инкапсуляции, подумать!
		TxDstAddress = (uint32_t)TxBufferUart2.GetAddressBuffer();
		RxNbData = RxBufferUart2.GetVolume();
		TxNbData = TxBufferUart2.GetVolume();
	}

	LL_DMA_ConfigAddresses(DMAx, RxChannel, LL_USART_DMA_GetRegAddr(USARTx), RxDstAddress, LL_DMA_GetDataTransferDirection(DMAx, RxChannel));
	LL_DMA_SetDataLength(DMAx, RxChannel, RxNbData);
	LL_DMA_EnableChannel(DMAx, RxChannel);
	LL_USART_EnableDMAReq_RX(USARTx);

    LL_DMA_ConfigAddresses(DMAx, TxChannel, TxDstAddress, LL_USART_DMA_GetRegAddr(USARTx), LL_DMA_GetDataTransferDirection(DMAx, TxChannel));
    LL_DMA_SetDataLength(DMAx, TxChannel, TxNbData);
	LL_USART_EnableDMAReq_TX(USARTx);

}



