#pragma once

#include <map>
#include <string>
#include <iostream>

#include "Utility.hpp"

namespace Common
{
	class Callback
	{
		public:
		
			Callback(const std::string& labelArg) : 
				label(labelArg)
			{
			}
		
			const std::string& getLabel() const
			{
				return label;
			}
		
			void setLabel(const std::string& labelArg)
			{
				label = labelArg;
			}
		
			virtual void execute()=0;
		
		private:
	
			std::string label;
	};

	class Menu
	{
		typedef std::map<unsigned int, Callback *> MenuRep;

		public:
		
			Menu() : 
				next(0)
			{
			}
		
			~Menu()
			{
				for(MenuRep::iterator iter = menu.begin(); iter != menu.end(); iter++)
				{
					delete iter->second;
				}
			}
		
			void add(Callback * callback)
			{
				menu[next] = callback;
				next++;
			}
		
			void show()
			{
				for(MenuRep::iterator iter = menu.begin(); iter != menu.end(); iter++)
				{
					std::cout << iter->first << " -> " << iter->second->getLabel() << '\n';
				}
			}
		
			void prompt()
			{
				std::cout << "\n\tChoix : ";
				std::string buffer;
				unsigned int choice = 0;
				std::cin >> buffer;
			
				if(!fromString(buffer, choice))
				{
					if(next > 1)
					{
						std::cout << "Veuillez entrer un nombre entre 0 et " << next-1 << '\n';
					}
					else
					{
						std::cout << "Nombre invalide.\n";
					}
				}
				else
				{
					MenuRep::iterator iter = menu.find(choice);
					if(iter != menu.end())
					{
						iter->second->execute();
					}
					else
					{
						std::cout << "Entree utilisateur invalide.\n";
					}
				}
			}
		
		private:
		
			unsigned int next;
			MenuRep menu;
	};
} // Common
