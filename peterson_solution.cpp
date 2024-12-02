#include <iostream>
#include <thread> // for std::thread, std::this_thread
#include <chrono> // for std::chrono
#include <cstdlib> // for std::rand()
#include <ctime> // for std::time()
#include <array> // for std::array<>
#include <limits>

static constexpr std::size_t NUM_THREADS = 4;

template<std::size_t N>
class PeterMutex
{
	private:
		bool m_isLocked;
		bool m_interested[N];
		std::size_t m_id;
		bool isAnyInterestedExcept(std::size_t id) const noexcept
		{
			for(std::size_t i = 0; i < N; ++i)
				if(m_interested[i] && (i != id))
					return true;
			return false;
		}
	public:
		PeterMutex() noexcept : m_isLocked(false), m_id(std::numeric_limits<std::size_t>::max())
		{
			for(std::size_t i = 0; i < N; ++i)
				m_interested[i] = std::numeric_limits<std::size_t>::max();
		}
		void lock(std::size_t id) noexcept
		{
			m_interested[id] = true;
			while(m_isLocked);
			m_id = id;
			while((m_id != id) && isAnyInterestedExcept(id));
			m_id = id;
			m_isLocked = true;
		}
		void unlock(std::size_t id) noexcept
		{
			m_interested[id] = false;
			m_isLocked = false;
		}
};

static std::size_t gValue = 0;
static PeterMutex<NUM_THREADS> gMutex;

static void increment(std::size_t id) noexcept
{
	for(std::size_t i = 0; i < 4; ++i)
	{
		gMutex.lock(id);
		gValue += 1;
		gMutex.unlock(id);
	}
}

int main()
{
	std::srand(std::time(nullptr));
	std::array<std::thread, NUM_THREADS> threads;
	for(std::size_t i = 0; i < NUM_THREADS; ++i)
		threads[i] = std::thread(increment, i);
	for(std::size_t i = 0; i < NUM_THREADS; ++i)
		threads[i].join();
	std::cout << "Running 5 times" << std::endl;
	for(std::size_t i = 0; i < 5; ++i)
	{
		if(gValue != (NUM_THREADS * 4))
			std::cout << "Race condition is not handled correctly" << std::endl;
		else
			std::cout << "Final value: " << gValue << std::endl;
	}
	return 0;
}
