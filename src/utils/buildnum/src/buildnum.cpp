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
    std::ostringstream oss;
    oss << Value;
    return oss.str();
}

template<typename T>
bool fromString( const std::string & Str, T & Dest )
{
    std::istringstream iss( Str );
    return iss >> Dest != 0;
}

int main(int argc, char * argv[])
{
	if(argc != 2)
    {
        cout << "Incorrect parameters.\n";
		help_using();
        return EXIT_FAILURE;
    }
	
	ifstream headerin(argv[1]);
    if(headerin.fail())
    {
        cout << "Header " << argv[1] << " does not exist.\n";
        return EXIT_FAILURE;
    }
	
	std::stringstream buffer;
	buffer << headerin.rdbuf();

	headerin.close();
	ofstream headerout(argv[1], ios_base::trunc);
	string line;
	while (std::getline(buffer, line))
	{
		std::istringstream iss(line);
		string define;
		string build;
		string num;
		if ((iss >> define >> build >> num) && define == "#define" && build == "BUILD")
		{
			int buildNum;
			fromString(num, buildNum);
			buildNum++;
			num = toString(buildNum);
			cout << "Updating Build Number to " << num;
			string output = define + " " + build + " " + num + "\n";
			headerout << output;
		}
		else
		{
			headerout << line << '\n';
		}
	}
	headerout.close();
	return EXIT_SUCCESS;
}

