#include <ui_EditClient.h>

namespace TheSleeper
{
    class EditClientDialog : public QDialog, public Ui::EditClient
    {
        Q_OBJECT
        
        public:
        
            EditClientDialog(QWidget * parent = 0);
    
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
    
} /* TheSleeper */
