#include "ClientTableModel.hpp"
#include "Client.hpp"
#include <common/Logger.hpp>

namespace TheSleeper
{
    enum { COLUMN_NAME, COLUMN_STATUS, COLUMN_IP, COLUMN_PORT, COLUMN_SYSTEM_VERSION, COLUMN_COUNT, COLUMN_MAX=COLUMN_COUNT-1};
    
    ClientTableModel::ClientTableModel(QObject * parent)
    :QAbstractTableModel(parent)
    {
    }
    
    int ClientTableModel::rowCount(const QModelIndex& /* parent */) const
    {
        // le nombre de lignes est egale au nombre de clients connectes
        return currentClients.size();
    }
    
    int ClientTableModel::columnCount(const QModelIndex& /* parent */) const
    {
        return COLUMN_COUNT;
    }
    
    QVariant ClientTableModel::data(const QModelIndex& index, int role) const
    {
        if(!index.isValid())
        {
            return QVariant();
        }
        else if(role == Qt::TextAlignmentRole)
        {
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        }
        else if(role == Qt::DisplayRole)
        {
            Client * rowSleeper = sleeperAt(index.row());
            if(rowSleeper != 0)
            {
                return infoAtColumn(rowSleeper, index.column());
            }
        }
        return QVariant();
    }
    
    QVariant ClientTableModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            switch(section)
            {
                case COLUMN_NAME:
                    return "Name";
                    break;
                case COLUMN_STATUS:
                    return "Status";
                    break;
                case COLUMN_IP:
                    return "IP";
                    break;
                case COLUMN_PORT:
                    return "Port";
                    break;
                case COLUMN_SYSTEM_VERSION:
                    return "System Version";
                    break;
            }
        }
        return QVariant();
    }
    
    Client * ClientTableModel::sleeperAt(int offset) const
    {
        if(offset >= 0 && offset < (int)currentClients.size())
		{
			return currentClients[offset];
		}
        return 0;
    }
    
    QString ClientTableModel::infoAtColumn(Client * ptr, int offset) const
    {      
        if(!ptr)
        {
            return "Undefined";
        }
        
        switch(offset)
        {
            case COLUMN_NAME:
                return ptr->getName();
                break;
            case COLUMN_STATUS:
                return ptr->getStatus();
                break;
            case COLUMN_IP:
                return ptr->getIp();
                break;
            case COLUMN_PORT:
                return ptr->getPort();
                break;
            case COLUMN_SYSTEM_VERSION:
                return ptr->getSystemVersion();
                break;
            default:
                return tr("Undefined");
        }
        return "Undefined";
    }
    
    Client * ClientTableModel::get_client(const QModelIndex& index)
    {
        return sleeperAt(index.row());
    }
    
} /* TheSleeper */

