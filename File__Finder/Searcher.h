#pragma once
#include <thread>
#include <iostream>
#include <vector>
#include<mutex>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

#define BOOST_FILESYSTEM_NO_DEPRECATED

using namespace boost::filesystem;
using namespace std;

class Manager
{
	class Target;
	class Node
	{
		thread th;
		bool wasEnded;
		mutex mt;
	public:
		void setwasEnded(){ mt.lock(); wasEnded = true; mt.unlock(); }
	
		Node(Target &target, Manager &manager)
		{
			wasEnded = false;
			//th = thread([&]() {Manager::find_file(target, manager, *this); });
			th.detach();
		}
		
	};
	class Target
	{
		string pathToFindFile;
		string wanted;
		bool wasFound = false;
		mutex mt;
	public:
		void setwasFound() { mt.lock(); wasFound = true; mt.unlock(); }
		bool getState() { mt.lock(); bool temp = wasFound; mt.unlock(); return temp; }

		string getWanted() { mt.lock();  string temp = wanted; mt.unlock(); return temp; }
		void setFindedFie(string path) { mt.lock();  pathToFindFile = path; mt.unlock(); }
		void setTarget(string targ) { wanted = targ; }

	}target;

	friend class Tarset;
	friend class Node;
	vector<Node> threads;               //vector of threads what do search
	vector<string> pathesToCalcul;      // vector of folders to check
	mutex mt;                           // to synchonize threads

	string getPathToCalculate();
	void setPathForCheck(string path);
	static void find_file( Target & target, Manager & manager, Node & node);
public:

	Manager(string wanted, string startFolder)
	{
		pathesToCalcul.push_back(startFolder);
		target.setTarget(wanted);
	}
	void stratSearch();
};