#pragma once
#include <thread>
#include <iostream>
#include <vector>
#include <deque>
#include<mutex>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#define BOOST_FILESYSTEM_NO_DEPRECATED
#define THREADS_COUNT 100
using namespace boost::filesystem;
using namespace std;


//void find_file(Target & target, Manager & manager, Node & node);

class Manager
{
	
	class Target;
	class Node;
	
	
	friend class Tarset;
	friend class Node;
	//friend void find_file(Target & target, Manager & manager, Node & node);
	static void find_file(Target & target, Manager & manager, Node & node);

	class Node
	{
		thread th;
		bool wasEnded;
		static mutex mt;
	public:
	
	
		void setwasEnded()
		{
			mt.lock();
			wasEnded = true;
			mt.unlock();
		}
		bool getState()
		{
			mt.lock();
			bool temp = wasEnded;
			mt.unlock(); return temp;
		}

		Node(Target &target, Manager &manager)
		{
			wasEnded = false;
			th = thread([&]() {find_file(target, manager, *this); });
			th.detach();
		}
	};
	class Target
	{
		vector<string> pathToFindFile;
		bool isFile = true;
		string wanted;
		bool wasFound = false;
		mutex mt;
	public:
		void setwasFound() { mt.lock(); wasFound = true; mt.unlock(); }
		bool getState() { mt.lock(); bool temp = wasFound; mt.unlock(); return temp; }

		string getWanted() { mt.lock();  string temp = wanted; mt.unlock(); return temp; }
		void setFindedFie(string path) { mt.lock(); /*cout << path << endl;*/ pathToFindFile.push_back(path); mt.unlock(); }
		vector<string>& getPathToWanted() { mt.lock(); vector<string> &temp = pathToFindFile; mt.unlock(); return temp; }
		void setTarget(string targ, bool isfile) { wanted = targ; isFile = isfile; }
		bool getWhatSearch() { mt.lock(); bool temp = isFile; mt.unlock(); return temp; }
		void outPathes()
		{
			mt.lock();
			for (auto it = pathToFindFile.begin(); it != pathToFindFile.end(); it++)
			{
				cout << *it << endl;
			}
			mt.unlock();
		}

	}target;

	vector<Node*> threads;               //vector of threads what do search
	deque<string> pathesToCalcul;      // vector of folders to check
	mutex mt1;                // to synchonize threads

	string getPathToCalculate();
	void setPathForCheck(string path);
public:

	Manager(string wanted, string startFolder,bool isFile = true/*default search file*/)
	{
		pathesToCalcul.push_back(startFolder);
		target.setTarget(wanted,isFile);
	}
	void stratSearch();

	~Manager()
	{
		for (auto it = threads.begin();
			it != threads.end();
			)
		{
			//Node::mt.lock();
			if ((*it)->getState())
			{
				delete *it;
				it = threads.erase(it);
			}
			else it++;
			//Node::mt.unlock();

		}

	}
};

bool compare(string type, string filname, bool compareNames = true/*false if need to compare type*/);

