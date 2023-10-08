/*
 * Buffer.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

enum class Result_e : uint8_t
{
	OK = 0,
	ERROR,
};

template<typename type, size_t size>
class LineBuffer_c
{
private:
	type Buffer[size];

public:
	size_t ReadIndex = 0;
	size_t WriteIndex = 0;

	uint32_t GetAddressBuffer() {
		return (uint32_t)Buffer;
	}

	size_t GetVolume() {
		return size;
	}

	size_t Size()
	{
		return WriteIndex - ReadIndex;
	}

	Result_e Write(const type& Data)
	{
		if (WriteIndex != size)
		{
			Buffer[WriteIndex++] = Data;
			return Result_e::OK;
		}
		else
			return Result_e::ERROR;
	}

	Result_e Read(type& Data)
	{

		if (ReadIndex != WriteIndex)
		{
			Data = Buffer[ReadIndex++];
			return Result_e::OK;
		}
		else
			return Result_e::ERROR;
	}

	Result_e Wiev(type& Data, size_t Index)
	{
		if (ReadIndex + Index < WriteIndex)
		{
			Data = Buffer[ReadIndex+Index];
			return Result_e::OK;
		}
		else
			return Result_e::ERROR;
	}

	size_t CutHead(size_t Count)
	{
		if(Count <= Size())
		{
			ReadIndex += Count;
		}
		return Size();
	}

	size_t CutTail(size_t Count)
	{
		if(Count <= Size())
		{
			WriteIndex -= Count;
		}
		return Size();
	}

	void Clear()
	{
		ReadIndex = WriteIndex = 0;
	}
};
