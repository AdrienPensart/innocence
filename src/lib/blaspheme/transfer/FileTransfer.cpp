#include "FileTransfer.hpp"

#include <log/Log.hpp>
#include <network/Stream.hpp>
#include <system/File.hpp>
#include <common/Innocence.hpp>
#include <blaspheme/hash/Hash.hpp>

namespace Blaspheme {
	TransferException::TransferException(const std::string& msgArg) {
		LOG << "TransferException : " + Common::toString(typeid(this).name()) + msgArg;
	}

	TransferDescriptor::TransferDescriptor() {
		transferred = 0;
		totalsize = 0;
		percentage = 0;
	}

	// met a jour la progression
	bool TransferDescriptor::update() {
		if(totalsize != 0) {
			unsigned int current_percentage = (((unsigned int)transferred * 100) / (unsigned int)totalsize);
			if(current_percentage > percentage) {
				percentage = current_percentage;
				return true;
			}
		}
		return false;
	}

	void LogTransfer::update(const TransferDescriptor& td) {
		LOG << Common::toString(td.percentage);
	}

	Transfer::Transfer(const std::string& _filename, Network::Stream& _stream)
		:stream(_stream), state(NOT_STARTED) {
		filename = _filename;
	}

	void Transfer::launch() {
		init();
		while(!isDone()) {
			transfer();
		}
		finish();
	}

	bool Transfer::isDone() {
		if(td.transferred > td.totalsize) {
			throw TransferException("Overflow");
		}
		return (td.transferred == td.totalsize);
	}

	Download::Download(const std::string& _filename, Network::Stream& _stream)
		:Transfer(_filename, _stream) {
	}

	void Download::init() {
		try {
			file.open(filename.c_str(), std::ios::out | std::ios::binary);
			if (file.is_open()) {
				std::string buffer_size;
				stream.recv(hash, '\n', false);

				if(hash == FAILURE) {
					// l'envoyeur n'a pas trouve le fichier a envoyer
					state = CANCELED;
					throw TransferException("Remote file not found : "+filename);
				}

				stream.recv(buffer_size, '\n', false);
				Common::fromString(buffer_size, td.totalsize);
				state = INITIALIZED;
				notify(td);
			} else {
				state = CANCELED;
				throw TransferException("Local file does not exist : " + filename);
			}
		} catch(...) {
			state = CANCELED;
			throw;
		}
	}

	void Download::transfer() {
		try {
			static std::vector<char> buffer(40000);
			int recvd = stream.recv(&buffer[0], buffer.size());
			if(recvd <= 0) {
				throw TransferException("Download transfer returned " + Common::toString(recvd));
			}

			file.write(&buffer[0], recvd);
			td.transferred += recvd;
			if(td.update()) {
				notify(td);
			}
		} catch(...) {
			state = CANCELED;
			throw;
		}
	}

	void Download::finish() {
		if(file.is_open()) {
			file.close();
		}

		Hash hasher;
		std::string destination_hash = hasher.getHashFromFile(filename);
		// on verifie si le hash du fichier est correct
		if(destination_hash != hash) {
			LOG << "File hash is incorrect";
		} else if(td.totalsize == td.transferred) {
			td.percentage = 100;
			notify(td);
		}
		state = DONE;
	}

	InMemoryDownload::InMemoryDownload(std::string& output_buffer, Network::Stream& input_stream)
		:Transfer("", input_stream), memory_buffer(output_buffer) {
	}

	void InMemoryDownload::init() {
		// reception du hash
		std::string buffer_size;
		stream.recv(hash, '\n', false);
		if(hash == FAILURE) {
			// l'envoyeur n'a pas trouve le fichier a envoyer
			state = CANCELED;
			throw TransferException("Remote file not found");
		}

		// reception de la taille
		stream.recv(buffer_size, '\n', false);
		Common::fromString(buffer_size, td.totalsize);
		state = INITIALIZED;
		notify(td);

		// pour optimiser le transfert, on va tout de suite r�server
		// la capacit� m�moire ad�quate, attention au chargement de fichiers
		// trop gros...
		memory_buffer.reserve((unsigned int)td.totalsize);
	}

	void InMemoryDownload::transfer() {
		try {
			static std::vector<char> buffer(40000);
			int recvd = stream.recv(&buffer[0], buffer.size());
			if(recvd <= 0) {
				throw TransferException("InMemoryDownload transfer returned " + Common::toString(recvd));
			}

			memory_buffer.append(&buffer[0], recvd);
			td.transferred += recvd;
			if(td.update()) {
				notify(td);
			}
		} catch(...) {
			state = CANCELED;
			throw;
		}
	}

	void InMemoryDownload::finish() {
		Hash hasher;
		std::string destination_hash = hasher.getHash(memory_buffer);
		if(destination_hash != hash) {
			LOG << "Initial hash is not final hash";
		} else if(td.totalsize == td.transferred) {
			td.percentage = 100;
			notify(td);
		}
		state = DONE;
	}

	Upload::Upload(const std::string& _filename, Network::Stream& _stream)
		:Transfer(_filename, _stream) {
	}

	void Upload::init() {
		try {
			file.open(filename.c_str(), std::ios::in | std::ios::binary);
			if(file) {
				td.totalsize = System::Size(file);
				Blaspheme::Hash hasher;
				hash = hasher.getHashFromFile(filename);
				stream.send(hash+'\n');
				stream.send(Common::toString(td.totalsize)+'\n');
				LOG << "File size : " + Common::toString(td.totalsize) +", hash = " + hash;
				state = INITIALIZED;
				LOG << "Transfer initialized";
			} else {
				// On doit notifier au receveur de fichier que le fichier ne peut pas
				// etre ouvert
				stream << FAILURE;
				throw TransferException("Local file not found : " + filename);
			}
		} catch(...) {
			state = CANCELED;
			throw;
		}
	}

	void Upload::transfer() {
		static std::vector<char> buffer(40000);
		file.read(&buffer[0], buffer.size());
		std::streamsize to_send = file.gcount();
		std::streamsize sent = 0;

		while(sent != to_send) {
			std::streamsize temp_sent = (int)stream.send(&buffer[0]+sent, (int)(to_send-sent));
			if(temp_sent <= 0) {
				throw TransferException("Upload transfer failed : " + Common::toString(temp_sent) + ", network error : " + Network::SocketException::getLastError());
			}
			sent += temp_sent;
		}

		td.transferred += sent;
		if(td.update()) {
			notify(td);
		}
	}

	void Upload::finish() {
		if(file.is_open()) {
			file.close();
			state = DONE;
		}

		if(td.totalsize == td.transferred) {
			td.percentage = 100;
		}
		notify(td);
	}

} // Blaspheme
