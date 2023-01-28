#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <bitset>					//for FileOutput
#include <cmath>
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

//The program that compresses a file using a huffman method

//The frequency of particular character is its occurence in a row


class BitBuffer				//Buffer for proper bits
{
public:

	FILE* file; 
	unsigned char buffer = 0;
	unsigned count = 0;

	void outputBit(unsigned char bit)
	{
		buffer <<= 1;         // Make room for next bit.
		if (bit) buffer |= 1; // I bit equals 1, add 1 to the buffer
		count++;              // Sionce another bit was added, we need to make a place for next move
		if (count == 8)       //write a character only if we rach 8 bits
		{
			fwrite(&buffer, sizeof(buffer), 1, file);  //writing a character in our file
			cout << buffer;							   //Informing the user which character is being written into the file
			buffer = 0;                                //resetting the buffer
			count = 0;                                 //resetting the counter
		}
	}
};


string MinValueKey(map<string, int> quantityMap)
{

	int minValue = quantityMap.begin()->second;

	string minValueKey = quantityMap.begin()->first;																//key = string for particular frequency


	for (auto pair : quantityMap)			//for loop from beginning of the map to its end
	{
		if (pair.second < minValue)
		{
			minValue = pair.second;													//second value of the iterator is our frequency

			minValueKey = pair.first;												//key for the lowest frequency

		}
	}

	return minValueKey;
}

int Encryption(map<string, int>& quantityMap, map<char, string>& cypherMap)
{
	while (quantityMap.size() > 1)				//as long as there is more than 1 individual character 
	{
		string first_minValueKey = MinValueKey(quantityMap);
		int first_minValue = quantityMap[first_minValueKey];
		quantityMap.erase(first_minValueKey);			//deletes value of the minimal frequency


		string second_minValueKey = MinValueKey(quantityMap);
		int second_minValue = quantityMap[second_minValueKey];
		quantityMap.erase(second_minValueKey);			//deletes value of another minimal frequency


		string newKey = first_minValueKey + second_minValueKey;			//we substitute previous keys with new one that is the combination of both
		int newValue = first_minValue + second_minValue;
		quantityMap[newKey] = newValue;


		if (first_minValue > second_minValue)
		{
			for (char subKey : first_minValueKey)				//execute this loop through all characters of the first minValueKey
			{
				cypherMap[subKey] = "1" + cypherMap[subKey];						//assign 1 if characters in the first minValueKey of such figure have greater frequency than the second one
			}

			for (char subKey : second_minValueKey)				//execute this loop through all characters of the second minValueKey
			{
				cypherMap[subKey] = "0" + cypherMap[subKey];
			}


		}
		else
		{
			for (char subKey : first_minValueKey)				//execute this loop through all characters of the first minValueKey
			{
				cypherMap[subKey] = "0" + cypherMap[subKey];						//assign 0 if characters in the first minValueKey of such figure have smaller frequency than the second one
			}

			for (char subKey : second_minValueKey)				//execute this loop through all characters of the second minValueKey
			{
				cypherMap[subKey] = "1" + cypherMap[subKey];
			}


		}

	}

	return 0;
}

int Compression(string sentence)
{
	BitBuffer buf;										//creating a buffer

	fopen_s(&buf.file, "Compressed Output.txt", "a");           //opening the file
	
	cout << "\n\nCompressed version of the text: ";        //Informing the user what is written in the file

	for (char character : sentence)                 //a loop that goes through every character in the initial set of bits
	{

		if (character == '0')                   //if the character equals 0, insert a 0 input into our function that adds such 0 to the buffer
		{
			buf.outputBit(0);
		}

		else if (character == '1')              //if the character equals 1, insert a 1 input into our function that adds such 1 to the buffer
		{
			buf.outputBit(1);
		}

		else                                    //in case a charatcer other to 0 and 1 is detected, print the error alongside the faulty character (its ascii representation)
		{
			cout << "\nError " << (int)character;
			break;
		}

	}


	while (buf.count != 0)                        //if we end up with number of characters that is not the multiplication of 8, add 0 bits (a couple of zeroes) to the last character
	{
		buf.outputBit(0);
	}

	return 0;
}

void FileInput(string& sentence)
{
	string  initialInput = "Hello World!";
	char option1[50];						//for the initial option
	fstream Text;

	Text.open("Text to compress.txt", ios::in);

	if (Text.is_open())				//checking whether the file is open	
	{
		getline(Text, sentence);
		if (sentence[0] != NULL) initialInput = sentence;

		Text.close();					 //close the file object
	}

	cout << "Would you like to write your text here or to insert it directly from the file (currently choose sentence: "<< initialInput<<")? \nWrite h for upper option or f for latter one : "<<endl;
	cin >> option1;
	
	
	while (option1[0] != 102 && option1[0] != 104)					//as long as the user has inputed neither letter h nor f
	{
		cout << "\n\nPlease determine a proper character!: \n";
		cin >> option1;
		
	}

	if (option1[0] == 104)											//if option "here" was chosen, execute this if
	{
		cin.get(option1[0]);										//option1[0] receives 0x0A character, i.e. - thanks to this the getline function below works properly
		cout << "Please, insert the text that you would like to compress: "<<endl;
		getline(cin, initialInput);
	}
	
	Text.open("Text to compress.txt", ios::out);  // open a file to perform write operation using file object

	if (Text.is_open())					//checking whether the file is open
	{
		Text << initialInput;					//inserting text
		Text.close();					 //close the file object
	}
	
	Text.open("Text to compress.txt", ios::in);

	if (Text.is_open())				//checking whether the file is open	
	{
		getline(Text, sentence);

		Text.close();					 //close the file object
	}

	cout << "Text from the file: " << sentence<<endl;

}

void FileOutput(string& output)
{
	fstream Text;

	Text.open("Output.txt", ios::out);  // open a file to perform write operation using file object

	if (Text.is_open())					//checking whether the file is open
	{
		Text << output;					//inserting text
		Text.close();					 //close the file object
	}


}


int main()
{
	string input;		

	FileInput(input);

	int inLength = input.length();					//the length of our input

	map<string, int> quantityMap;						//map for frequencies		

	map<char, string> cypherMap;				//map for cyphered message


	for (int i = 0; i < inLength; i++)				//counting the number of occurences for particular character
	{
		string key;
		key = input[i];

		quantityMap[key]++;
	}

	Encryption(quantityMap, cypherMap);

	string outputStr;		//output as a string

	for (char in : input)			//for every character in the string, replace it with binary numbers from cyphermap
	{
		outputStr = outputStr + cypherMap[in];
	}

	cout <<"\nThe binary equivalence of the text above: "<< outputStr;

	FileOutput(outputStr);
	Compression(outputStr);

	return 0;
}