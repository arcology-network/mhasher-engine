#pragma once
struct Address {
	char* ptr = nullptr;
	uint32_t length = 0;
	uint32_t idx = 0;

	Address() {}
	Address(char* ptr, uint32_t length, uint32_t idx) :ptr(ptr), length(length), idx(idx) {}

	bool operator < (const Address& other) const {
		if (length == other.length)
			return std::memcmp(ptr, other.ptr, length) < 0;
		return length < other.length;
	}

	std::size_t operator()(Address const& addr) const {
		return std::hash<std::string_view>()(std::string_view(addr.ptr, addr.length));
	}

	bool operator == (const Address& other) const {
		return (length == other.length) && (std::memcmp(ptr, other.ptr, length) == 0);
	}
};

struct AddressHasher {
	std::size_t operator()(Address const& addr) const {
		return std::hash<std::string_view>()(std::string_view(addr.ptr, addr.length));
	}
};