#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Text {
public:
    string text;

    Text(string adding) : text(adding) {};

    pair<string, int> DeleteText(int position = 0, int length = 0) {
        int textLength = text.length();
        if (length == 0) {
            int line;
            int index;
            cout << "\nChoose line, index, and number of symbols to delete: ";
            string input;
            getline(cin, input);

            istringstream iss(input);
            if (!(iss >> line >> index >> length)) {
                cerr << "Failed to parse row of three numbers." << endl;
                make_pair("", 0);
            }

            position = this->FindIndex(line, index);
        }

        int from = position + length;
        from = (from <= textLength) ? from : textLength;
        string deleted = text.substr(position, length);
        text = text.substr(0, position) + text.substr(from);
        return make_pair(deleted, position);
    }

    void WriteSelf() {
        cout << "The text is: \"" << this->text << "\".\n";
    }

    void AddNewLine() {
        text.append("\n");
    }

    string AddSomeText(string textToAdd = "") {
        if (textToAdd == "")
        {
            cout << "Enter text you want to add: ";
            getline(cin, textToAdd);
        }
        text.append(textToAdd);
        cout << "\nNew text was added.\n";
        return textToAdd;
    }

    void ClearText() {
        text = "";
        cout << "\nThe text is cleared.\n";
    }

    int FindIndex(int line, int column) {

        int lines = 0;
        int position = 0;

        for (int i = 0; i < text.length(); i++) {
            if (lines == line - 1 && position < column) {
                position = column;
            }

            if (lines == line) {
                position = i + column;
                break;
            }
            else if (text[i] == '\n') {
                lines++;
            }
        }

        return position;
    }

    pair<int, int> InsertText(int position = 0, string textToInsert = "") {
        if (textToInsert == "")
        {
            int line = 0;
            int column = 0;
            cout << "Enter text you want to add: ";
            getline(cin, textToInsert);
            cout << "Enter line of inserting: ";
            cin >> line;
            cout << "Enter column of inserting: ";
            cin >> column;

            position = this->FindIndex(line, column);
        }

        int text_length = text.length();
        int add_length = textToInsert.length();

        string result(text_length + add_length, ' ');

        for (int i = 0; i < position; i++) {
            result[i] = text[i];
        }
        for (int i = 0; i < add_length; i++) {
            result[position + i] = textToInsert[i];
        }
        for (int i = 0; i < text_length - position; i++) {
            result[position + add_length + i] = text[position + i];
        }

        text = result;
        return make_pair(position, add_length);
    }

    int SearchText(string find="") {
        if (find == "")
        {
            cout << "Enter text to search: ";
            cin >> find;
        }

        int n_length = find.length();
        int indices[100];
        int count = 0;
        for (int i = 0; i <= text.length() - n_length; i++)
        {
            bool get_out = false;
            for (int j = 0; j < n_length; j++)
            {
                if (text[i + j] != find[j])
                {
                    get_out = true;
                    break;
                }
            }
            if (!get_out)
            {
                indices[count] = i;
                count++;
            }
        }
        if (count == 0) cout << "\nSorry, nothing was found.\n";
        else {
            cout << "\nText " << find << " was found at index: ";
            for (int i = 0; i < count - 1; i++) cout << indices[i] << ", ";
            cout << indices[count - 1] << ".\n";
            return indices[0];
        }
        return 0;
    }

    string SaveToFile(string filename = "") {
        if (filename == "")
        {
            cout << "Enter a name of the file you want to use for saving the text: ";
            cin >> filename;
        }

        ofstream outputFile(filename);


        if (!outputFile) {
            cout << "\nError opening file." << endl;
        }
        else {
            outputFile << text;
            outputFile.close();
            cout << "\nText successfully saved to " << filename << endl;
        }
        return filename;
    }

    string LoadFromFile(string filename = "") {
        if (filename == "")
        {
            cout << "\nEnter a name of the file you want to use for loading the text: ";
            cin >> filename;
        }
        //cout << "\nDo you want to clear text at first (y/n)?";
        //char answer = 'n';
        //cin >> answer;
        //if (answer == 'y') text = "";

        ifstream inputFile(filename);

        if (!inputFile) {
            cout << "\nError opening file." << endl;
        }
        else {
            string line;
            while (getline(inputFile, line)) {
                text += line + "\n";
            }
            inputFile.close();

            cout << "Text successfully loaded from " << filename << endl;
        }
        text = text.substr(0, text.length() - 1);
        return text;
    }
};
