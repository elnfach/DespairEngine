#include <DespairEngine/Core.hpp>

#include <iostream>

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
	auto decay = make_unique<Decay>();
	int return_code = decay->start("Decay", 1080, 720);
	return return_code;
}