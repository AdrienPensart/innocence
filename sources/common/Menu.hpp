#ifndef _MENU_HPP_
#define _MENU_HPP_

#include "Convert.hpp"

#include <iostream>
#include <map>
#include <string>
using namespace std;

class Callback
{
	public:
		
		Callback(const string& _label):label(_label){}
		virtual void execute()=0;
		const string& getLabel(){return label;}
		
	private:
	
		const string label;
};

class Menu
{
	typedef map<unsigned int, Callback *> MenuRep;
	public:
		
		Menu() : next(0)
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
				cout << iter->first << " -> " << iter->second->getLabel() << '\n';
			}
		}
		
		void prompt()
		{
			cout << "\n\tChoix : ";
			string buffer;
			unsigned int choice = 0;
			cin >> buffer;
			
			if(!from_string(buffer, choice))
			{
				if(next > 1)
				{
					cout << "Veuillez entrer un nombre entre 0 et " << next-1 << '\n';
				}
				else
				{
					cout << "Nombre invalide.\n";
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
					cout << "Entree utilisateur invalide.\n";
				}
			}
		}
		
	private:
		
		unsigned int next;
		MenuRep menu;
};

#endif _MENU_HPP_
