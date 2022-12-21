#include <iostream>
#include <memory.h>
#include <cstdlib>
#include <stdio.h>

using namespace std;

// Attempts to create the string data type to understand it better
// Not finished and full of bugs

class Xstring{
	private:
		size_t len, cap;
		void *str;
		size_t roundPowTwo(size_t num) {
			num--;
			for (size_t i = 1; i < sizeof(size_t)*8; i *= 2)
				num |= num >> i;
			return ++num;
		}
	public:
		// Constructors
			// Default
		Xstring() {
			cap = 1; len = 0;
			if (!(str = malloc(1))) {
				printf("Error - class Xstring error - instruction 'Xstring()' - ran out of memory\n");
				exit(-1);
			}
		}
			// Reserve
		Xstring(size_t num) {
			cap = roundPowTwo(num); len = 0;
			if (!(str = malloc(cap))) {
				printf("Error - class Xstring error - instruction 'Xstring()' - ran out of memory\n");
				exit(-1);
			}
		}
			// Copy
		Xstring(const Xstring &Xstr) {
			len = Xstr.len;
			cap = Xstr.cap;

			if (str = malloc(cap))
				memcpy(str, Xstr.str, Xstr.len);
			else {
				printf("Error - class Xstring error - instruction 'Xstring(Xstring)' - memory assignment error\n");
				exit(-1);
			}
		}
			// Substring
		Xstring(const Xstring &Xstr, size_t s, size_t e = -1) {
			if (s >= Xstr.len) {
				len = 0;
				cap = 1;
				if (!(str = malloc(1))) {
					printf("Error - class Xstring error - instruction 'Xstring(Xstring, %ld, %ld)' - ran out of memory\n", s, e);
					exit(-1);
				}
			} else {
				len = e < Xstr.len ? e : Xstr.len;
				cap = roundPowTwo(len);

				if (str = malloc(len))
					memcpy(str, (char*)Xstr + s, len);
				else {
					printf("Error - class Xstring error - instruction 'Xstring(Xstring, %ld, %ld)' - ran out of memory\n", s, e);
					exit(-1);
				}
				memcpy(str, (char*)Xstr.str + s, len);
			}
		}
			// From C-String
		Xstring(const char* s) {
			len = 0;
			while(*(s + len)) len++;

			cap = roundPowTwo(len);
			if (!(str = malloc(cap))) {
				printf("Error - class Xstring error - instruction 'Xstring(const char*)' - ran out of memory\n");
				exit(-1);
			}

			memcpy(str, s, len);
		}
			// From Sequence
		Xstring(const char* s, size_t n) {
			len = n;
			while(*(s + len)) len++;
			len -= n;

			cap = roundPowTwo(len);
			if (!(str = malloc(cap))) {
				printf("Error - class Xstring error - instruction 'Xstring(const char*)' - ran out of memory\n");
				exit(-1);
			}

			memcpy(str, s + n, len);
		}
			// Fill
		Xstring(size_t num, char c) {
			len = num;
			cap = roundPowTwo(num);

			if (str = malloc(cap))
				memset(str, c, num);
			else {
				printf("Error - class Xstring error - instruction 'Xstring(%ld, %c)' - memory assignment error\n", num, c);
				exit(-1);
			}
		}
			// Range

		~Xstring() { free(str); }
		Xstring operator=(const Xstring &Xstr) {
			len = Xstr.len;
			reserve(len);
			memcpy(str, Xstr.str, Xstr.len);

			return Xstring(*this);
		}
		Xstring operator=(const char *s) {
			while(*(s + len)) len++;
			reserve(len);
			memcpy(str, s, len);

			return Xstring(*this);
		}
		Xstring operator=(const char c) {
			len = cap = 1;
			((char*)str)[0] = c;

			return Xstring(*this);
		}
		void print() const {
			printf("%.*s\n", len, (char*)str);
		}


		// Iterators
		

		// Capacity
		size_t size() const {
			return len;
		}
		size_t length() const {
			return len;
		}
		constexpr size_t max_size() const {
			return (size_t)-1;
		}
		void resize(size_t num = 0, char c = '\0') {
			reserve(num);
			if (num > len)
				memset((char *)str + len, c, num - len);

			len = num;
		}
		size_t capacity() const {
			return cap;
		}
		void reserve(size_t num = 0) {
			if (num > cap) {
				cap = roundPowTwo(num);

				if (!(str = realloc(str, cap))) {
					printf("Error - class Xstring error - instruction 'Xstring.reserve(%ld)' - ran out of memory\n", num);
					exit(-1);
				}
			} else {
				num /= 2;
				if (len <= num && num < cap) {
					cap = roundPowTwo(num);

					str = realloc(str, cap);
				}
			}
		}
		void clear() {
			len = 0;
		}
		bool empty() const {
			return (bool)len;
		}
		void shrink_to_fit() {
			cap = roundPowTwo(len/2);
			str = realloc(str, cap);
		}

		// Element Access
		char operator[](size_t idx) const {
			return ((char*)str)[idx];
		}
		char at(size_t idx) const {
			if (idx >= len) {
				if (idx == len)
					return 0;
				printf("Error - class Xstring error - instruction 'at[%ld]' - invaild array index\n", idx);
				exit(-1);
			}

			return ((char*)str)[idx];
		}
		char back() const {
			if (len)
				return ((char*)str)[len-1];

			printf("Error - class Xstring error - instruction 'back()' - element access in empty Xstring\n");
			exit(-1);
		}
		char front() const {
			if (len)
				return ((char*)str)[0];

			printf("Error - class Xstring error - instruction 'front()' - element access in empty Xstring\n");
			exit(-1);
		}

		// Modifiers
		//		String
		Xstring operator+=(const Xstring &Xstr) {
			reserve(len + Xstr.len);
			memcpy((char*)str + len, Xstr.str, Xstr.len);
			len += Xstr.len;

			return Xstring(*this);
		}
		//		C-String
		Xstring operator+=(const char *s) {
			size_t offset = len;
			while(*(s + len)) len++;

			if (len > cap) {
				cap = roundPowTwo(len);
				if (!(str = realloc(cap))) {
					printf("Error - class Xstring error - instruction 'Xstring(const char*)' - ran out of memory\n");
					exit(-1);
				}
			}

			memcpy((char *)str + offset, s, len - offset);

			return Xstring(*this);
		}
		//		Character
		Xstring operator+=(const char c) {
			if (cap == len) {
				cap *= 2;
				if (!(str = realloc(str, cap))) {
					printf("Error - class Xstring error - instruction 'Xstring += %c' - memory assignment error\n", c);
					exit(-1);
				}
			}
			((char*)str)[len++] = c;

			return Xstring(*this);
		}
		//		String
		Xstring &append(const Xstring &Xstr) {
			reserve(len + Xstr.len);
			memcpy((char*)str + len, Xstr.str, Xstr.len);
			len += Xstr.len;
			return *this;
		}
		//		Substring
		Xstring &append(const Xstring &Xstr, size_t s, size_t e = -1) {
			if (Xstr.len > s) {
				size_t subLen = Xstr.len - s;
				if (e < subLen)
					subLen = e;

				reserve(len + subLen);
				memcpy((char*)str + len, (char*)Xstr.str + s, subLen);
				len += subLen;
			}

			return *this;
		}
		//		C-String
		Xstring &append(const char* s) {
			int olen = 0;
			while(*(s + clen)) clen++;
			reserve(len + clen);
			memcpy((char*)str + len, s, clen);
			len += clen;
			return *this;
		}
		//		Buffer
		Xstring &append(const char* s, size_t n) {
			reserve(len + n);
			memcpy((char*)str + len, s, n);
			len += n;
			return *this;
		}
		//		Fill
		Xstring &append(size_t num, char c) {
			reserve(len + num);
			memset((char*)str + len, c, num);
			len += n;
			return *this;
		}
		//		Range

		void push_back(char c) {
			if (cap == len) {
				cap *= 2;
				if (!(str = realloc(str, cap))) {
					printf("Error - class Xstring error - instruction 'Xstring.push_back(%c)' - memory assignment error\n", c);
					exit(-1);
				}
			}
			((char*)str)[len++] = c;
		}

		//		String
		Xstring &assign(const Xstring& Xstr) {
			len = Xstr.len;
			reserve(len);

			memcpy(str, Xstr.str, len);
			return *this;
		}
		//		Substring
		Xstring &assign(const Xstring &Xstr, size_t s, size_t e = -1) {
			if (s >= Xstr.len) {
				len = 0;
				reserve(1);
			} else {
				len = Xstr.len < e ? Xstr.len : e;
				reserve(len);
				memcpy(str, (char*)Xstr.str + s, len);
			}
			return *this;
		}
		//		C-String
		Xstring &assign(const char* s) {
			len = 0;
			while(*(s + len)) len++;

			reserve(len);

			memcpy(str, s, len);
			return *this;
		}
		//		Buffer
		Xstring &assign(const char* s, size_t num) {
			len = num;
			reserve(len);

			memcpy(str, s, len);
			return *this;
		}
		//		Fill
		Xstring &assign(size_t num, char c) {
			len = num;
			reserve(len);

			memset(str, c, len);
			return *this;
		}
		//		Range

		//		Initializer List

		//		Move

		//		String
		Xstring &insert(size_t pos, const Xstring &Xstr) {
			reserve(len + Xstr.len);
			if (pos >= len)
				memcpy((char*)str + len, Xstr.str, Xstr.len);
			else {
				memcpy((char*)str + Xstr.len + pos, (char*)str + pos, len - pos);
				memcpy((char*)str + pos, Xstr.str, Xstr.len);
			}
			len += Xstr.len;

			return *this;
		}
		//		Substring
		Xstring &insert(size_t pos, const Xstring& Xstr, size_t s, size_t e = -1) {
			if (Xstr.len > s) {
				size_t subLen = Xstr.len - s;
				if (e < subLen)
					subLen = e;

				if (pos >= len)
					memcpy((char*)str + len, (char*)Xstr.str + s, subLen);
				else {
					memcpy((char*)str + Xstr.len + pos, (char*)str + pos, len - pos);
					memcpy((char*)str + pos, (char*)Xstr.str + s, subLen);
				}
				len += subLen;
			}

			return *this;
		}
		//		C-String
		Xstring &insert(size_t pos, const char* s) {
			int olen = 0;
			while(*(s + clen)) clen++;
			reserve(len + clen);
			if (pos >= len)
				memcpy((char*)str + len, Xstr.str, olen);
			else {
				memcpy((char*)str + olen + pos, (char*)str + pos, len - pos);
				memcpy((char*)str + pos, Xstr.str, olen);
			}
			len += clen;

			return *this;
		}
		//		Buffer
		Xstring &insert(size_t pos, const char* s, size_t num) {
			reserve(len + num);
			if (pos >= len)
				memcpy((char*)str + len, Xstr.str, num);
			else {
				memcpy((char*)str + num + pos, (char*)str + pos, len - pos);
				memcpy((char*)str + pos, Xstr.str, num);
			}
			len += num;

			return *this;
		}
		//		Fill
		Xstring &insert(size_t pos, size_t num, char c) {
			reserve(len + num);
			if (pos >= len)
				memset((char*)str + len, c, num);
			else {
				memcpy((char*)str + Xstr.len + pos, (char*)str + pos, len - pos);
				memset((char*)str + pos, c, num);
			}
			len += num;

			return *this;
		}
		//		Fill Iterator 

		//		Range

		//		Initializer List

		//		Sequence	
		Xstring &erase(size_t pos = 0, size_t ePos = -1) {
			if (pos < len) {
				size_t delLen = len - pos;
				if (ePos < delLen)
					delLen = ePos;

				memcpy((char*)str + pos, (char*)str + pos + delLen, len - pos - delLen);
				len -= delLen;
			}
			return *this;
		}
		//		Character	

		//		Range

		//		String
		Xstring &replace(size_t pos, size_t repLen, const Xstring &Xstr) {
			
		}
		//		String Iterator
		Xstring &replace(const_iterator i1, const_iterator i2, const Xstring &Xstr) {

		}
		//		Substring
		Xstring &replace(size_t pos, size_t repLen, const Xstring &Xstr, size_t subpos, size_t sublen = npos) {

		}
		//		C-String
		Xstring &replace(size_t pos, size_t repLen, const char* s) {

		}
		//		C-String Iterator

		//		Buffer
		Xstring &replace(size_t pos, size_t repLen, const char* s, size_t n) {

		}
		//		Buffer Iterator

		//		Fill
		Xstring &replace(size_t pos, size_t repLen, size_t n, char c) {

		}
		//		Fill Iterator

		//		Range

		//		Initializer List

		void swap(Xstring &Xstr) {
			swap(len, Xstr.len);
			swap(cap, Xstr.cap);
			swap(str, Xstr.str);
		}
		void pop_back() {
			if (len)
				len -= 1;
			else {
				printf("Error - class Xstring error - instruction 'Xstring.pop_back()' - undefined behavior\n");
				exit(-1);
			}
		}

		// String Operations
		char *c_str() const {
			return (char *)str+'\0';
		}
		char *data() const {
			return (char *)str;
		}
		// allocator_type get_allocator() const {
			
		// }
		size_t copy(char* c, size_t length, size_t pos = 0) const {
			size_t newLen = length + pos;
			if (newLen < length || newLen >= len)
				newLen = len - pos;

			memcpy(c, (char *)str + pos, newLen);
			return newLen;
		}
		//		String
		size_t find(const Xstring &str, size_t pos = 0) const {
			
		}
		//		C-String
		size_t find(const char* s, size_t pos = 0) const {
			
		}
		//		Buffer
		size_t find(const char* s, size_t pos, size_t n) const {
			
		}
		//		Character
		size_t find(char c, size_t pos = 0) const {
			
		}












		Xstring substr(size_t s, size_t e = -1) const {
			size_t newLen = len < e ? len : e;
			if (s >= len)
				return Xstring();

			Xstring newStr(newLen);
			newStr.len = newLen;
			memcpy(newStr, (char*)str + s, newLen);

			return newStr;
		}
		//		String
		int compare(const Xstring &Xstr) const {
			int i = 0, j = 0;
			char *s1 = (char*)str, *s2 = (char*)Xstr.str;
			for(;i<len && j<Xstr.len; i++, j++) {
				if (s1[i] != s2[i]) {
					if (s1[i] < s2[i])
						return -1;
					return 1;
				}
			}
			if (i != len)
				return -1;
			if (j != Xstr.len)
				return 1;
			return 0;
		}
		//		Substrings
		int compare(size_t pos, size_t len, const Xstring &Xstr) const {
			
		}
		int compare(size_t pos, size_t len, const Xstring &Xstr, size_t subpos, size_t sublen) const {
			
		}
		//		C-String	
		int compare(const char* s) const {
			
		}
		int compare(size_t pos, size_t len, const char* s) const {

		}
		//		Buffer	
		int compare(size_t pos, size_t len, const char* s, size_t n) const {
			
		}

		// Function Overloads
		Xstring operator+(const Xstring &Xstr) const {
			Xstring newStr(len + Xstr.len);

			memcpy((char*)newStr.str, str, len);
			memcpy((char*)newStr.str + len, Xstr.str, Xstr.len);

			return newStr;
		}

		// Explicit Type Conversion
		operator char*() const {
			return (char*)str;
		}

};

int main() {
	char a[11] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 0} ;
	Xstring str(7, 'r'), str2, str3(a, 7);
	str.print();
	str3.print();

	str.set(10, '7');
	str.print();

	str2.set(10, '6');
	str2.print();

	str += str2;
	str.print();

	str2.append(str);
	str2.print();

	str.push_back('c');
	str.print();

	str2 = str.substr(5);
	str2.print();

	str2 = str.substr(9, 4);
	str2.print();
	
	str2.resize(6, 'r');
	str2.print();

	str2.resize(2, 'c');
	str2.print();
}