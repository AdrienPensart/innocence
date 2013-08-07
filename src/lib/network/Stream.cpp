#include "Stream.hpp"
#include "SelectSet.hpp"

namespace Network
{
    Stream::Stream()
    :maxCharacter(DEFAULT_MAX_CHAR)
    {
    }

    Stream::~Stream()
    {
    }

    std::string Stream::recv()
    {
        std::string buffer;
        recv(buffer);
        return buffer;
    }

    Stream& Stream::operator<<(const std::string& object)
    {
        send(object);
        return *this;
    }

    Stream& Stream::operator>>(std::string& object)
    {
        recv(object);
        return *this;
    }

    void Stream::setMaxCharacter(const int& max)
    {
        maxCharacter = max;
    }

    const int Stream::getMaxCharacter()
    {
        return maxCharacter;
    }

    int Stream::send(const std::string& object)
    {
        return send(object.c_str(), object.size());
    }

    int Stream::send(const std::string& object, Timeout to)
    {
        return send(object.c_str(), object.size(), to);
    }
    /*
    int Stream::send(const std::vector<char>& object)
    {
        return send(&object[0], object.size());
    };
    */
    int Stream::recv(std::string& object)
    {
        char * buffer = new char[DEFAULT_MAX_CHAR];
        int received_char = 0; 
        try
        {
            received_char = recv(buffer, DEFAULT_MAX_CHAR);
            if(received_char >= 0)
            {
                object.assign(buffer, received_char);
            }
            delete buffer;
        }
        catch(...)
        {
            delete buffer;
            throw;
        }
        return received_char;
    }

    int Stream::recv(std::string& object, Timeout to)
    {
        int received_char = 0;
        char * buffer = new char[DEFAULT_MAX_CHAR];
        try
        {
            received_char = recv(buffer, DEFAULT_MAX_CHAR, to);
            if(received_char >= 0)
            {
                object.assign(buffer,received_char);
            }
            delete buffer;
        }
        catch(...)
        {
            delete buffer;
            throw;
        }
        return received_char;
    }

	int Stream::recv(std::string& object, char delimiter, Timeout to, bool include)
	{
		object.clear();
        char last;
        for(;;)
        {
            if(!recv(&last, 1, to))
            {
                break;
            }
            if(last != delimiter)
            {
                object += last;
            }
            else
            {
                if(include)
                    object+=delimiter;
                break;
            }
        }
		return object.size();
	}

    int Stream::recv(std::string& object, char delimiter, bool include)
    {
        object.clear();
        char last;
        for(;;)
        {
            if(!recv(&last, 1))
            {
                break;
            }
            if(last != delimiter)
            {
                object += last;
            }
            else
            {
                if(include)
                    object+=delimiter;
                break;
            }
        }
		return object.size();
    }
}
