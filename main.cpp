#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>
#include <thread>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

using size_t = std::size_t;
using string = std::string;
using StringVector = std::vector<string>;
using IntVector = std::vector<int>;

struct FileParams {
	string src;
	int start;
	int end;
};

void Read(const FileParams &params) {
	std::ifstream stream(params.src);
	stream.seekg(params.start);
	string buf;
	while (stream.tellg() < params.end && std::getline(stream, buf)) {
		std::cout << buf << std::endl;
	};
	std::cout << std::endl;
}

IntVector Split(const int n, const string &src) {
	fs::path path(src);
	if (!fs::exists(path)) {
		std::cerr << "Failed to find: " << src << std::endl;
		throw;
	}
	const int file_size = fs::file_size(path);
	const int bulk_size = fs::file_size(path) / n;
	std::vector<int> pos{0};
	std::ifstream stream(src);
	for (int i(1); i < n; ++i) {
		const int start = i * bulk_size;
		std::cout << "File pos: " << start << std::endl;
		stream.seekg(start);
		string buf;
		std::getline(stream, buf);
		pos.push_back(start + buf.size());
	}
	pos.push_back(file_size);
	return pos;
}

template<typename T>
T GetArg(int argc, char *argv[], const int ind) {
	assert(argc >= ind + 1);
	std::stringstream ss(argv[ind]);
	T n;
	ss >> n;
	return n;
}

int main(int argc, char *argv[]) {
	try {
		const auto src = GetArg<string>(argc, argv, 1);
		const auto m = GetArg<int>(argc, argv, 2);
		auto points = Split(m, src);

		std::vector<std::thread> readers;
		for (size_t i(1); i < points.size(); ++i) {
			FileParams params{src, points[i - 1], points[i]};
			readers.emplace_back(Read, std::move(params));
		}
		for (auto &r : readers) {
			r.join();
		}
	} catch(const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
