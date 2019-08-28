#include "Searcher.h"
//#define USE_LOOP_IN_FUNCTION
mutex Manager::Node::mt;
bool compare(string base, string whatSearch, bool compareNames/*false if need to compare type*/)
{
	if (compareNames)//compareNames
	{
		return base == whatSearch;
	}
	else //comparetypes
	{
		rotate(base.begin(), base.begin() + 1, base.end());
		rotate(whatSearch.begin(), whatSearch.begin() + 1, whatSearch.end());
		for (int i = 0; i < whatSearch.size(); i++)
		{
			if (base[i] != whatSearch[i]) return false;
		}
		return true;
		
	}
}


string Manager::getPathToCalculate()
{
	mt1.lock();
	string temp = pathesToCalcul.size() ? pathesToCalcul[0] : "";
	if (temp != "") 
	{
		//cout << pathesToCalcul[0] << endl;
		pathesToCalcul.pop_front();
	}
	else
	{
		//cout << "here is no pathes in vector\n";
	}
	mt1.unlock();
	return temp;
}

void Manager::setPathForCheck(string path)
{
	mt1.lock();
	pathesToCalcul.push_back(path);
	mt1.unlock();
}

void Manager::find_file( Target & target, Manager & manager, Node & node)
{
	path dir_path;
	bool whatSearch = target.getWhatSearch();
//	while (!(target.getState()))
	{
		dir_path = manager.getPathToCalculate();
		if (dir_path.string() != "")
		{
			try
			{
				if (!exists(dir_path)) { node.setwasEnded(); return; }
				directory_iterator end_itr; // default construction yields past-the-end
				for (directory_iterator itr(dir_path);
					itr != end_itr;
					++itr)
				{
					//cout << itr->path().string() << endl;

					if (is_directory(itr->status()))
					{
						manager.setPathForCheck(itr->path().string());
					}
					else if (compare(itr->path().filename().string(), target.getWanted(), whatSearch)) // see below
					{
						target.setFindedFie(itr->path().string());
						if (whatSearch)
						{

							target.setwasFound();
							node.setwasEnded();
							//cout << itr->path().string() << endl;
							return;
						}
					}
				}
				node.setwasEnded();
				return;
			}
			catch (boost::filesystem::filesystem_error &e)
			{
				//cout << e.what() << endl;
				cout << "exception in find_file function\n";
				node.setwasEnded();
				return;
			}
		}
	}
	node.setwasEnded();
}

void Manager::stratSearch()
{
	do
	{
		if(!target.getState())
		if (threads.size() < THREADS_COUNT)
		{
			int count =(threads.size() - THREADS_COUNT) > pathesToCalcul.size() ? THREADS_COUNT - threads.size() : pathesToCalcul.size();
			if (count > pathesToCalcul.size()) count = pathesToCalcul.size();
			for (int i = 0; i < count; i++)
			{
				mt1.lock();
				threads.push_back(new Node(target, *this));

				mt1.unlock();
			}
			cout << "******************************PUSH_TEW_THREADS******************************   " << count << endl;
		}
		try
		{
			int count = 0;
			for (auto it = threads.begin(); 
				      it != threads.end(); )
			{
				//Node::mt.lock();
				if ((*it)->getState())
				{
					delete *it;
					it = threads.erase(it);
					count++;
				}
				else it++;
				//Node::mt.unlock();

			}
			cout << "******************************DELETE_THREADS******************************   " << count << endl;

		}
		catch (exception &e)
		{
			cout << e.what() << endl;
		}
	} while (!target.getState());

	mt1.lock();
	cout << "***************************FILE WAS FOUND***************************\n";
	vector<string>& targets = target.getPathToWanted();
	for (auto it = targets.begin(); it != targets.end(); it++)
	{

		cout << *it << endl;
	}
	cout << "********************************************************************\n";
	mt1.unlock();

}
//E:\QtProjects\HELPER\build-HELPER-Desktop_Qt_5_4_2_MinGW_32bit-Debug\release
