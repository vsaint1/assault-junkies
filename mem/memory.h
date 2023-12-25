#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include <cstdint>
#include "../utils/helper_macros.h"

class Memory {
	int m_pid;
	uintptr_t m_moduleName;
	unsigned int m_moduleSize;

public:
	explicit Memory(LPCSTR moduleName) {
		this->m_moduleName = reinterpret_cast<uintptr_t>(GetModuleHandleA(moduleName));
		this->m_pid = static_cast<int>(GetCurrentProcessId());
	}

	Memory(const Memory&) = delete;

	Memory& operator=(const Memory&) = delete;


	unsigned int getModuleSize() {
		return this->m_moduleSize;
	}

	uintptr_t getModuleBase() {
		return this->m_moduleName;
	}

	int getProcessId() {
		return this->m_pid;
	}

	template<typename T>
	T read(uint8_t* address) {
		return *reinterpret_cast<T*>(address);
	}

	template<typename T>
	T read(uintptr_t address) {
		return *reinterpret_cast<T*>(address);
	}

	template<>
	std::string read(uintptr_t address) {
		std::string str;
		char ch;
		do {
			ch = this->read<char>(address);
			str += ch;
			address++;
		} while (ch != '\0');

		return str;
	}

	template<typename T>
	void write(uintptr_t address, T value) {
		*reinterpret_cast<T*>(address) = value;
	}

	uint8_t* findSignature(const char* szSignature) noexcept;

	bool patch(uint8_t* address, char* bytes, unsigned int length) noexcept;

};
