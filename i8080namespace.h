#pragma once
#include <inttypes.h>
#include <string>

namespace i8080 {
	using Byte = unsigned char;
	using Word = unsigned short int;

	struct i8080State;

	class regPair {
	private:
		Word data;
	public:
		regPair(Byte l, Byte h) {
			this->data = (h << 8) | l;
		}

		regPair() = default;

		Word retWord() {
			return this->data;
		}

		Byte retLower() {
			return this->data & 0xFF;
		}

		Byte retHigher() {
			return (this->data & 0xFF00) >> 8;
		}

		void setWord(Word d) {
			this->data = d;
		}

		void setLower(Byte l) {
			this->data &= 0xFF00;
			this->data |= l;
		}

		void setHigher(Byte h) {
			this->data &= 0xFF;
			this->data |= h << 8;
		}
	};
}
