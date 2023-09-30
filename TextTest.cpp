#include "pch.h"
#include "CppUnitTest.h"
#include "Text.h"
#include "Actions.h"
#include "Command.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TextTest
{
    TEST_CLASS(TextTest)
    {
    public:

        TEST_METHOD(TestConstructorAndWriteSelf)
        {
            Text text("Initial Text");
            Assert::AreEqual(std::string("Initial Text"), text.text);
        }

        TEST_METHOD(TestAddNewLine)
        {
            Text text("Initial Text");
            text.AddNewLine();
            Assert::AreEqual(std::string("Initial Text\n"), text.text);
        }

        TEST_METHOD(TestAddSomeText)
        {
            Text text("Initial Text");
            text.AddSomeText("\nAdded text)");
            Assert::AreEqual(std::string("Initial Text\nAdded text)"), text.text);
        }

        TEST_METHOD(TestClearText)
        {
            Text text("Initial Text");
            text.ClearText();
            Assert::AreEqual(std::string(""), text.text);
        }

        TEST_METHOD(TestDeleteText)
        {
            Text text("This is some text to delete.");
            text.DeleteText(10, 2);
            Assert::AreEqual(std::string("This is so text to delete."), text.text);
        }

        TEST_METHOD(TestFindIndex) {
            Text text("Line 1\nLine 2\nLine3.");
            int index = text.FindIndex(1, 3);
            Assert::AreEqual(10, index);
        }

        TEST_METHOD(TestInsertText)
        {
            Text text("This is text.");
            text.InsertText(5, "some ");
            Assert::AreEqual(std::string("This some is text."), text.text);
        }

        TEST_METHOD(TestSearchText)
        {
            Text text("This is some text to search in.");
            int index = text.SearchText(std::string("some"));
            Assert::AreEqual(8, index);
        }

        TEST_METHOD(TestSaveToFile)
        {
            std::ofstream testFile("test.txt");
            testFile << "This is some text to save.";
            testFile.close();

            Text text("Text to save.");

            std::string filename = "test.txt";
            text.SaveToFile(filename);

            std::ifstream savedFile(filename);
            std::stringstream buffer;
            buffer << savedFile.rdbuf();
            std::string savedText = buffer.str();

            Assert::AreEqual(std::string("Text to save."), savedText);

            remove(filename.c_str());
        }

        TEST_METHOD(TestLoadFromFile)
        {
            std::ofstream testFile("test.txt");
            testFile << "text to load.";
            testFile.close();

            Text text("Initial ");

            std::string filename = "test.txt";
            text.LoadFromFile(filename);

            Assert::AreEqual(std::string("Initial text to load."), text.text);

            remove(filename.c_str());
        }

        TEST_METHOD(TestCommandsUndoRedo1)
        {
            Text text("Initial Text");

            Actions actions(text.text);
            actions.NextCommand(1, text.AddSomeText("Added text"));

            actions.Undo(&text);
            Assert::AreEqual(std::string("Initial Text"), text.text);

            actions.Redo(&text);
            Assert::AreEqual(std::string("Initial TextAdded text"), text.text);
        }

        TEST_METHOD(TestCommandsUndoRedo2)
        {
            Text text("Initial Text"); // "Initial Text"
            Actions actions(text.text);
            actions.NextCommand(1, text.AddSomeText("Added text")); // "Initial TextAdded text"
            text.AddNewLine();
            actions.NextCommand(2); // "Initial TextAdded text\n"
            actions.NextCommand(3, text.SaveToFile("save.txt")); // add smth
            pair<int, int> posAndLength = text.InsertText(7, "inserted text"); // "Initialinserted text TextAdded text\n"
            actions.NextCommand(6, "", posAndLength.first, posAndLength.second);
            pair<string, int> textAndIndex = text.DeleteText(1, 3); // "Iialinserted text TextAdded text\n"
            actions.NextCommand(8, textAndIndex.first, textAndIndex.second);
            Assert::AreEqual(std::string("Iialinserted text TextAdded text\n"), text.text);
            actions.Undo(&text);
            Assert::AreEqual(std::string("Initialinserted text TextAdded text\n"), text.text);
            actions.Redo(&text);
            Assert::AreEqual(std::string("Iialinserted text TextAdded text\n"), text.text);
            actions.Undo(&text);
            Assert::AreEqual(std::string("Initialinserted text TextAdded text\n"), text.text);
            actions.Undo(&text);
            Assert::AreEqual(std::string("Initial TextAdded text\n"), text.text);
            actions.Undo(&text);
            Assert::AreEqual(std::string("Initial TextAdded text\n"), text.text);
            actions.Undo(&text);
            Assert::AreEqual(std::string("Initial TextAdded text\n"), text.text);

            actions.Redo(&text);
            Assert::AreEqual(std::string("Initial TextAdded text\n"), text.text);
            actions.Redo(&text);
            Assert::AreEqual(std::string("Initialinserted text TextAdded text\n"), text.text);
            actions.Redo(&text);
            Assert::AreEqual(std::string("Iialinserted text TextAdded text\n"), text.text);
            actions.Redo(&text);
            Assert::AreEqual(std::string("Iialinserted text TextAdded text\n"), text.text);
        }
    };
}
