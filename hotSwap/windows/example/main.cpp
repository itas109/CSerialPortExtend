#include <iostream>
using namespace std;

#include "CSerialPortHotSwap.h"
using namespace itas109;

class mySlot : public has_slots<>
{
public:
	mySlot(){};

	void OnPortChange(int portStatus, std::string portName)
	{
		//std::cout << portStatus << portName.c_str() << endl;
		printf("portStatus : %d, portName : %s \n", portStatus, portName.c_str());
	};
};

int main()
{
	mySlot receive;

	CSerialPortHotSwap m_portHotSwap;
	m_portHotSwap.portChangeSignal.connect(&receive, &mySlot::OnPortChange);
	
	std::cout << "Version : " << m_portHotSwap.getVersion() << std::endl << std::endl;

	while (true);

	return 1;
}