#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void help_using()
{
	cout << "buildnum [buildnum_header]\n";
}

template<typename T>
std::string toString( const T & Value )
{
    // utiliser un flux de sortie pour créer la chaîne
    std::ostringstream oss;
    // écrire la valeur dans le flux
    oss << Value;
    // renvoyer une string
    return oss.str();
}

template<typename T>
bool fromString( const std::string & Str, T & Dest )
{
    // créer un flux à partir de la chaîne donnée
    std::istringstream iss( Str );
    // tenter la conversion vers Dest
    return iss >> Dest != 0;
}

int main(int argc, char * argv[])
{
	if(argc != 2)
    {
        cout << "Le nombre de parametres est incorrect.\n";
		help_using();
        return EXIT_FAILURE;
    }
	
	ifstream headerin(argv[1]);
    if(headerin.fail())
    {
        cout << "Le fichier binaire " << argv[1] << " n'existe pas.\n";
        return EXIT_FAILURE;
    }
	
	std::stringstream buffer;
	buffer << headerin.rdbuf();

	headerin.close();
	ofstream headerout(argv[1], ios_base::trunc);
	string line;
	while (std::getline(buffer, line))
	{
		//cout << "Reading line : " << line << endl;
		std::istringstream iss(line);
		string define;
		string build;
		string num;
		if ((iss >> define >> build >> num))
		{
			int buildNum;
			fromString(num, buildNum);
			buildNum++;
			num = toString(buildNum);
			cout << "Updating Build Number to " << num << endl;
			string output = define + " " + build + " " + num + "\n";
			headerout << output;
		}
		else
		{
			cout << line << '\n';
			headerout << line << '\n';
		}
	}
	headerout.close();
	return EXIT_SUCCESS;
}
