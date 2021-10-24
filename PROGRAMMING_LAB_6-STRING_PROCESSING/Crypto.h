#pragma once
#define __CRYPTO__
#include <iostream>
#include <sstream>
#include <string>

#include <ctype.h>
#include <cmath>

bool isstop(const char c) {
	return c == '?' || c == '!' || c == '.';
}

namespace crypto {
	template <typename _ST, typename _KT>
	class encryption_base {
	public:
		virtual size_t len() const = 0;
		virtual _ST encrypt(_KT) const = 0;
		virtual _ST decrypt(_KT) const = 0;
		virtual void sanitize() { };

		virtual const _ST &get() const = 0;
		virtual void set(_ST) { }
	};

	typedef encryption_base<std::string, int> enc_base;

	class caesar : public enc_base {
	public:
		caesar() = default;
		caesar(std::string _s) : _M_str{_s} {}
		size_t len() const {
			return _M_str.size();
		}

		// Thanks to Adrian for the improv mod formula
		static char offset(char c, int k) {
			c -= 'A';
			c += k - 26 * (int) floor(k / 26.);
			c %= 26;
			c += 'A';
			return c;
		}

		std::string encrypt(int key) const {
			std::string _e = _M_str;
			for (auto i = _e.begin(); i != _e.end(); ++i) {
				if (!isalpha(*i)) continue;
				bool _il = islower(*i);
				*i = toupper(*i);

				*i = offset(*i, key);
				
				/**i -= 'A';
				char s = *i + (key % 26);
				char r = s % 26;
				*i = r + 'A';*/

				if (_il) *i = tolower(*i);
			}

			return _e;
		}

		std::string decrypt(int key) const {
			std::string _e = _M_str;
			for (auto i = _e.begin(); i != _e.end(); ++i) {
				if (!isalpha(*i)) continue;
				bool _il = islower(*i);
				*i = toupper(*i);

				*i = offset(*i, -key);

				//*i -= 'A';
				//char s = *i - (key % 26);
				//char r = (s + 26) % 26; // https://stackoverflow.com/a/43827557
				//*i = r + 'A';

				if (_il) *i = tolower(*i);
			}

			return _e;
		}
		
		void sanitize() {
			std::stringstream ss(_M_str);
			_M_str.clear();

			std::string _bf;
			bool snt = true;
			while (ss.good() && (ss >> _bf)) {
				if (snt && !(snt = false)) 
					_bf.at(0) = toupper(_bf.at(0));
				for (auto i = _bf.begin() + 1; i != _bf.end(); ++i) {
					*i = tolower(*i);
				}

				_M_str.append(_bf);
				if (ss.good()) _M_str.append(1, ' ');
				if (isstop(_bf.back())) snt = true;
			}
		}

		const std::string &get() const {
			return _M_str;
		}

		void set(std::string _s) {
			_M_str = _s;
		}

		// I spent a lot of time writing event callbacks that relied
		// on the fact that the message property in the global context
		// namespace was a string. Instead of changing all of that code,
		// I override assignment operator to circumvent it. Isn't that
		// something?
		void operator=(std::string _s) {
			set(_s);
		}

		friend std::ostream &operator<<(std::ostream &os, const crypto::caesar &cipher);

	private:
		std::string _M_str;
	};

	// Same thing goes for this one
	std::ostream &operator<<(std::ostream &os, const crypto::caesar &cipher) {
		os << cipher._M_str;
		return os;
	}
}