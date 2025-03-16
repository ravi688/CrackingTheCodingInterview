# Sequence

| Seq | File name | Description |
| ----|-----------|-------------|
| 1. | launching_a_thread.cpp | Launching a Thread |
| 2. | waiting_for_a_thread_to_finish.cpp | Waiting for a thread to finish (in try-catch also) |
| 3. | thread_guard_raii.cpp | A thread_guard object to automatically join an acquired thread when it goes out of the scope |
| 4. | scoped_thread_raii.cpp | A scoped_thread object which acquires (and takes ownership) a `std::thread` object upon construction, and join that when it goes out of the scope |

> [!Note]
> The `std::terminate()` isn't called when `std::thread` object is destroyed <br>
> if that `std::thread` has been detached by calling `std::thread::detach()` on it.

> [!Tip]
> `scoped_thread(std::thread(myFunction))` is more informative, and it tells the actual intent unlike `scoped_thread(myThread)` which would accept reference to existing `std::thread`.
