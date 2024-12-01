#include <iostream>
#include <thread> // for std::this_thred
#include <mutex> // for std::mutex
#include <cstdlib> // for rand() and srand()
#include <ctime> // for std::time()

static constexpr std::size_t NUM_PHILOSOPHERS = 5;

static std::mutex gStdOutMutex;
static std::mutex gRandMutex;
// This may introduce false-sharing but that is not what we are concerned about here for now
static std::size_t gPhilosophersMaxEat[NUM_PHILOSOPHERS];
static std::size_t gPhilosophersEat[NUM_PHILOSOPHERS];
// Energy of a philosohper will decrease every time he thinks
static std::size_t gEnergyDecreaseRate = 1;
// Energy of a philosohper will increase every time he eats
static std::size_t gEnergyIncreaseRate = 1;

// thread safe rand() function
static int tsRand() noexcept
{
	std::lock_guard lk(gRandMutex);
	return std::rand();
}

static void think(std::size_t id) noexcept
{
	{
		std::lock_guard lk(gStdOutMutex);
		std::cout << "[" << id << "] " << "Thinking" << std::endl;
	}
	int randValue = tsRand();
	std::this_thread::sleep_for(std::chrono::milliseconds(randValue % 1000));
	if(gPhilosophersEat[id] >= gEnergyDecreaseRate)
		gPhilosophersEat[id] -= gEnergyDecreaseRate;
	else
		gPhilosophersEat[id] = 0;
}

static void eat(std::size_t id) noexcept
{
	{
		std::lock_guard lk(gStdOutMutex);
		std::cout << "[" << id << "] " << "Eating" << std::endl;
	}
	int randValue = tsRand();
	std::this_thread::sleep_for(std::chrono::milliseconds(randValue % 1000));
	gPhilosophersEat[id] == std::min(gPhilosophersMaxEat[id], gPhilosophersEat[id] + gEnergyIncreaseRate);
}

static inline bool isFull(std::size_t id) noexcept
{
	return gPhilosophersEat[id] >= gPhilosophersMaxEat[id];
}

static inline bool isStarving(std::size_t id) noexcept
{
	return gPhilosophersEat[id] == 0;
}

static bool tryGrabForks(std::size_t id) noexcept
{
	return true;
}

static void philosopher(std::size_t id) noexcept
{
	while(!isFull(id))
	{
		if(tryGrabForks(id))
			eat(id);
		else
		{
			think(id);
			if(isStarving(id))
			{
				std::lock_guard lk(gStdOutMutex);
				std::cout << "[" << id << "] " << "I'm Starving" << std::endl;
			}
		}
	}
	std::lock_guard lk(gStdOutMutex);
	std::cout << "[" << id << "] " << "I'm Full" << std::endl;
}

int main(int argc, const char* argv[])
{
	// NOTE: the return type of std::time() is not always guaranteed to be integral type, it is implementation dependent
	std::srand(std::time(0));

	// Initialize the maximum food that each philosphers can eat
	for(std::size_t i = 0; i < NUM_PHILOSOPHERS; ++i)
	{
		gPhilosophersMaxEat[i] = std::max(5, std::rand() % 20);
		// Initially a philosopher will be starving
		gPhilosophersEat[i] = 0;
	}

	// Default constructors are automatically called when creating arrays
	std::thread threads[NUM_PHILOSOPHERS];
	for(std::size_t i = 0; i < NUM_PHILOSOPHERS; ++i)
	{
		// Right side is r value, so move constructor will be invoked
		threads[i] = std::thread(philosopher, i);
	}

	for(std::size_t i = 0; i < NUM_PHILOSOPHERS; ++i)
		threads[i].join();
	return 0;
}
