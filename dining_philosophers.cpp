#include <iostream>
#include <thread> // for std::this_thred
#include <mutex> // for std::mutex
#include <cstdlib> // for rand() and srand(), and std::malloc/std::free
#include <ctime> // for std::time()
#include <semaphore> // for std::binary_semaphore

static constexpr std::size_t NUM_PHILOSOPHERS = 5;

static std::mutex gStdOutMutex;
static std::mutex gRandMutex;
// This may introduce false-sharing but that is not what we are concerned about here for now
static std::size_t gPhilosophersMaxEat[NUM_PHILOSOPHERS];
static std::size_t gPhilosophersEat[NUM_PHILOSOPHERS];
// std::counting_semaphore (or std::binary_semaphore) doesn't have default constructors nor move constructors or move assignment operator overloads
// Therefore, we have to create a raw memory buffer and use placement new to initialize each std::binary_semaphore object in that.
static std::binary_semaphore* gForks;
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
	gPhilosophersEat[id] = std::min(gPhilosophersMaxEat[id], gPhilosophersEat[id] + gEnergyIncreaseRate);
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
	std::size_t fork1 = id;
	std::size_t fork2 = (id + 1) % NUM_PHILOSOPHERS;

	// Solution no 1:
	// Use a big lock for all the forks; and before checking acquire the lock and release it when we are done with it.
	// Main issue: in-efficient, serializes the fork grabbing if we multiple philosophers can acquire their pair of forks simulateneously.
	//
	// Solution no 2:
	// Use one mutex per fork; grabbing a fork would be equivalent to acquiring the lock on the fork's mutex
	//
	// Solution no 3:
	// Use one binary semaphore per fork; grabbing a fork would be equivalent to decrementing (acquiring) the semaphore

	bool isFork1Acquired = gForks[fork1].try_acquire();
	bool isFork2Acquired = gForks[fork2].try_acquire();

	if(isFork1Acquired && isFork2Acquired)
	{
		std::lock_guard lk(gStdOutMutex);
		std::cout << "[" << id << "] Grabbed forks" << std::endl;
		return true;
	}
	else
	{
		if(isFork1Acquired)
			gForks[fork1].release();
		else if(isFork2Acquired)
			gForks[fork2].release();
	}
	return false;
}

static void releaseForks(std::size_t id) noexcept
{
	std::size_t fork1 = id;
	std::size_t fork2 = (id + 1) % NUM_PHILOSOPHERS;
	gForks[fork1].release();
	gForks[fork2].release();
	std::lock_guard lk(gStdOutMutex);
	std::cout << "[" << id << "] Released forks" << std::endl;
}

static void philosopher(std::size_t id) noexcept
{
	while(!isFull(id))
	{
		if(tryGrabForks(id))
		{
			eat(id);
			releaseForks(id);
		}
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

	gForks = static_cast<std::binary_semaphore*>(std::malloc(sizeof(std::binary_semaphore) * NUM_PHILOSOPHERS));

	// Initialize the maximum food that each philosphers can eat
	for(std::size_t i = 0; i < NUM_PHILOSOPHERS; ++i)
	{
		gPhilosophersMaxEat[i] = std::max(5, std::rand() % 20);
		// Initially a philosopher will be starving
		gPhilosophersEat[i] = 0;
		// Initially a fork will be in Released state; i.e. no philosopher has grabbed it
		new (gForks + i) std::binary_semaphore { 1 };
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

	std::free(static_cast<void*>(gForks));
	return 0;
}
