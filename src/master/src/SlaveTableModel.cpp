#include "SlaveTableModel.hpp"
#include "Slave.hpp"
#include <common/Log.hpp>

namespace Master
{
    enum { COLUMN_NAME, COLUMN_STATUS, COLUMN_IP, COLUMN_PORT, COLUMN_SYSTEM_VERSION, COLUMN_COUNT, COLUMN_MAX=COLUMN_COUNT-1};
    
    SlaveTableModel::SlaveTableModel(QObject * parent)
    :QAbstractTableModel(parent)
    {
    }
    
    int SlaveTableModel::rowCount(const QModelIndex& /* parent */) const
    {
        // le nombre de lignes est egale au nombre de clients connectes
        return currentSlaves.size();
    }
    
    int SlaveTableModel::columnCount(const QModelIndex& /* parent */) const
    {
        return COLUMN_COUNT;
    }
    
    QVariant SlaveTableModel::data(const QModelIndex& index, int role) const
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
            Slave * rowSlave = slaveAt(index.row());
            if(rowSlave != 0)
            {
                return infoAtColumn(rowSlave, index.column());
            }
        }
        return QVariant();
    }
    
    QVariant SlaveTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
    
    Slave * SlaveTableModel::slaveAt(int offset) const
    {
        if(offset >= 0 && offset < (int)currentSlaves.size())
		{
			return currentSlaves[offset];
		}
        return 0;
    }
    
    QString SlaveTableModel::infoAtColumn(Slave * ptr, int offset) const
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
    
    Slave * SlaveTableModel::getSlave(const QModelIndex& index)
    {
        return slaveAt(index.row());
    }
    
} // Master

