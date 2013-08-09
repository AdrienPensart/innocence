#pragma once

#include <fstream>
#include <vector>
#include <list>
#include <string>

#include <common/Convert.hpp>
#include <common/Observable.hpp>
#include <common/Exception.hpp>

namespace Network
{
	class Stream;
}

namespace Blaspheme
{
    class TransferException : public Common::Exception
	{
		public:
	        
			TransferException(const std::string& msgArg);
	};
    
    class TransferDescriptor
    {
        public:

            TransferDescriptor();
            bool update();
            std::streamsize percentage;
            std::streamsize transferred;
            std::streamsize totalsize;
    };

    class TransferObserver : public Common::Observable<TransferObserver, TransferDescriptor>
    {
        public:

            virtual void update(const TransferDescriptor& td) = 0;
    };

	class LogTransfer : public TransferObserver
	{
		public:

			void update(const TransferDescriptor& td);
	};
    
    class Transfer : public Common::Observable<TransferObserver, TransferDescriptor>
    {
        public:
            
            enum TransferState
            {
                NOT_STARTED,
                INITIALIZED,
                TRANSFERRING,
                CANCELED,
                STOPPED,
                DONE
            };

            Transfer(const std::string& _filename, Network::Stream& _stream);
            
            void launch();
            bool isDone();
            Network::Stream& getStream(){return stream;}
            
        protected:
        
            virtual void init()=0;
            virtual void transfer()=0;
            virtual void finish()=0;
            
            TransferState state;
            Blaspheme::TransferDescriptor td;
            Network::Stream& stream;
            std::fstream file;
            std::string filename;
            std::string hash;
    };

    class Download : public Transfer
    {
        public:

            Download(const std::string& output_filename, Network::Stream& input_stream);
            
        protected:

            void init();
            void transfer();
            void finish();
    };

    class InMemoryDownload : public Transfer
    {
        public:
            
            InMemoryDownload(std::string& output_buffer, Network::Stream& input_stream);
        
       protected:

            void init();
            void transfer();
            void finish();

        private:
            
            std::string& memory_buffer;
    };

    class Upload : public Transfer
    {
        public:
            
            Upload(const std::string& input_filename, Network::Stream& output_stream);
        
        protected:
        
            void init();
            void transfer();
            void finish();
    };

} // Blaspheme
