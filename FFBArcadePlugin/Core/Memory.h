/*This file is part of FFB Arcade Plugin.
FFB Arcade Plugin is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
FFB Arcade Plugin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with FFB Arcade Plugin.If not, see < https://www.gnu.org/licenses/>.
*/

#pragma once
#include <Windows.h>
#include <stddef.h>
#include <type_traits>

namespace ffb
{
	// What the offset of a game address is relative to
	enum class Base
	{
		Absolute, // the address itself
		Module,   // the base of the game's executable, wherever Windows loaded it
	};

	// A value of type T in the memory of the game, e.g. GameValue<float>{ Base::Module, 0x392C58 }
	template <typename T>
	struct GameValue
	{
		Base base;
		INT_PTR offset;
	};

	// A value reached through pointers: the pointer at `root`, plus offsets[0] gives the address of the
	// next pointer, and so on; the value is at the last pointer plus the last offset.
	// e.g. PointerChain<UINT8, 2>{ { Base::Module, 0x1F7D578 }, { 0x268, 0xAC } }
	template <typename T, size_t N>
	struct PointerChain
	{
		GameValue<INT_PTR> root;
		INT_PTR offsets[N];
	};

	// Reads and writes the memory of the game. A read that fails returns 0.
	class Memory
	{
	public:
		// Base of the game's executable
		static INT_PTR ModuleBase();

		static INT_PTR Address(Base base, INT_PTR offset)
		{
			return base == Base::Module ? ModuleBase() + offset : offset;
		}

		template <typename T>
		static INT_PTR Address(const GameValue<T>& value)
		{
			return Address(value.base, value.offset);
		}

		template <typename T>
		static T Read(INT_PTR address)
		{
			T value{};
			ReadBytes(address, &value, sizeof(T), std::is_floating_point<T>::value);
			return value;
		}

		template <typename T>
		static T Read(const GameValue<T>& value)
		{
			return Read<T>(Address(value));
		}

		template <typename T, size_t N>
		static T Read(const PointerChain<T, N>& chain)
		{
			INT_PTR pointer = Read(chain.root);
			for (size_t i = 0; i + 1 < N; i++)
				pointer = Read<INT_PTR>(pointer + chain.offsets[i]);
			return Read<T>(pointer + chain.offsets[N - 1]);
		}

		template <typename T>
		static void Write(INT_PTR address, T value)
		{
			WriteBytes(address, &value, sizeof(T));
		}

		template <typename T>
		static void Write(const GameValue<T>& value, T newValue)
		{
			Write<T>(Address(value), newValue);
		}

		// Replaces count bytes of code by NOP instructions
		static void WriteNop(INT_PTR address, int count);

		// Copies bytes over code or read-only data (the page protection is lifted for the copy)
		static void Patch(INT_PTR address, const void* bytes, size_t size);

		// Writes a jump (E9 rel32) from `from` to `to`
		static void WriteJump(INT_PTR from, const void* to);

		// First address of the process memory matching the pattern, whose mask has 'x' for the bytes
		// to compare and '?' for the others; skips `skip` (a match found before). Null if none.
		static char* FindPattern(const char* pattern, const char* mask, const char* skip = nullptr);

	private:
		// floating: the value is a float or a double (unused by the plugin; lets a test harness
		// make up plausible values)
		static void ReadBytes(INT_PTR address, void* destination, size_t size, bool floating);
		static void WriteBytes(INT_PTR address, const void* source, size_t size);
	};
}
