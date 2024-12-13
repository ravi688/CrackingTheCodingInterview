#include <iostream>

namespace SimulationEngines::Physics::ElectroMagnetics
{
	static void doSimulation() noexcept
	{
		std::cout << "Doing ElectroMagentic simulation" << std::endl;
	}
}

int main()
{
	SimulationEngines::Physics::ElectroMagnetics::doSimulation();
	return 0;
}
