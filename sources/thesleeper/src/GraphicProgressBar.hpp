#ifndef GRAPHIC_PROGRESS_BAR
#define GRAPHIC_PROGRESS_BAR

#include <QProgressBar>
#include <blaspheme/transfer/FileTransfer.hpp>

namespace TheSleeper
{
    class GraphicProgressBar : public Blaspheme::TransferObserver
    {
        public:
            
			GraphicProgressBar(QProgressBar * _bar)
				:bar(_bar)
			{
			}

			void update(const Blaspheme::TransferDescriptor& td)
            {
				bar->setValue(td.percentage);
            };

		private:
			
			QProgressBar * bar;
    };

} /* TheSleeper */

#endif
