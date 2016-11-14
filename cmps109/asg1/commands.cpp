// $Id: commands.cpp,v 1.11 2014-06-11 13:49:31-07 - - $

#include "commands.h"
#include "debug.h"

commands::commands(): map ({
	{"cat"   , fn_cat   },
	{"cd"    , fn_cd    },
	{"echo"  , fn_echo  },
	{"exit"  , fn_exit  },
	{"ls"    , fn_ls    },
	{"lsr"   , fn_lsr   },
	{"make"  , fn_make  },
	{"mkdir" , fn_mkdir },
	{"prompt", fn_prompt},
	{"pwd"   , fn_pwd   },
	{"rm"    , fn_rm    },
}){}

command_fn commands::at (const string& cmd) {
	// Note: value_type is pair<const key_type, mapped_type>
	// So: iterator->first is key_type (string)
	// So: iterator->second is mapped_type (command_fn)
	command_map::const_iterator result = map.find (cmd);
	if (result == map.end()) {
		throw yshell_exn (cmd + ": no such function");
	}
	return result->second;
}


void fn_cat (inode_state& state, const wordvec& words){
	inode_ptr current = state.get_cwd();
	map<string, inode_ptr> directory = current->get_dir()->get_dirents();
	map<string, inode_ptr>::const_iterator it = directory.begin();
	while (it != directory.end())
	{
		if (it->first == words.at(1))
		{
			for (unsigned int i = 0; i < it->second->get_file()->readfile().size(); i++)
			{
				cout << it->second->get_file()->readfile().at(i);
				if (i + 1 != it->second->get_file()->readfile().size())
					cout << " ";
			}
			cout << endl;
			break;
		}
		it++;
	}
}

void fn_cd (inode_state& state, const wordvec& words){
	if (words.size() < 2)
		cerr << "Invalid command use" << endl;
	else
		state.set_cwd(words.at(1));
}

void fn_echo (inode_state& state, const wordvec& words){
	DEBUGF('c', state);
	for (unsigned int i = 1; i < words.size(); i++)
		cout << words.at(i) << " ";
	cout << endl;
}

void fn_exit (inode_state& state, const wordvec& words){
	exit_status exitState;
	int exitnum = stoi(words.at(1));
	exitState.set(exitnum);
	cout << "Exiting with status: " << exit_status_message();
	throw ysh_exit_exn();
}

void fn_ls (inode_state& state, const wordvec& words)
{
	string space = "     ";
	inode_ptr current = state.get_cwd();
	map<string, inode_ptr> directory = current->get_dir()->get_dirents();
	map<string, inode_ptr>::const_iterator it = directory.begin();
	cout << "/";
	if (words.size() > 1)
		cout << words.at(1);
	else if (current->get_name() != "." || current->get_name() != "..")
		cout << current->get_name();
	cout << ":" << endl;
	for (; it != directory.end(); it++)
	{
		cout << space << it->second->get_inode_nr() << space;
		if (it->second->get_type() == PLAIN_INODE)
			cout << it->second->get_file()->size() << " " << it->first << endl;
		else if (it->first == "." || it->first == "..")
			cout << it->second->get_dir()->size() << " " << it->first << endl;
		else
			cout << it->second->get_dir()->size() << " " << it->first + "/" << endl;
	}
}

void fn_lsr (inode_state& state, const wordvec& words){
	cout << "/";
	if (words.size() > 1)
		cout << words.at(1);
	cout << ":" << endl;
	cout << state << endl;
}

void fn_make (inode_state& state, const wordvec& words){
	if (words.size() < 2)
		cerr << "Invalid command use" << endl;
	else
		state.make_file_one(words);
}

void fn_mkdir (inode_state& state, const wordvec& words){
	if (words.size() < 2)
		cerr << "Invalid command use" << endl;
	else
	{
		inode_ptr new_dir = make_shared<inode>(DIR_INODE);
		state.make_dir_one(words.at(1), new_dir);
	}
}

void fn_prompt (inode_state& state, const wordvec& words){
	if (words.size() > 1)
		state.set_prompt(words);
	else
		cerr << "Invalid command use" << endl;
}

void fn_pwd (inode_state& state, const wordvec& words){
	if (words.size() > 1)
		cerr << "Invalid command use" << endl;
	else
		cout << "/" << state.get_cwd()->get_name() << endl;
}

void fn_rm (inode_state& state, const wordvec& words){
	if (words.size() < 1 || words.at(1) == "." || words.at(1) == "..")
	{
		cerr << "Invalid command use" << endl;
		return;
	}
	else
	{
		inode_ptr current = state.get_cwd();
		map<string, inode_ptr> directory = current->get_dir()->get_dirents();
		map<string, inode_ptr>::const_iterator it = directory.begin();
		for (; it != directory.end(); ++it)
		{
			if (it->first == words.at(1))
			{
				state.remove_one(words.at(1));
				return;
			}
		}
	}
	cout << "File not found" << endl;
}

void fn_rmr (inode_state& state, const wordvec& words){
	DEBUGF ('c', state);
	DEBUGF ('c', words);
}

int exit_status_message() {
	int exit_status = exit_status::get();
	cout << execname() << ": exit(" << exit_status << ")" << endl;
	return exit_status;
}

