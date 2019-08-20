#include "Searcher.h"

string Manager::getPathToCalculate()
{
	mt.lock();
	string temp = pathesToCalcul[0];
	pathesToCalcul.erase(pathesToCalcul.begin());
	mt.unlock();
	return temp;
}

void Manager::setPathForCheck(string path)
{
	mt.lock();
	pathesToCalcul.push_back(path);
	mt.unlock();
}

void Manager::find_file( Target & target, Manager & manager, Node & node)
{
	path dir_path;
	while (!(target.getState()))
	{
		dir_path = manager.getPathToCalculate();

		try
		{
			if (!exists(dir_path)) { node.setwasEnded(); return; }
			directory_iterator end_itr; // default construction yields past-the-end
			for (directory_iterator itr(dir_path);
				itr != end_itr;
				++itr)
			{
				cout << itr->path().string() << endl;

				if (is_directory(itr->status()))
				{
					manager.setPathForCheck(itr->path().string());
				}
				else if (itr->path().filename() == target.getWanted()) // see below
				{
					target.setFindedFie(itr->path().string());
					target.setwasFound();
					node.setwasEnded();
					return;
				}
			}
			node.setwasEnded();
			return;
		}
		catch (boost::filesystem::filesystem_error &e)
		{
			cout << e.what() << endl;
			node.setwasEnded();
			return;
		}
	}
}

void Manager::stratSearch()
{
	while (!target.getState())
	{
		static bool asd = true;
		if (asd)
		{
			Node node(target, *this);
			asd = false;
		}

		/*if (threads.size() <= 8)
		{
			int count = (threads.size() - 8) > pathesToCalcul.size() ? 8 - threads.size() : pathesToCalcul.size();
			for (int i = 0; i < count; i++)
			{
				mt.lock();
				threads.push_back(new Node(target, *this));

				mt.unlock();
			}
		}
		for (auto it = threads.begin(); it != threads.end(); it++)
		{
			if (!(*it)->getState())
			{
				mt.lock();
				delete *it;
				it = threads.erase(it);
				mt.unlock();
			}

		}*/
	}
	cout << "***************************FILE WAS FOUND***************************\n";
	cout << target.getPathToWanted() << endl;
	cout << "********************************************************************\n";

}
