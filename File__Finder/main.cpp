#include "Searcher.h"

bool find_file(path dir_path, string file_name, path & path_found) // recurcive function which looks for a file
{
	{
		try
		{
			if (!exists(dir_path)) return false;
			directory_iterator end_itr;
			for (directory_iterator itr(dir_path); itr != end_itr; ++itr)
			{
				if (is_directory(itr->status()))
				{
					if (find_file(itr->path(), file_name, path_found)) return true;
				}
				else if (itr->path().filename() == file_name)
				{
					path_found = itr->path();
					cout << "+++++++++++++++++++"<<itr->path() << endl;
					return true;
				}
			}

			return false;
		}
		catch (const filesystem_error& e) {
			cout << e.what() << '\n';
			return false;
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	string myPath = "E:/";
	string myFile = "fucking_name.txt";
	

	/*path p = "E:/";
	path pp;

	thread th([&]() {find_file(p, myFile, pp); });*/
	Manager man(myFile, myPath);
	man.stratSearch();
	//th.join();
	system("pause");
}