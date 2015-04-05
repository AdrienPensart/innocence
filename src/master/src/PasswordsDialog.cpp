#include "PasswordsDialog.hpp"

#include <fstream>
#include <QFileDialog>

namespace Master {
	PasswordsDialog::PasswordsDialog(QWidget * parent)
		:QDialog(parent) {
		setupUi(this);
		connect(savePasswordsButton, SIGNAL(clicked()), this, SLOT(save_passwords()));
		passwordsView->setModel(&passwordsModel);
	}

	void PasswordsDialog::save_passwords() {
		QStringList password_list = ((QStringListModel*)passwordsView->model())->stringList();
		if(password_list.size()) {
			QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"C:\\",tr("Text (*.txt)"));
			if(fileName.size()) {
				std::fstream file;
				file.open(fileName.toStdString().c_str(), std::ios::out | std::ios::app);
				file.write(password_list.join("\n").toStdString().c_str(), password_list.join("\n").toStdString().size());
			}
		}
	}

	void PasswordsDialog::setPasswords(const QStringList& passwords_list) {
		passwordsModel.setStringList(passwords_list);
	}
} // Master
