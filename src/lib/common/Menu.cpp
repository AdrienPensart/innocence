#include "Menu.hpp"
#include "Convert.hpp"

#include <iostream>
using namespace std;

Callback::Callback(const string& labelArg) : 
	label(labelArg)
{
}

const string& Callback::getLabel()
{
	return label;
}

Menu::Menu() : 
	next(0)
{
}
		
Menu::~Menu()
{
	for(MenuRep::iterator iter = menu.begin(); iter != menu.end(); iter++)
	{
		delete iter->second;
	}
}
		
void Menu::add(Callback * callback)
{
	menu[next] = callback;
	next++;
}
		
void Menu::show()
{
	for(MenuRep::iterator iter = menu.begin(); iter != menu.end(); iter++)
	{
		cout << iter->first << " -> " << iter->second->getLabel() << '\n';
	}
}
		
void Menu::prompt()
{
	cout << "\n\tChoix : ";
	string buffer;
	unsigned int choice = 0;
	cin >> buffer;
	
	if(!fromString(buffer, choice))
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
