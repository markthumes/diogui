#include "layout.h"

Layout::Layout(){
	addControls();

	for( int i = 0; i < 3; i++ )
		setGPIOInput(i);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(label_period, 0, 0);
	mainLayout->addWidget(spin_period, 0, 1);
	mainLayout->addWidget(label_auto, 1, 0);
	mainLayout->addWidget(check_auto, 1, 1);
	for( int i = 0; i < 3; i++ )
		mainLayout->addWidget(hex[i], 3, i);
	mainLayout->addWidget(btn, 2, 1);
	QGroupBox *group[3];
	QGridLayout *hbox[3];
	for( int j = 0; j < 3; j++ ){
		QString name("J");
		name.append(QString::number(j));
		group[j] = new QGroupBox(name);
		hbox[j] = new QGridLayout;
		for( int i = 0; i < 32; i++ ){
			bit[j][31-i] = new Bit(QString::number(31-i));
			hbox[j]->addWidget(bit[j][31-i], i/16, i%16);
		}
		group[j]->setLayout(hbox[j]);
		mainLayout->addWidget(group[j], 4+j*3, 0, 1, 3);
	}
	setLayout(mainLayout);
	setTimer(0.05);

	connect(timer, SIGNAL(timeout()), this, SLOT(sample()));
	connect(btn,   SIGNAL(clicked()), this, SLOT(sample()));
	connect(spin_period, SIGNAL(valueChanged(double)), this,
		SLOT(setTimer(double)));
	connect(check_auto, SIGNAL(clicked(bool)), this, 
		SLOT(automate(bool)));
	timer->start();
}

void Layout::automate(bool b){
	if(b) timer->start();
	else  timer->stop();
}

void Layout::addControls(){
	btn = new QPushButton("Sample Data");
	timer = new QTimer;
	for( int i = 0; i < 3; i++ ){
		hex[i] = new QLineEdit;
//		hex[i]->setDisabled(true);
	}
	spin_period = new QDoubleSpinBox;
	spin_period->setValue(0.05);
	spin_period->setMinimum(0.05);
	spin_period->setMaximum(100.00);
	spin_period->setSingleStep(0.01);
	label_period = new QLabel("Sample Rate (s)");
	label_period->setAlignment(Qt::AlignRight);

	check_auto = new QCheckBox;
	check_auto->setChecked(true);
	label_auto = new QLabel("Auto Sample:");
	label_auto->setAlignment(Qt::AlignRight);

	udp = new QUdpSocket;
}

void Layout::setTimer(double v){
	timer->setInterval(v*1000);
}

void Layout::setGPIOInput( uint8_t reg_id ){
	uint8_t message[3];
	memset(message, 0, 3);
	message[0] = 8;
	message[1] = 2;
	message[2] = reg_id;
	udp->writeDatagram((char*)message, 3, 
		QHostAddress(CLEVEL_IP), CLEVEL_PORT);
	while( !udp->hasPendingDatagrams() )
		QCoreApplication::processEvents();
	udp->readDatagram((char*)message, 3);
}

void Layout::sample(){
	uint8_t message[14];
	memset(message, 0, 14);
	message[0] = 11;
	message[1] = 2;
	udp->writeDatagram((char*)message,14, 
		QHostAddress(CLEVEL_IP), CLEVEL_PORT);
	while( !udp->hasPendingDatagrams() )
		QCoreApplication::processEvents();
	udp->readDatagram((char*)message,14);
	uint32_t *bswap = (uint32_t*)&(message[2]);
	for( int i = 0; i < 3; i++ ){
		data[i] = bswap[i];
		QString temp = QString("0x%1").arg(data[i], 8, 16, 
			QLatin1Char('0'));
		hex[i]->setText(temp);
		for( uint8_t j = 0; j < 32; j++ ){
			if( (data[i] & ( (uint32_t)0x01 << j )) > 0 )
				bit[i][j]->set(true);	
			else
				bit[i][j]->set(false);	
		}
	}
}
