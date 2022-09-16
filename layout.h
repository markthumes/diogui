#ifndef __layout__
#define __layout__

#define CLEVEL_IP "192.168.10.9"
#define CLEVEL_PORT 54321

#include <QLayout>
#include <QPushButton>
#include <QTimer>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QUdpSocket>
#include <QCoreApplication>

class Bit : public QWidget{
	Q_OBJECT
public:
	Bit(QString name){
		box = new QGroupBox(name);
		check = new QCheckBox();
//		check->setDisabled(true);
		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(check);	
		box->setLayout(layout);
		QGridLayout* mainLayout = new QGridLayout;
		mainLayout->addWidget(box);
		setLayout(mainLayout);
	}
public slots:
	void set(bool chk){
		check->setChecked(chk);
	}
private:
	QGroupBox *box;
	QCheckBox *check;
};

class Layout : public QWidget{
	Q_OBJECT
public:
	Layout();
public slots:
	void sample();
	void setTimer(double);
	void setGPIOInput( uint8_t reg_id );
	void automate(bool);
private:
	void addControls();
	QLineEdit   *hex[3];
	QLabel      *label_period;
	QDoubleSpinBox *spin_period;
	QPushButton *btn;
	QUdpSocket* udp;
	QTimer      *timer;
	uint32_t data[3];
	const char* m_ip;
	Bit *bit[3][32];
	QCheckBox *check_auto;
	QLabel *label_auto;
};

#endif
