#pragma once
#include <iostream>
#include <stdio.h>
#include "Text.h"

using namespace std;

class Command
{
public:
	bool state = false;
	Command(int com, string text = "", int index = 0, int len = 0) : command(com), state(true) {
		switch (command)
		{
		case -1: // delete: info(text)
			info = text;
			break;
		case 1 || 4:
			length = text.length();
			break;
		case 2:
			break;
		case 3:
			info = text; // file name
			break;
		case 6:
			position = index;
			length = len;
			break;
		case 8:
			info = text; // deleted text
			position = index;
			break;
		default:
			state = false;
			break;
		}
	}

	void Undo(Text* text) { // return everything to the way it was
		state = false; // now it's falsified command)

		string previousText = info;
		string line = "";
		const char* filename = info.c_str();
		ifstream inputFile(filename);
		int len = text->text.length();
		switch (command)
		{
		case -1: // deleted: info(text) => textBack
			text->AddSomeText(info);
			info = "";
			break;
		case 1 || 4: // added: len => textBack: info(addedT)
			info = text->text.substr(text->text.length() - length); // text to add(1)
			text->DeleteText(text->text.length() - length, length);
			break;
		case 2:
			text->DeleteText(text->text.length() - 1, 1);
			break;
		case 3: // saved, info(file) => delete file, info(text)
			if (!inputFile) {
				cerr << "Error opening file '" << filename << "'.\n";
				break;
			}
			info = "";
			while (getline(inputFile, line)) {
				info += line; // text from file
			}
			inputFile.close();
			if (remove(filename) == 0) { cout << "File '" << filename << "' deleted successfully.\n"; }
			else { cerr << "Error deleting file '" << filename << "'.\n"; }
			break;
		case 6: // inserted: pos, len => textBack: info(text), pos
			info = text->text.substr(position, length); // text to insert(6)
			text->DeleteText(position, length);
			break;
		case 8: // deleted: info(delText), pos => insert: pos, len
			text->InsertText(position, info);
			length = info.length();
			info = "";
			break;
		default:
			state = true;
			break;
		}
	}

	void Redo(Text* text) {
		state = true;
		switch (command)
		{
		case 1 || 4: // added: position -> deleted: info(addedT) => add: position
			text->AddSomeText(info);
			info = "";
			break;
		case 2:
			text->AddNewLine();
			break;
		case 3: // saved -> deleted, info(file) => save
			text->SaveToFile(info);
			info = "";
			break;
		case 6: // inserted, i, len -> deleted, pos, info(text), len => insert, i, len
			text->InsertText(position, info);
			info = "";
			break;
		case 8: // deleted, info(text), pos -> inserted, pos, len => delete, info(text), pos
			info = text->text.substr(position, length);
			text->DeleteText(position, length);
			break;
		default:
			state = false;
			break;
		}
	}

private:
	int command = 0; // for differentiation btw simple and complex commands
	string info = ""; // for save/load/delete/clear
	int position = 0; // for insert/delete
	int length = 0; // for insert/add
};

