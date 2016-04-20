// $Id: inode.cpp,v 1.12 2014-07-03 13:29:57-07 - - $

#include <iostream>
#include <stdexcept>

using namespace std;

#include "debug.h"
#include "inode.h"

int inode::next_inode_nr {1};

/**********************************************************************
 *	INODE_STATE *******************************************************
 **********************************************************************/

inode_state::inode_state()
{
	root = make_shared<inode>(DIR_INODE);
	cwd = root;
	cwd->set_parent(root);
	cwd->make_dir_two(".", cwd);
	cwd->make_dir_two("..", cwd);
}

// GETTERS

inode_ptr inode_state::get_root()
{
	return root;
}

inode_ptr inode_state::get_cwd()
{
	return cwd;
}

string inode_state::get_prompt()
{
	return prompt;
}


// SETTERS

void inode_state::set_cwd(const string& dir_name)
{
	cwd->set_cwd_two(dir_name, cwd);
}

void inode_state::set_prompt(const wordvec& words)
{
	string temp = "";
	for (unsigned int i = 1; i < words.size(); i++)
	{
		temp.append(words.at(i));
		if (i + 1 != words.size())
			temp.append(" ");
	}
	prompt = temp;
}

void inode_state::make_dir_one(const string& filename, inode_ptr& child)
{
	child->set_name(filename);
	cwd->make_dir_two(filename, child);
	child->make_dir_two(".", child);
	child->make_dir_two("..", cwd);
	child->set_parent(cwd);
}

void inode_state::make_file_one(const wordvec& words)
{;
	wordvec temp;
	temp.resize(words.size() - 2);
	if (words.size() > 2)
	{
		for (unsigned int i = 0; i + 2 < words.size(); i++)
			temp.at(i) = words.at(i + 2);
	}
	else if (words.size() == 2)
	{
		temp.resize(1);
		temp.at(0) = "";
	}
	else
	{
		cout << "Invalid Argument" << endl;
		return;
	}
	cwd->make_file_cont(words.at(1), temp);
}

void inode::make_file_cont(const string& filename, const wordvec& words)
{
	dir->mkfile(filename, words);
}

// OVERRIDES

ostream& operator<< (ostream& out, const inode_state& state)
{
	out << "inode_state: root = " << state.root
		<< ", cwd = " << state.cwd;
	return out;
}

void inode_state::remove_one(const string& filename)
{
	cwd->remove_two(filename);
}

/****************************************************************
 *	INODE *******************************************************
 ****************************************************************/

inode::inode(inode_t init_type) :
inode_nr(next_inode_nr++), type(init_type)
{
	switch (type) {
	case PLAIN_INODE:
		file = make_shared<plain_file>();
		break;
	case DIR_INODE:
		dir = make_shared<directory>();
		break;
	}
	type = init_type;
}

// GETTERS

int inode::get_inode_nr() const
{
	return inode_nr;
}

string inode::get_name()
{
	return name;
}

inode_t inode::get_type()
{
	return type;
}

plain_file_ptr inode::get_file()
{
	return file;
}

directory_ptr inode::get_dir()
{
	return dir;
}

inode_ptr inode::get_parent()
{
	return parent;
}

// SETTERS

void inode::set_name(const string& new_name)
{
	name = new_name;
}

void inode::set_cwd_two(const string& dir_name, inode_ptr& _cwd)
{
	dir->set_cwd_three(dir_name, _cwd);
}

void inode::set_inode_nr()
{
	inode_nr = 1;
}

void inode::set_parent(inode_ptr& new_parent)
{
	parent = new_parent;
}

void inode::make_file_two(const wordvec& words)
{
	file->writefile(words);
}

void inode::make_dir_two(const string& filename, inode_ptr& child)
{
	dir->mkdir(filename, child);
}

void inode::remove_two(const string& filename)
{
	dir->remove_three(filename);
}

/********************************************************************
 *	FILE_BASE *******************************************************
 ********************************************************************/

plain_file_ptr plain_file_ptr_of(file_base_ptr ptr)
{
	plain_file_ptr pfptr = dynamic_pointer_cast<plain_file> (ptr);
	if (pfptr == nullptr) throw invalid_argument("plain_file_ptr_of");
	return pfptr;
}

directory_ptr directory_ptr_of(file_base_ptr ptr)
{
	directory_ptr dirptr = dynamic_pointer_cast<directory> (ptr);
	if (dirptr == nullptr) throw invalid_argument("directory_ptr_of");
	return dirptr;
}


/*********************************************************************
 *	PLAIN_FILE *******************************************************
 *********************************************************************/

size_t plain_file::size() const
{
	size_t size = 0;
	for (unsigned int i = 0; i < data.size(); i++)
		size += data.at(i).length();
	return size;
}

const wordvec& plain_file::readfile() const
{
	return data;
}

void plain_file::writefile(const wordvec& words)
{
	data = words;
}


/********************************************************************
 *	DIRECTORY *******************************************************
 ********************************************************************/

size_t directory::size() const
{
	size_t size = dirents.size();
	return size;
}

void directory::remove_three(const string& filename)
{
	dirents.erase(dirents.find(filename));
}

void directory::mkfile(const string& filename, const wordvec& words)
{
	inode_ptr temp = make_shared<inode>(PLAIN_INODE);
	temp->make_file_two(words);
	dirents[filename] = temp;
}

void directory::mkdir(const string& dirname, inode_ptr& child)
{
	dirents[dirname] = child;
}

map<string, inode_ptr> directory::get_dirents()
{
	return dirents;
}

void directory::set_cwd_three(const string& dir_name, inode_ptr& _cwd)
{
	_cwd = dirents.find(dir_name)->second;
}

