

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "FibHeap.h"
#include <hash_map>
#include <ext/hash_map>
using namespace __gnu_cxx;
namespace __gnu_cxx
{
	template<> struct hash<string>
	{
		size_t operator()(const string& s) const
		{
			return __stl_hash_string(s.c_str());
		}
	};
}

using namespace std;






int main(int argc, char *argv[])
{

	FibHeap* FH = new FibHeap();
	ifstream in(argv[1]);
	string filename;
	string line;
	FibNode* tmp;
	hash_map <string, FibNode*> table;
	hash_map <string, FibNode*> ::const_iterator value;
	typedef pair <string, FibNode*> SF_Pair;



	if (in) //
	{
		ofstream outfile("output_file.txt");
		while (getline(in, line))
		{


			string buf;
			stringstream ss(line);
			ss >> buf;// put every line of input command to buf
			if (buf[0] == '$')// if the first character is '$'
			{
				string input1 = buf.erase(0, 1);
				value = table.find(input1);
				if (value == table.end())// the key word is not in the hash table
				{
					ss >> buf;
					int input2 = stoi(buf);
					tmp = FH->insert(input2, input1);//insert the keyword and its frequecy to the fibonacci heap, return a pointer points to the inserted node
					table.insert(SF_Pair(input1, tmp)); //insert the keyword and corresponding node pointer to the hash table
				}
				else// the key word is in the hash table
				{
					ss >> buf;
					int increasenum = stoi(buf);
					FH->increase(value->second, increasenum);// increase the frequency of the corresponding node by 'increasenum'
				}


			}
			else if (buf[0] == 's')// the first character is 's', stop
			{
				return 0;
			}
			else// if the first character  is neither 's' or '$', it should be a integer number
			{
				int count = stoi(buf);
				FH->maximumn(count, outfile); // find the top most n keyword
				outfile << endl;



			}
		}
		outfile.close();
	}
	else //no such file
		cout << "no such file" << endl;



	return 0;
}
