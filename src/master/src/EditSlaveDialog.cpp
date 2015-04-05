#include "EditSlaveDialog.hpp"

#include <common/Innocence.hpp>
#include <QFileDialog>
#include <QMessageBox>
#include <log/Log.hpp>
#include <system/File.hpp>
using namespace std;

namespace Master {
	EditSlaveDialog::EditSlaveDialog(QWidget * parent)
		:QDialog(parent) {
		setupUi(this);

		connect(selectSlaveButton, SIGNAL(clicked()), this, SLOT(choose_exe()));
		connect(modifyExeButton, SIGNAL(clicked()), this, SLOT(modify_exe()));
		connect(cancelEditorButton, SIGNAL(clicked()), this, SLOT(close()));

		original_size = 0;
		begin_info = 0;
		end_info = 0;
	}

	void EditSlaveDialog::choose_exe() {
		// on trouve le chemin du client a editer
		fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"C:\\",tr("Executable (*.exe)"));
		if(fileName.size()) {
			slaveSelectedBox->setText(fileName);

			// on a trouvé le fichier, on va maintenant essayé d'extraire les informations que
			// contient l'exécutable

			// chargement de tout l'exécutable en mémoire :
			std::fstream input_exe(fileName.toStdString().c_str(), std::ios::in | std::ios::binary);
			content_exe = System::ReadAll(input_exe);
			input_exe.close();
			original_size = content_exe.size();

			begin_info = content_exe.find(MARKER);
			end_info = content_exe.find(MARKER, begin_info+MARKER_SIZE);
			begin_info += MARKER_SIZE;

			string buffer = content_exe.substr(begin_info, end_info-begin_info);
			istringstream iss(buffer);

			std::getline( iss, original_server, SEPERATOR );
			std::getline( iss, original_port, SEPERATOR );
			std::getline( iss, original_client_name, SEPERATOR );
			std::getline( iss, original_password, SEPERATOR );

			LOG << original_client_name;
			LOG << original_port;
			LOG << original_server;
			LOG << original_password;

			sourceSlaveNameEdit->setText(original_client_name.c_str());
			sourceMasterIpEdit->setText(original_server.c_str());
			sourceMasterPortEdit->setText(original_port.c_str());
		}
	}

	void EditSlaveDialog::modify_exe() {
		if(!System::isReadable(fileName.toStdString())) {
			QMessageBox::critical(this, windowTitle(),tr("Cliquez sur \"Parcourir...\" afin de choisir un client a editer."));
			return;
		}

		if(!destinationSlaveNameEdit->text().size() || !destinationMasterIpEdit->text().size() || !destinationMasterPortEdit->text().size() || !destinationSlavePasswordEdit->text().size()) {
			QMessageBox::critical(this, windowTitle(),tr("Veuillez entrer toutes les informations de connexion."));
			return;
		}

		std::string new_client_name = destinationSlaveNameEdit->text().toStdString();
		std::string new_port = Common::toString(destinationMasterPortEdit->value());
		std::string new_server = destinationMasterIpEdit->text().toStdString();
		std::string new_password = destinationSlavePasswordEdit->text().toStdString();

		string buffer = new_server + SEPERATOR + new_port + SEPERATOR + new_client_name + SEPERATOR + new_password + MARKER;

		if(buffer.size() >= Common::CONNECTION_INFO_SIZE - MARKER_SIZE) {
			QMessageBox::critical(this, windowTitle(),tr("Les informations que vous avez entrées sont trop longues."));
		}

		for(size_t index = begin_info ; index != begin_info + Common::CONNECTION_INFO_SIZE - MARKER_SIZE ; index++) {
			content_exe[index] = '\0';
		}

		content_exe.replace(begin_info, buffer.size(), buffer, 0, buffer.size());

		QFile::remove(fileName);
		std::ofstream output_exe(fileName.toStdString().c_str(), std::ios::binary);
		output_exe.write(content_exe.c_str(), content_exe.size());
	}

} // Master
