using namespace std;

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <map>

template<typename key, typename value>
class Dictionary {
	std::map<key, value> map;

	class mvalue {
	private:
		std::iterator iterator;
	public:
		mvalue(std::iterator i) : iterator(i) {}
		void operator=(value v) {
			*iterator = std::pair<key, value>((*iterator).first, v);
		}
	};

public:
	Dictionary(){}
	virtual ~Dictionary(){}
	void Add(key k, value v) {
		map.insert(std::pair<key, value>(k, v));
	}
	void Remove(key k) {
		map.erase(k);
	}
	void RemoveAll() {
		map.clear();
	}
	bool ContainsKey(string name) {
		return map.find(name) != map.end();
	}

	size_type Count() const {
		return map.size();
	}

	std::vector<key> Keys() const {
		std::vector<key> keys;
		std::transform(begin(), end(), std::back_inserter(keys), KEY);
		return keys;
	}

	std::vector<value> Items() const {
		std::vector<value> values;
		std::transform(begin(), end(), std::back_inserter(values), VALUE);
		return values;
	}

	mvalue operator[](key k) {
		return mvalue(map.find(k));
	}

protected:
	static key KEY(std::pair<key, value> p) {
		return p.first;
	}

	static key VALUE(std::pair<key, value> p) {
		return p.second;
	}
};
