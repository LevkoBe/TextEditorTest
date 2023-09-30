#pragma once
#include <iostream>
#include "Command.h"
#include "Text.h"

using namespace std;

class Actions
{
public:
	Actions(string text) : command1(new Command(0)), command2(new Command(0)), command3(new Command(0)) {
		cout << "\nLast three commands will be tracked.\n";
	};

	void NextCommand(int com, string text = "", int index = 0, int len = 0) {
		current++;
		if (current % 3 == 1)
		{
			command1 = new Command(com, text, index, len);
		}
		else if (current % 3 == 2)
		{
			command2 = new Command(com, text, index, len);
		}
		else
		{
			command3 = new Command(com, text, index, len);
		}
	};

	void Undo(Text* text) {
		Command* command = current % 3 == 1 ? command1 : current % 3 == 2 ? command2 : command3;
		if (!command->state)
		{
			cout << "\nSorry, no commands available.\n";
			return;
		}
		current--;
		command->Undo(text);
	}

	void Redo(Text* text) {
		current++;
		Command* command = current % 3 == 1 ? command1 : current % 3 == 2 ? command2 : command3;
		if (command->state)
		{
			cout << "\nSorry, no commands available.\n";
			return;
		}
		command->Redo(text);
	}

private:
	int current = 0;
	Command* command1;
	Command* command2;
	Command* command3;
};

