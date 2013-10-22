#pragma once

#include "SocketHandle.hpp"
#include "SocketException.hpp"

namespace Network
{
    /**
     * Représente un socket basique, sans protocole particulier associé, cette
     * classe sert de base à TcpSocket, UdpSocket et RawSocket
     */
	class Socket : public SocketHandle
	{
		protected:
		
			// de quelle facon ferme-t-on la connexion ?
			enum
			{
				RECEIVING = 0,
				SENDING = 1,
				SENDRECV = 2
			};
			
			/** 
			 * Structure d'adresse associée à la socket
             */
			AddrIn attachedAddr;
			
		public:
		
			Socket();
			
			/**
			 * Activation / désactivation de certaines options de fonctionnement
			 * interne des socket
			 */
			int setOptions(const int& level,const int& optname, const void *optval, const socklen_t& optlen);
			/**
			 * Recuperations des options sous-jacentes de l'interface BSD
			 */
			int getOptions(int level, int option_name, char * option_value, socklen_t *option_len);
			
			/**
			 * Renvoi l'ip associée à la socket
			 */
			const Host getIp(void);
			
			/** 
			 * Renvoi le port associé à la socket
			 */
			const Port getPort(void);
			
			/**
			 * Renvoi les informations d'adresse de la machine de l'autre
			 * côté de la connexion
			 */
			int getPeerName(AddrIn& addr);
	
			/**
			 * Définition de l'ip et du port de connexion
			 */
			void setDestInfo(const Host& argIp, const Port& argPort);
			
			/**
			 * Définit uniquement l'ip vers laquelle il faut se connecter
			 */
			void setDestIp(const Host& argIp);
			
			/**
			 * Définit uniquement le port de la connexion
			 */
			void setDestPort(const Port& argPort);
			
			/**
			 * Utilise la structure standard d'adresse pour compléter les
			 * informations de connexion
			 */
			void setAddr(const AddrIn& Info);
			
			const AddrIn& getAddr() const;

			/**
			 * Fermeture explicite de la socket
			 */
			void close(void);
			
			/**
			 * Définit la taille du tampon d'envoi (optimisation)
			 */
			bool setSendBufSize(int);
			
			/**
			 * Récupère la taille du tampon d'envoi (optimisation)
			 */
			int  getSendBufSize();
			
			/**
			 * Définit la taille du tampon de réception (optimisation)
			 */
            bool setReceiveBufSize(int);
            
            /**
			 * Récupère la taille du tampon de réception (optimisation)
			 */
            int  getReceiveBufSize();
			
		private:
			
			/**
			 * Statut courant de la socket, est-il en mode bloqué ou non ?
			 */
			bool blocking_status;			
			
			/**
			 * Positionne le socket dans le mode synchrone(bloquant) ou en 
			 * mode asynchrone (non-bloquant), le socket est bloquant par défaut
			 */
			int setBlocking(const bool blocking);
			
		public:
			
			/**
			 * Met le socket en mode synchrone (bloquant)
             */
			int block(void)
			{
				return setBlocking(true);
			}
			
			/**
			 * Met le socket en mode asynchrone (non-bloquant)
             */
			int unblock(void)
			{
				return setBlocking(false);
			}
			
			/**
			 * Renvoi le statut du socket, soit bloquant, soit non-bloquant
			 */
			bool blocked(void) const
			{
				return blocking_status;
			}
	};
} // Network
