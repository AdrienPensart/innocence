#pragma once

#include <string>
#include <vector>
#include "Socket.hpp"
#include "Timeout.hpp"

namespace Network
{
    /**
     * Repr�sente un flux d'octet sur le r�seau, le client qui utilise cet objet
     * ne sait pas si ce qu'il envoi est envoy� par UDP, TCP ou une autre technique
     */
    class Stream
    {
        public:
            Stream();
            virtual ~Stream();

            /**
             * Surcharge de l'operateur de d�calage pour l'envoi d'un objet string
             * vers l'autre c�t� de la connexion
             */
            virtual Stream& operator<<(const std::string& object);

            /**
             * Surcharge de l'operateur de d�calage pour la r�ception d'un objet string
             * depuis l'autre c�t� de la connexion
             */
            virtual Stream& operator>>(std::string& object);

		    /**
		     * Fonction d'envoi d'une cha�ne de caract�re, avec ou sans timeout
		     */
            virtual int send(const char * object, int sizeOfObject)=0;
		    virtual int send(const char * object, int sizeOfObject, Timeout to)=0;
            virtual int send(const std::string& object)=0;
            virtual int send(const std::string& object, Timeout to)=0;
            //virtual int send(const std::vector<char>& object);
            
            /**
		     * Fonction de r�ception d'une cha�ne de caract�re, avec ou sans timeout
		     */
            virtual int recv(char * object, int sizeOfObject)=0;
		    virtual int recv(char * object, int sizeOfObject, Timeout to)=0;
            virtual int recv(std::string& object)=0;
            virtual int recv(std::string& object, Timeout to)=0;
            virtual std::string recv();
            //virtual int recv(std::vector<char>& object);
            
            /**
             * Reception d'une chaine de caractere jusqu'au caract�re c
             */
			virtual int recv(std::string& object, char delimiter, bool include=false);
            virtual int recv(std::string& object, char delimiter, Timeout to, bool include=false);
            
		    /**
		     * Renvoi la taille du buffer d'envoi / r�ception (optimisation)
		     */
            virtual int getBufferDataSize()=0;
            
            /**
             * D�finit le nombre maximal de caract�re � envoyer � chaque envoi
             * ou r�ception de donn�es
             */
            void setMaxCharacter(const int& max);
            
            /**
             * R�cup�re le nombre maximal de caract�re � envoyer � chaque envoi
             * ou r�ception de donn�es
             */
            const int getMaxCharacter();
            
        protected:
            
            /**
             * Nombre maximal de caract�re � envoyer ou � recevoir du r�seau
             */
            int maxCharacter;

	    public:
	
		    enum { DEFAULT_MAX_CHAR=8192 };

    };
} // Network
