#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
using namespace std;

long filesize(std::ifstream & Fichier) {
	// sauvegarder la position courante
	long pos = Fichier.tellg();
	// se placer en fin de fichier
	Fichier.seekg( 0 , std::ios_base::end );
	// récupérer la nouvelle position = la taille du fichier
	long size = Fichier.tellg() ;
	// restaurer la position initiale du fichier
	Fichier.seekg( pos,  std::ios_base::beg ) ;
	return size ;
}

struct to_upper_string {
	char operator()(char c) const {
		return std::toupper(static_cast<unsigned char>(c));
	}
};

void help_using() {
	cout << "bin2array [binary_file(input)] [header_filename(output)] [array_name(input/output)]\n";
}

int main(int argc, char * argv[]) {
	if(argc != 4) {
		cout << "Le nombre de parametres est incorrect.\n";
		help_using();
		return EXIT_FAILURE;
	}

	// le parametre n°1 est le fichier binaire a convertir
	// test de son existence
	ifstream binary_file(argv[1], std::ios_base::binary);
	if(binary_file.fail()) {
		cout << "Le fichier binaire " << argv[1] << " n'existe pas.\n";
		return EXIT_FAILURE;
	}

	// on essai d'effacer le fichier de destination de la conversion
	remove(argv[2]);

	// le parametre n°2 est le fichier qui contiendra le tableau C
	// une fois le fichier binaire converti.
	std::ofstream array_file(argv[2]);

	// on verifie si l'on peut créer le fichier
	if(!array_file) {
		cout << "Impossible de creer le fichier " << argv[2] << "\n";
		return EXIT_FAILURE;
	}

	// le troisième parametre est le nom du tableau d'octet
	// ecriture de l'entete de protection contre l'auto inclusion
	string header_name;
	header_name = "_";
	header_name += argv[3];
	header_name += "_";

	// conversion du nom de fichier en majuscule
	std::transform(header_name.begin(), header_name.end(), header_name.begin(), to_upper_string());
	const long file_size = filesize(binary_file);

	array_file << "#ifndef " << header_name
	           << "\n#define " << header_name << "\n\n";

	array_file << "static const unsigned char " << argv[3]
	           << "[" << file_size
	           << "] =\n{\n";

	unsigned char charact = 0;

	while(binary_file.read(reinterpret_cast<char*>(&charact), sizeof(unsigned char))) {
		array_file /*<< "0x" << hex*/ << (int)charact << ',';
	}

	array_file << "\n};\n\n#endif //" << header_name << "\n";
	binary_file.close();
	array_file.close();

	cout << argv[2] << " generated.\n";

	return EXIT_SUCCESS;
}
