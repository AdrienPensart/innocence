#pragma once

#include <QProgressBar>
#include <blaspheme/transfer/FileTransfer.hpp>

namespace Master {
	class GraphicProgressBar : public Blaspheme::TransferObserver {
		public:

			GraphicProgressBar(QProgressBar * _bar)
				:bar(_bar) {
			}

			void update(const Blaspheme::TransferDescriptor& td) {
				bar->setValue(td.percentage);
			};

		private:

			QProgressBar * bar;
	};

} // Master
