#include <iostream>
using namespace std;
int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	long n;
	while(true) {
		cout << "Find all prime numbers from 1 to: " << flush;
		cin >> n; cin.ignore();
		if (!cin.fail() && n > 1) break;
		cin.clear();
		cin.ignore((unsigned int)-1, '\n');
		cout << "\t Error - given number must be at least 1.\n\n";
	}
	bool numbers[n] = {false};

	for(long curr = 2; curr <= n; curr++) {
		if (numbers[curr] == false) {
			cout << curr << " ";
			for(long i = curr*curr; i <= n; i += curr)
				numbers[i] = true;
		}
	}
	cout << endl;
}