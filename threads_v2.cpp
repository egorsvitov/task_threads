#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <numeric>
#include <vector>
#include <chrono>

using steady_clock = std::chrono::steady_clock;

const size_t BORDER_SIZE = 1000u;
const unsigned int NUMBER_OF_POINTS = 12;

template <typename Iterator, typename T>
T parallel_accumulate(Iterator begin, Iterator end, T init, unsigned int n)
{
    auto size = std::distance(begin, end);
    if (size <= BORDER_SIZE) {
        return std::accumulate(begin, end, 0);
    }
    unsigned int num_threads;
    if (n > 0) {
        num_threads = n;
    } else {
        num_threads = std::thread::hardware_concurrency();
    }
    auto block_size = size / num_threads;

    std::vector<Iterator> borders;
    borders.push_back(begin);
    for (int i = 1; i < num_threads; i++) {
        borders.push_back(std::next(begin, i*size/num_threads));
    }
    borders.push_back(end);

    std::vector<std::future<T>> futures;
    for (auto i = 0; i < num_threads; i++) {
        futures.push_back(std::async(std::launch::async, std::accumulate<Iterator, T>, borders[i], borders[i+1], *begin - *begin)); 
    }

    auto result = init;
    for (auto i = 0; i < num_threads; i++) {
        result += futures[i].get();
    }

    return result;
}

int main() {
    std::vector<int> array(100'000);
    std::iota(array.begin(), array.end(), 1);
    
    std::ofstream n;
	n.open("data.txt");
	n.close();

    std::ofstream out;
    out.open("data.txt", std::ios::app);

    for (auto i = 1; i <= NUMBER_OF_POINTS; i++) {
		auto start = std::chrono::high_resolution_clock::now();
        parallel_accumulate(array.begin(), array.end(), 0, i);
		auto end = std::chrono::high_resolution_clock::now();
    	out << std::to_string(i) << " " << std::to_string((end-start).count()) << std::endl;
	}

    out.close();
}