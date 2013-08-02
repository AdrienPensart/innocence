#pragma once

#include <QtGui>
#include <ui_Passwords.h>

namespace Master
{
    class PasswordsDialog : public QDialog, public Ui::PasswordsDialog
    {
        Q_OBJECT
        
        public:
        
            PasswordsDialog(QWidget * parent = 0);
            void setPasswords(const QStringList& passwords_list);
            
        private slots:
        
            void save_passwords();
            
        private:
            
            QStringListModel passwordsModel;
    };
} // Master
