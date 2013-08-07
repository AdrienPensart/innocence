#pragma once

#include <map>
#include <string>

class Callback
{
	public:
		
		Callback(const std::string& label);
		virtual void execute()=0;
		const std::string& getLabel();
		
	private:
	
		const std::string label;
};

class Menu
{
	typedef std::map<unsigned int, Callback *> MenuRep;

	public:
		
		Menu();
		~Menu();
		void add(Callback * callback);
		void show();
		void prompt();
		
	private:
		
		unsigned int next;
		MenuRep menu;
};
