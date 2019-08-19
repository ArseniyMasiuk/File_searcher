#include "Searcher.h"

string Manager::getPathToCalculate()
{
	mt.lock();
	string temp = *pathesToCalcul.rend();
	pathesToCalcul.pop_back();
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
				//cout << itr->path().string() << endl;

				if (is_directory(itr->status()))
				{
					manager.setPathForCheck(itr->path().string());
				}
				else if (itr->path().filename() == target.getWanted()) // see below
				{
					target.setFindedFie(itr->path().string());
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
	//while (target.getState())
	//{
	//	if (threads.size() <= 8)
	//	{
	//		int count = (threads.size() - 8) > pathesToCalcul.size() ? 8 - threads.size() : pathesToCalcul.size();
	//		for (int i = 0; i < count; i++)
	//		{
	//			//threads.push_back(Node(target,*this));
	//		}
	//	}
	//	for (auto it = threads.begin(); it!=threads.end(); it++)
	//	{
	//		//if(!it->wasEnded)
	//	}
	//}
	//cout << "***************************FILE WAS FOUND***************************\n";
	//cout << target.getWanted() << endl;
	//cout << "********************************************************************\n";
}
