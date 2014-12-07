#pragma once

#include "Types.hpp"

namespace Network
{
	/**
	 * Informations sur un hôte comme le nom de la machine locale, etc.
	 */
	class HostInfo
	{
		public:

			/**
			 * Renvoi l'adresse ip locale
			 */
			static Host getLocalIp();
			/**
			 * Renvoi du nom d'hôte de la machine locale
			 */
			static Host getLocalHostname();

			/**
			 * Permet de connaître l'ip d'une machine à partir de son nom d'hote
			 */
			static Host getHostByName(const Host& name);

			/**
			 * Permet de connaître le nom d'hôte d'une machine à partir de son ip
			 */
			static Host getHostByAddr(const Host& ip);

		private:

			// taille par defaut du hostname
			enum {SIZEOFHOSTNAME = 64};
	};

	/**
	 * Donne des informations sur les services que proposent une machine
	 */
	class ServiceInfo
	{
		public:
			/**
			 * Représente un service sur une machine
			 */
			typedef struct
			{
				Host name;
				Port port;
				int proto;
			} Service;

			/**
			 * Le protocole que le service utilise
			 */
			enum PROTO {TCP, UDP};

			static Service getServByName(const Host& name, PROTO proto);

			/**
			 * Renvoi le nom du service à partir de son port associé
			 */
			static Service getServByPort(const Port& port, PROTO proto);
	};
} // Network
