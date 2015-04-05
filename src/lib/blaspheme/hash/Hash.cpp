#include <fstream>
#include "Hash.hpp"

namespace Blaspheme {

	std::string Hash::hashit(const char * chunk, size_t size) {
		MD5_CTX ctx;
		md5.MD5Init(&ctx);
		md5.MD5Update(&ctx, (unsigned char*)chunk, size);
		unsigned char buff[16] = "";
		md5.MD5Final((unsigned char*)buff,&ctx);
		return convert(buff);
	}

	std::string Hash::convert(unsigned char *bytes) {
		char asciihash[33];

		int p = 0;
		for(int i=0; i<16; i++) {
			::sprintf(&asciihash[p],"%02x",bytes[i]);
			p += 2;
		}
		asciihash[32] = '\0';
		return std::string(asciihash);
	}

	std::string Hash::getHash(std::string text) {
		return hashit(text.c_str(), text.size());
	}

	std::string Hash::getHashFromFile(const std::string& filename) {
		FILE * file;
		MD5_CTX context;

		int len;
		unsigned char buffer[1024], digest[16];

		//open file
		if ((file = fopen (filename.c_str(), "rb")) == NULL) {
			return "-1";
		}

		//init md5
		md5.MD5Init (&context);

		//read the filecontent
		while ( (len = fread (buffer, 1, 1024, file)) ) {
			md5.MD5Update (&context, buffer, len);
		}

		/*
		generate hash, close the file and return the
		hash as std::string
		*/
		md5.MD5Final (digest, &context);
		fclose (file);
		return convert(digest);
	}

} // Blaspheme
