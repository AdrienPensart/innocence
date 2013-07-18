#include <QAbstractTableModel>
#include <vector>

namespace TheSleeper
{
    class Client;
    
    class ClientTableModel : public QAbstractTableModel
    {
        Q_OBJECT
        
        public:
        
            ClientTableModel(QObject * parent=0);

            template<class Container>
            void set(const Container& clients)
            {
                currentClients.clear();
                currentClients.insert(currentClients.begin(), clients.begin(), clients.end());

                // mise à jour de la vue
                //reset();
            }

            int rowCount(const QModelIndex& parent) const;
            int columnCount(const QModelIndex& parent) const;
            QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
            QVariant headerData(int section, Qt::Orientation, int role) const;
            Client * get_client(const QModelIndex& index);
            
        private:
        
            // retourne le sleeper en cours de traitement
            Client * sleeperAt(int offset) const;
            // retourne l'information que demande la view (IP, Port, etc)
            QString infoAtColumn(Client * ptr, int offset) const;
            std::vector<Client *> currentClients;
    };
    
} /* TheSleeper */

