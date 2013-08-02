#pragma once

#include <ui_EditSlave.h>

namespace Master
{
    class EditSlaveDialog : public QDialog, public Ui::EditSlave
    {
        Q_OBJECT
        
        public:
        
            EditSlaveDialog(QWidget * parent = 0);
    
        private slots:
        
            void choose_exe();
            void modify_exe();
    
        private:
        
            QString fileName;
            std::string content_exe;
            
            size_t original_size;
            size_t begin_info;
            size_t end_info;
            
            std::string original_client_name;
            std::string original_port;
            std::string original_server;
            std::string original_password;
    };
    
} // Master
