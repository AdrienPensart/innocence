#pragma once

#include <string>
#include <vector>
#include "Socket.hpp"
#include "Timeout.hpp"

namespace Network
{
    /**
     * Représente un flux d'octet sur le réseau, le client qui utilise cet objet
     * ne sait pas si ce qu'il envoi est envoyé par UDP, TCP ou une autre technique
     */
    class Stream
    {
        public:
            Stream();
            virtual ~Stream();

            /**
             * Surcharge de l'operateur de décalage pour l'envoi d'un objet string
             * vers l'autre côté de la connexion
             */
            virtual Stream& operator<<(const std::string& object);

            /**
             * Surcharge de l'operateur de décalage pour la réception d'un objet string
             * depuis l'autre côté de la connexion
             */
            virtual Stream& operator>>(std::string& object);

		    /**
		     * Fonction d'envoi d'une chaîne de caractère, avec ou sans timeout
		     */
            virtual int send(const char * object, int sizeOfObject)=0;
		    virtual int send(const char * object, int sizeOfObject, Timeout to)=0;
            virtual int send(const std::string& object)=0;
            virtual int send(const std::string& object, Timeout to)=0;
            //virtual int send(const std::vector<char>& object);
            
            /**
		     * Fonction de réception d'une chaîne de caractère, avec ou sans timeout
		     */
            virtual int recv(char * object, int sizeOfObject)=0;
		    virtual int recv(char * object, int sizeOfObject, Timeout to)=0;
            virtual int recv(std::string& object)=0;
            virtual int recv(std::string& object, Timeout to)=0;
            virtual std::string recv();
            //virtual int recv(std::vector<char>& object);
            
            /**
             * Reception d'une chaine de caractere jusqu'au caractère c
             */
			virtual int recv(std::string& object, char delimiter, bool include=false);
            virtual int recv(std::string& object, char delimiter, Timeout to, bool include=false);
            
		    /**
		     * Renvoi la taille du buffer d'envoi / réception (optimisation)
		     */
            virtual int getBufferDataSize()=0;
            
            /**
             * Définit le nombre maximal de caractère à envoyer à chaque envoi
             * ou réception de données
             */
            void setMaxCharacter(const int& max);
            
            /**
             * Récupère le nombre maximal de caractère à envoyer à chaque envoi
             * ou réception de données
             */
            const int getMaxCharacter();
            
        protected:
            
            /**
             * Nombre maximal de caractère à envoyer ou à recevoir du réseau
             */
            int maxCharacter;

	    public:
	
		    enum { DEFAULT_MAX_CHAR=8192 };

    };
} // Network
