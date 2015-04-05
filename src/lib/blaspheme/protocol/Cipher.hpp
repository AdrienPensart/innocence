#pragma once

#include <string>

namespace Blaspheme {
	class CipherMethod {
		public:

			CipherMethod() {}
			virtual void encrypt(std::string&)=0;
			virtual void decrypt(std::string&)=0;
	};

	class NoCiphering : public CipherMethod {
		public:

			NoCiphering();
			void encrypt(std::string&);
			void decrypt(std::string&);
	};

	class XORCiphering : public CipherMethod {
		public:

			XORCiphering(const std::string& initial_key="");
			void encrypt(std::string& object);
			void decrypt(std::string& object);
			void setKey(const std::string& xor_key);

		private:

			void x_or(std::string& object);
			std::string xor_key;
	};

} // Blaspheme
