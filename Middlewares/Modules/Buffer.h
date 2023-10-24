/*
 * Buffer.h
 *
 *  Created on: Oct 8, 2023
 *      Author: Vladislav
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cstdarg>
#include <stdio.h>

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

	int FormatString(const char* format, ...) {
		va_list args;
		va_start(args, format);
		int result = snprintf(Buffer, size, format, args);
		va_end(args);

		return result;
	}

	template<size_t strSize>
	Result_e FindString(const char(&str)[strSize])
	{
		/* Поиск строки в буффере приема */
		size_t strLen = strSize - 1; // Исключаем завершающий нулевой символ
		size_t searchIndex = ReadIndex;

		if (strLen > WriteIndex || WriteIndex == 0)
			return Result_e::ERROR;

		while (searchIndex < WriteIndex - strLen + 1)
		{
			bool match = true;
			for (size_t i = 0; i < strLen; ++i) {
				if (Buffer[(searchIndex + i) % size] != str[i]) {
					match = false;
					break;
				}
			}
			if (match)
				return Result_e::OK;

			++searchIndex;
		}
		return Result_e::ERROR;
	}

	type* GetAddressBuffer() const {
		return (type*)Buffer;
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
		memset(Buffer, 0, size);
	}
};
