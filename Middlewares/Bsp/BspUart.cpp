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

	uint32_t Periphs;
	DMA_TypeDef *Tx_DMAx = nullptr;
	DMA_TypeDef *Rx_DMAx = nullptr;
	uint32_t RxChannel, TxChannel;

	/* TODO Подумать, как сделать лучше */
	uint32_t RxDstAddress, TxDstAddress;
	uint32_t RxNbData, TxNbData;

	IRQn_Type RxDmaChannelIRQ, TxDmaChannelIRQ, UsartIRQ;

	if (UsartX == USART2) {
		__HAL_RCC_DMA1_CLK_ENABLE();
		RxDmaChannelIRQ = DMA1_Channel6_IRQn;
		TxDmaChannelIRQ = DMA1_Channel7_IRQn;

		UsartIRQ = USART2_IRQn;
		Periphs = LL_APB1_GRP1_PERIPH_USART2;

		Tx_DMAx = DMA1;
		Rx_DMAx = DMA1;

		RxChannel = LL_DMA_CHANNEL_6;
		TxChannel = LL_DMA_CHANNEL_7;

		RxDstAddress = (uint32_t)RxBufferUart2.GetAddressBuffer();
		TxDstAddress = (uint32_t)TxBufferUart2.GetAddressBuffer();
		RxNbData = RxBufferUart2.GetVolume();
		TxNbData = TxBufferUart2.GetVolume();
	}

	GpioInit(UartCfg->Tx);
	GpioInit(UartCfg->Rx);

	/* DMA interrupt init */
	/* interrupt configuration */
	NVIC_SetPriority(RxDmaChannelIRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 8, 0));
	NVIC_EnableIRQ(RxDmaChannelIRQ);
	/* interrupt configuration */
	NVIC_SetPriority(TxDmaChannelIRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 8, 0));
	NVIC_EnableIRQ(TxDmaChannelIRQ);

	/* Clock configuration */
	LL_APB1_GRP1_EnableClock(Periphs);

	/* Config DMA Rx */
	LL_DMA_SetDataTransferDirection(Rx_DMAx, RxChannel, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	LL_DMA_SetChannelPriorityLevel(Rx_DMAx, RxChannel, LL_DMA_PRIORITY_LOW);
	LL_DMA_SetMode(Rx_DMAx, RxChannel, LL_DMA_MODE_CIRCULAR);
	LL_DMA_SetPeriphIncMode(Rx_DMAx, RxChannel, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(Rx_DMAx, RxChannel, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_SetPeriphSize(Rx_DMAx, RxChannel, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMemorySize(Rx_DMAx, RxChannel, LL_DMA_MDATAALIGN_BYTE);

	/* Config DMA Tx */
	LL_DMA_SetDataTransferDirection(Tx_DMAx, TxChannel, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetChannelPriorityLevel(Tx_DMAx, TxChannel, LL_DMA_PRIORITY_LOW);
	LL_DMA_SetMode(Tx_DMAx, TxChannel, LL_DMA_MODE_CIRCULAR);
	LL_DMA_SetPeriphIncMode(Tx_DMAx, TxChannel, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(Tx_DMAx, TxChannel, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_SetPeriphSize(Tx_DMAx, TxChannel, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMemorySize(Tx_DMAx, TxChannel, LL_DMA_MDATAALIGN_BYTE);

	USART_InitStruct.BaudRate = UartHdl->BaudRate;
	USART_InitStruct.DataWidth = UartHdl->DataWidth;
	USART_InitStruct.StopBits = UartHdl->StopBits;
	USART_InitStruct.Parity = UartHdl->Parity;
	USART_InitStruct.TransferDirection = UartHdl->TransferDirection;
	USART_InitStruct.HardwareFlowControl = UartHdl->HardwareFlowControl;
	USART_InitStruct.OverSampling = UartHdl->OverSampling;

	LL_USART_Init(UsartX, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(UsartX);
	LL_USART_Enable(UsartX);

	//Настройка 6 канала DMA (прием данных)
	LL_USART_EnableIT_IDLE(UsartX);
	//Настройка 7 канала DMA (передача данных)
	LL_DMA_EnableIT_TC(Tx_DMAx, TxChannel);

	LL_DMA_ConfigAddresses(Rx_DMAx, RxChannel, LL_USART_DMA_GetRegAddr(UsartX), RxDstAddress, LL_DMA_GetDataTransferDirection(Rx_DMAx, RxChannel));
	LL_DMA_SetDataLength(Rx_DMAx, RxChannel, RxNbData);
	LL_DMA_EnableChannel(Rx_DMAx, RxChannel);
	LL_USART_EnableDMAReq_RX(UsartX);

    LL_DMA_ConfigAddresses(Tx_DMAx, TxChannel, TxDstAddress, LL_USART_DMA_GetRegAddr(UsartX), LL_DMA_GetDataTransferDirection(Tx_DMAx, TxChannel));
    LL_DMA_SetDataLength(Tx_DMAx, TxChannel, TxNbData);
    LL_USART_EnableDMAReq_TX(UsartX);

	/* Interrupt Init */
	NVIC_SetPriority(UsartIRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 8, 0));
	NVIC_EnableIRQ(UsartIRQ);
}
