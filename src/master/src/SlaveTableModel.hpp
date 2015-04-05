#pragma once

#include <QAbstractTableModel>
#include <vector>

namespace Master {
	class Slave;

	class SlaveTableModel : public QAbstractTableModel {
			Q_OBJECT

		public:

			SlaveTableModel(QObject * parent=0);

			template<class Container>
			void set(const Container& slaves) {
				currentSlaves.clear();
				currentSlaves.insert(currentSlaves.begin(), slaves.begin(), slaves.end());

				// mise à jour de la vue
				//reset();
			}

			int rowCount(const QModelIndex& parent) const;
			int columnCount(const QModelIndex& parent) const;
			QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
			QVariant headerData(int section, Qt::Orientation, int role) const;
			Slave * getSlave(const QModelIndex& index);

		private:

			// retourne le slave en cours de traitement
			Slave * slaveAt(int offset) const;
			// retourne l'information que demande la view (IP, Port, etc)
			QString infoAtColumn(Slave * ptr, int offset) const;
			std::vector<Slave *> currentSlaves;
	};

} // Master
