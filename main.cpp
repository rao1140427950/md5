#include <iostream>
#include <iomanip>
#include "md5.h"

using namespace std;

string info = "16711083";


int main()
{
	cout << "Orignial message: " << info << endl;
	cout << "MD5 value: " << getMD5(info) << endl;
}
