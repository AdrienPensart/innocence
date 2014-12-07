#pragma once

#include "Types.hpp"

namespace Network
{
	/**
	 * Informations sur un h�te comme le nom de la machine locale, etc.
	 */
	class HostInfo
	{
		public:

			/**
			 * Renvoi l'adresse ip locale
			 */
			static Host getLocalIp();
			/**
			 * Renvoi du nom d'h�te de la machine locale
			 */
			static Host getLocalHostname();

			/**
			 * Permet de conna�tre l'ip d'une machine � partir de son nom d'hote
			 */
			static Host getHostByName(const Host& name);

			/**
			 * Permet de conna�tre le nom d'h�te d'une machine � partir de son ip
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
			 * Repr�sente un service sur une machine
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
			 * Renvoi le nom du service � partir de son port associ�
			 */
			static Service getServByPort(const Port& port, PROTO proto);
	};
} // Network
