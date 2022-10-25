#include <iostream>

#include "DespairEngine/Core.h"

using namespace std;

class Decay : public engine::Core
{
	virtual void update() override
	{
		// cout << "Frame: " << frame++ << endl;
	}
	int frame = 0;
};

int main()
{
	auto decay = std::make_unique<Decay>();
	int return_code = decay->start("Decay", 1920, 1080);
	return return_code;
}