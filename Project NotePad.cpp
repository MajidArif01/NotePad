#include<iostream>
#include<conio.h>
#include<vector>
#include<string>
#include<cstring>
#include<fstream>
#include<windows.h>
using namespace std;

struct Node
{
	char data;
	Node* Right, * Left, * Bottom, * up;
	Node(char data)
	{
		this->data = data;
		Right = NULL;
		Left = NULL;
		up = NULL;
		Bottom = NULL;
	}
};
class Stack
{
private:
	Node* top;
public:
	Stack()
	{
		top = NULL;
	}
	void Push_insert(char data)
	{
		if (top == NULL)
		{
			top = new Node(data);
		}
		else
		{
			Node* temp;
			temp = new Node(data);
			temp->Right = top;
			top = temp;
			temp = NULL;
			delete temp;
		}
	}
	bool is_Empty()
	{
		if (top == NULL)
			return true;
		else
			return false;
	}
	char Top_Value()
	{
		if (is_Empty())
			return '-1';
		else
			return top->data;
	}
	void pop_stack()
	{
		if (is_Empty() == true)
			return;
		else
		{
			Node* temp = top;
			top = temp->Right;
			temp->Right = NULL;
			delete temp;
		}
	}
};

class List
{
private:
	Node* head, * tail, * current_Line, * prev_Line;
	int word_Count, line_Count;
	string copy_Text;
protected:
	Stack undo, redo;

public:
	List()
	{
		prev_Line = NULL;
		current_Line = NULL;
		head = NULL;
		tail = NULL;
		word_Count = 0;
		line_Count = 0;
		copy_Text = "";
	}
	void insert_Node(char data)
	{

		//inserting head
		if (head == NULL)
		{

			head = new Node(data);
			tail = head;
			prev_Line = head;
			word_Count++;
			undo.Push_insert(data);
			if (data == '\n')
				word_Count--;
		}
		else
		{
			Node* temp;
			temp = new Node(data);
			undo.Push_insert(data);
			tail->Right = temp;
			temp->Left = tail;
			tail = temp;
			word_Count++;
			if (tail->data == '\n')
			{
				line_Count++;
				word_Count--;
			}
			if (tail->Left->data == '\n')
			{
				//to make the previous pointer to point on the next line
				if (line_Count > 1)
				{
					prev_Line = current_Line;
				}
				current_Line = tail;
			}
			if (prev_Line != NULL && line_Count >= 1)
			{
				if (tail->data == '\n')
				{

				}
				else
				{
					prev_Line->Bottom = tail;
					tail->up = prev_Line;
					prev_Line = prev_Line->Right;
				}
			}
		}
	}
	void Insert_Middle()
	{
		string data;
		int row, col;
		cout << "\nenter the row position : ";
		cin >> row;
		cout << "\nenter the column position : ";
		cin >> col;
		cout << "\nenter the character ";
		cin.ignore();
		getline(cin, data);
		for (int i = 0; i < data.length(); i++)
		{
			if (i == 0)
			{
				position_(col, row, ' ');
			}
			col++;
			position_(col, row, data[i]);
		}
		position_(++col, row, ' ');
	}
	//changing the links of up
	void change_Top(Node*& temp, Node*& s)
	{

		temp = temp->up;
		while (temp != NULL && temp->data != '\n' && s != NULL && s->data != '\n')
		{
			s->up = temp;
			temp->Bottom = s;
			s = s->Right;
			temp = temp->Right;
		}
		if (temp != NULL && temp->data == '\n')
		{
			while (temp != NULL && temp->data == '\n')
			{
				temp->Bottom = NULL;
				temp = temp->Right;
			}
		}
		if (s != NULL && s->data == '\n')
		{
			while (s != NULL && s->data == '\n')
			{
				s->up = NULL;
				s = s->Right;
			}
		}
	}
	void change_Down(Node*& temp, Node*& s)
	{
		temp = temp->Bottom;

		while (temp != NULL && temp->data != '\n' && s != NULL && s->data != '\n')
		{
			s->Bottom = temp;
			temp->up = s;
			s = s->Right;
			temp = temp->Right;
		}
		if (temp != NULL && temp->data == '\n')
		{
			while (temp != NULL && temp->data == '\n')
			{
				temp->up = NULL;
				temp = temp->Right;
			}
		}
		if (s != NULL && s->data == '\n')
		{
			while (s != NULL && s->data == '\n')
			{
				s->Bottom = NULL;
				s = s->Right;
			}
		}
	}
	//inserting at a particular poistion
	void position_(int x, int y, char data)
	{
		if (head == NULL)
		{
			return;
		}
		//row should be equal to or less then line count 
		if (y <= line_Count)
		{
			Node* temp;
			temp = head;
			//traversing to the given line 
			for (int i = 0; i < y; i++)
			{
				if (temp == NULL)
					break;
				temp = temp->Bottom;
			}
			//traversing to the given column
			for (int i = 0; i < x; i++)
			{
				if (temp->data == '\n')
					break;
				if (temp->Right == NULL)
					break;
				temp = temp->Right;
			}
			//It means we have found the exact spot where we can add the data
			if (temp != NULL)
			{
				if (temp->data != '\n')
				{
					Node* s = new Node(data);
					s->Left = temp->Left;
					temp->Left = s;
					s->Right = temp;
					if (s->Left != NULL)
						s->Left->Right = s;
					Node* down = temp;
					Node* current = s;
					//if the uperline exsists then change the links
					if (temp->up != NULL)
					{
						change_Top(temp, s);
					}
					temp = down;
					s = current;
					//if the down Nodes exsists then change the links
					if (temp->Bottom != NULL)
					{
						change_Down(temp, s);
					}
				}
			}
			else
				cout << "\nyou are exceeding the columns!!!";
		}
		else
			cout << "\nrow should not exceed the nodepad Rows!!!";
		displaying_List();
		COORD pos = { x,y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		Sleep(200);
	}
	//helper function
	void delete_Middle()
	{
		int row, start, end;
		cout << "\nenter the row no : ";
		cin >> row;
		cout << "\nenter the starting index :";
		cin >> start;
		cout << "\nenter the ending index :";
		cin >> end;
		for (int i = end; i >= start; i--)
		{
			position_Deletion(row, i);
		}
	}
	//deleeting from middle
	void position_Deletion(int row, int col)
	{
		if (head == NULL)
		{
			return;
		}
		if (row <= line_Count)
		{
			Node* temp = head;
			//traversing to the given line 
			for (int i = 0; i < row; i++)
			{
				if (temp == NULL)
					break;
				temp = temp->Bottom;
			}
			//traversing to the given column
			for (int i = 0; i < col; i++)
			{
				if (temp->data == '\n')
					break;
				if (temp->Right == NULL)
					break;
				temp = temp->Right;
			}
			if (temp != NULL)
			{
				Node* s = nullptr, * temp2 = nullptr;
				s = temp;
				temp2 = temp->Right;
				temp = temp->Left;
				temp->Right = temp2;
				if (temp2 != NULL)
					temp2->Left = temp;
				s->Left = NULL, s->Right = NULL, s->up = NULL, s->Bottom = NULL;
				delete s;
				word_Count--;
				Node* down = temp;
				Node* current = temp2;
				//if the uperline exsists then change the links
				if (temp->up != NULL)
				{
					change_Top(temp, temp2);
				}
				temp = down;
				s = current;
				//if the down Nodes exsists then change the links
				if (temp->Bottom != NULL)
				{
					change_Down(temp, temp2);
				}
			}
			else
				cout << "\nyou exceeded the columns no column exsist!!!";
		}
		else
			cout << "\n\nyou exceeded the row limit no row exsists of this no";
		displaying_List();
		COORD pos = { col,row };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		Sleep(200);
	}
	void displaying_List()
	{
		COORD pos = { 90, 29 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		cout << "word count : " << word_Count << " Line count : " << line_Count;
		Node* s = head;
		pos = { 0, 0 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
		while (s != NULL)
		{
			cout << s->data;
			s = s->Right;
		}
		delete s;
	}
	void delete_Node(char a)
	{
		if (head == NULL)
		{
			if (undo.is_Empty() == false)
			{
				redo.Push_insert(undo.Top_Value());
				undo.pop_stack();
			}
			return;
		}
		//it mean tail==head
		if (tail->Left == NULL)
		{
			head = NULL;
			word_Count--;
		}
		if (head != NULL)
		{
			//deleting the recent insertion or the last node in 4d Linked list
			if (tail->Right == NULL)
			{
				if (prev_Line != NULL)
					prev_Line = prev_Line->Left;
				redo.Push_insert(undo.Top_Value());
				undo.pop_stack();
				Node* s;
				//if tail is connected from the top with another Node
				if (tail->up != NULL)
				{
					s = tail->up;
					s->Bottom = NULL;
					tail->up = NULL;
				}
				s = tail;
				if (s->data == '\n')
					line_Count--;
				if (s->data != '\n')
					word_Count--;
				tail = tail->Left;
				tail->Right = NULL;
				s->Left = NULL;
				delete s;
			}
		}
	}
	void search_Node()
	{
		int times = 0; // no of times a no has occured in the Advanced node pad
		string s;
		cout << "\nenter the word you want to find\n";

		getline(cin, s);

		// storing null pointer in the last index to avoid buffer overflow
		int count = 0;
		Node* temp = head;
		for (int i = 0; i < s.length(); i++)
		{
			if (temp == NULL)
				break;
			if (temp->data == s[i])
			{
				count++;
				if (count == s.length())
				{
					//incrementing the times the number occured
					times++;
					//resetting the loop
					count = 0;
					i = -1;
				}
			}
			else
			{
				count = 0;
				i = -1;
			}
			temp = temp->Right;
		}
		if (times == 0)
			cout << "\n" << s << " has no occurrence in the Node pad\n";
		else
			cout << "\n" << s << " has occurred " << times << " time(s) in the Node pad\n";
		//sleep for 2sec
		Sleep(2000);
	}

	void word_Replacement()
	{
		string word, replace;
		cout << "\nenter the string : ";
		cin >> word;
		cout << "\nenter the word you want to replace with : ";
		cin >> replace;
		Node* temp = head;
		int count = 0;
		if (word.length() == replace.length())
		{
			Node* start = NULL, * end = NULL;
			for (int i = 0; i < word.length(); i++)
			{
				if (temp == NULL)
					break;
				if (temp->data == word[i])
				{
					if (i == 0)
					{
						start = temp;
					}
					count++;
					if (count == word.length())
					{
						end = temp;
						break;
					}
				}
				else
				{
					//loop reseting
					start = NULL;
					count = 0;
					i = -1;
				}
				temp = temp->Right;
			}
			if (start != NULL && end != NULL)
			{
				for (int i = 0; i < replace.length(); i++)
				{
					start->data = replace[i];
					start = start->Right;
				}
				system("cls");
				displaying_List();
				cout << "\n successfully replaced first occurence of the word :" << word << " with :" << replace << endl;
			}
			else
				cout << word << "\nNo occurance found in Node Pad\n";
		}
		else
			cout << "\nthe length of both the words should be same!!!\n";
		system("pause");
	}
	void undo_Text()
	{
		//checking if the undo stack is empty or not
		if (undo.is_Empty() == false)
		{
			char text = undo.Top_Value();
			if (text == '-1')
				return;
			else
				delete_Node(text);
		}
	}
	void redo_Text()
	{
		if (redo.is_Empty() == false)
		{
			char text = redo.Top_Value();
			if (text == '-1')
				return;
			else
			{
				redo.pop_stack();
				insert_Node(text);
			}
		}
	}
	void Copy_Text()
	{
		if (head != NULL)
		{
			int row, start, end;
			cout << "\n\nenter the row where the string is : ";
			cin >> row;
			cout << "enter the starting index of the string : ";
			cin >> start;
			cout << "enter the end index of a string : ";
			cin >> end;
			string copy(end - start, ' ');
			if (!(row > line_Count))
			{
				Node* temp = head;
				//traversing to the given row
				for (int i = 0; i < row; i++)
				{
					if (temp == NULL)
						break;
					temp = temp->Bottom;
				}
				if (temp != NULL)
				{
					//traversing to the given column
					for (int i = 0; i < start; i++)
					{
						if (temp->data == '\n')
							break;
						temp = temp->Right;
					}
					if (temp->data != '\n')
					{
						int k = 0;
						COORD pos = { start, row };
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
						for (int i = start; i <= end; i++, k++, pos.X++)
						{
							copy[k] = temp->data;
							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
							//sleep for one sec
							Sleep(500);
							temp = temp->Right;
						}
						copy_Text = copy;
					}
					else
						cout << "the ending index exceeds the columns so it cannot be copied\n";
				}
			}
			Sleep(1000);
		}
	}

	void Paste_Text()
	{
		if (copy_Text.empty())
			cout << "\n\nfirst you need to copy the data !!!\n";
		else
		{
			cout << "\n\nwhere you want to paste the Text\n";
			int row, col;
			cout << "\nenter the row position : ";
			cin >> row;
			cout << "\nenter the column position : ";
			cin >> col;
			if (row <= line_Count)
			{
				for (int i = 0, k = 0; i <= copy_Text.length(); i++, k++, col++)
					position_(col, row, copy_Text[k]);
			}
			else
				cout << "\n\ndo not exceed the rows";
		}
		//sleep for 0.5sec
		Sleep(500);
	}
	void displaying_Instrunction()
	{
		cout << "-----------------------------------------------Advanced Node Pad-----------------------------------------------";
		cout << "\nwelcome !!! carefully Read the instrucntions below ";
		cout << "\n\npress (Enter) key for next Line";
		cout << "\npress(escape)  key to exit";
		cout << "\npress (backspace) key to delete\n";
		cout << "press (@) key for searching\n";
		cout << "press ($) key for replacing\n";
		cout << "press (control-z) key to undo the action\n";
		cout << "press (control-y) key to redo the action\n";
		cout << "press (-) to delete at a particular point\n";
		cout << "press (+) to insert at a particular point\n";
		cout << "press (control-c) key to copy a given text from the Nodepad\n";
		cout << "press (control-v) key to paste a given text in the Nodepad\n";
		cout << "press (TAB) key to paste choose from the given suggestion\n";
	}
	//for saving data into file
	void read_File()
	{
		vector<string>obj, obj2;
		string s, str;
		ifstream read;
		cout << "write 1st name of file";
		cin >> s;
		s += ".txt";
		read.open(s);
		if (read.is_open())
		{
			while (getline(read, s))
			{
				obj.push_back(s);
			}
			read.close();
			system("pause");
		}
		else
			cout << "filenot open";
		cout << "write 2nd name of file";
		cin >> str;
		str += ".txt";
		read.open(str);
		if (read.is_open())
		{
			while (getline(read, str))
			{
				obj2.push_back(str);
			}
			read.close();
			system("pause");
		}
		int i = 0;
		while (!(obj.empty()) && !(obj2.empty()))
		{
			if (obj[i] == obj2[i])
			{
				obj.pop_back(), obj2.pop_back();
			}
			else
			{
				cout << "\n\nsimiliaruty doesnt exsists ";
				break;
			}
			i++;
		}
	}
	void save_File()
	{
		string str, filename;
		cout << "\n\nenter the filename : ";
		cin >> filename;
		ofstream file;
		filename += ".txt";
		file.open(filename);
		Node* temp = head;
		/ file.open(filename, ios::app); /
			if (file.is_open())
			{
				while (temp != NULL)
				{
					if (temp->data == ' ' || temp->data == '\n')
					{
						file << str;
						str.clear();
					}
					str += temp->data;
					temp = temp->Right;
				}
				file.close();
			}
			else
			{
				cout << "file did not opened";
			}
	}
	void Make_NuLL()
	{
		prev_Line = NULL, current_Line = NULL, tail = NULL;
		if (head == NULL)
		{

		}
		else if (head->Right != NULL)
		{
			Node* temp = head->Right;
			Node* next;
			head->Right = NULL;
			while (temp != NULL)
			{
				next = temp->Right;
				temp->Right = NULL, temp->Bottom = NULL, temp->up = NULL, temp->Left = NULL;
				delete temp;
				temp = next;
			}
		}
		delete head;
	}
	~List()
	{
		Make_NuLL();
	}
};
//for Trie
struct TrieNode
{
	TrieNode* words[26];
	bool End;
	TrieNode()
	{
		memset(words, NULL, sizeof(words));
		End = false;
	}
};
class Trie :public List
{
	vector<string>suggestion;
	TrieNode* root;
public:
	Trie()
	{
		//making the root Node NULL
		root = new TrieNode();
	}
	void insert_Trie(string& s)
	{
		insert(root, s);
	}
	void insert(TrieNode*& s, string data)
	{
		// We have reached the last character
		if (data.length() == 0)
		{
			s->End = true;
			return;
		}
		// Calculating the index number of the small alphabets
		int index = data[0] - 'a';
		TrieNode* temp;
		// If the character exists
		if (s->words[index] != NULL)
			temp = s->words[index];
		else
		{
			temp = new TrieNode();
			s->words[index] = temp;
		}
		insert(temp, data.substr(1));
	}
	void Load_Data()
	{
		string s;
		ifstream out;
		out.open("outfile.txt", ios::in);
		//reading the file till end of file
		while (out >> s)
		{
			insert_Trie(s);
		}
		out.close(); // close the file after readin
	}
	void WordSuggestion_Trie(const string prefix)
	{
		bool flag = true;
		//checking if the strin exsists between a-z
		for (char check : prefix)
		{
			//becasue our we  are only dealing with lower alphbets
			if (check >= 'a' && check <= 'z')
			{
			}
			else
			{
				flag = false;
				break;
			}
		}
		if (flag == true)
		{
			//emptying the vector
			if (suggestion.size() != 0)
				suggestion.clear();
			TrieNode* s = root;
			for (char value : prefix)
			{
				int index = value - 'a';
				if (s->words[index] == NULL)
				{
					cout << "\nNo suggestions ||No word found in Trie\n\n";
					return;
				}
				s = s->words[index];
			}
			int count = 0;
			Suggestions(s, prefix, count);
		}
	}

	//if the user wants to select from the suggestions
	void select(const string value)
	{
		cout << endl << endl << "----Suggestions----" << endl;

		for (int i = 0; i < suggestion.size(); i++)
			cout << "\npress" << i << " for " << suggestion[i];

		cout << "\npress the index or order no of the suggestion you want to choose\n";
		int choice;
		cin >> choice;

		if (choice >= 0 && choice < suggestion.size())
		{
			string s = suggestion[choice];
			s = s.substr(value.length());
			cout << s << endl;
			while (s.length() != 0)
			{
				insert_Node(s[0]);
				cout << endl << s[0];
				s = s.substr(1);
			}
		}
		else
			cout << "\nerror: you didn't select from the given choices\n";
	}

	void Suggestions(TrieNode* s, const string& prefix, int& count)
	{
		if (s == NULL || count == 8)
			return;
		if (s->End == true)
		{
			suggestion.push_back(prefix);
			count++;
		}
		for (int i = 0; i < 26; i++)
		{
			if (s->words[i] != NULL)
			{
				/*	char a;*/
				string temp = prefix + char('a' + i);
				Suggestions(s->words[i], temp, count);
			}
		}
	}
	void display()
	{
		if (suggestion.size() != 0)
		{
			cout << endl << endl;
			for (int i = 0; i < suggestion.size(); i++)
				cout << "\npress" << i << " for " << suggestion[i];
		}
	}
};
int main()
{

	Trie obj;
	obj.Load_Data();
	obj.displaying_Instrunction();
	string str;
	char input;
	system("pause");
	while (true)
	{
		while (true)
		{
			system("cls");
			obj.displaying_List();
			//suggestion
			obj.display();
			input = _getch();
			//for escape key
			if (input == 27)
				break;
			//ascii for tab
			else if (input == 9)
			{
				obj.select(str);
				str.clear();
			}
			//if the input is enter key
			else if (input == 13)
				obj.insert_Node('\n'), str.clear();
			//if entered key is backspace
			else if (input == 8)
				obj.delete_Node(input);
			//for $ key
			else if (input == 36)
				obj.word_Replacement();
			//ascii for @
			else if (input == 64)
				obj.search_Node();
			//ascci value for 'control-z'
			else if (input == 26)
				obj.undo_Text();
			//ascii value for 'control-y'
			else if (input == 25)
				obj.redo_Text();
			//ascii for '+'
			else if (input == 43)
				obj.Insert_Middle();
			else if (input == '-')
				obj.delete_Middle();
			//ascii value of control-c 
			else if (input == 3)
				obj.Copy_Text();
			//ascii value of control-s
			else if (input == 19)
				obj.save_File();
			//ascii value for contol-v
			else if (input == 22)
				obj.Paste_Text();
			else
			{
				obj.insert_Node(input);
				if (input == ' ')
					str.clear();
				else
				{
					str += input;
					obj.WordSuggestion_Trie(str);
				}
			}
		}
		int choice;
		obj.displaying_Instrunction();
		cout << "\n\npress 1 to continue\n";
		cin >> choice;
		if (choice != 1)
			break;
	}
}