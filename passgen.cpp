//////////////////////////////////////////////////////////////
// PASSGEN.CPP -- easy to use text mode password generator. //
//////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <fstream>

// Functions prototypes.
void printProgramHeader();
void printPwdMenu();
unsigned getNumber(const std::string&, const unsigned, const unsigned);
bool isDigits(const std::string&);
bool isAnswerYes(const std::string&);
std::string getFilename(const std::string&, const std::string&);
std::string getPassword(const unsigned, const unsigned);
std::string getPwHead(const unsigned, const std::string);
std::string getPwBody(const unsigned, std::string = "", std::string s2 = "",std::string s3 = "");

// Program constants definition.
const std::string letters{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"};
const std::string digits{"0123456789"};
const std::string specialCharacters{"!?@#$%&+-*/\\~^`(){}[]_=.,;:|'\""};
const std::string deniedFilenameCharacters{"\\/:*?\"<>|"};
const std::string fileExtension{".txt"};
const std::string okMessage{"O.K.\n\n"};

// Main function starts here.
int main()
{
	const unsigned pwCounterMin{1};
	const unsigned pwCounterMax{1024};
	const unsigned pwCharactersMin{1};
	const unsigned pwCharactersMax{128};
	std::string message;
	
	printProgramHeader();
	
	message = "Enter the number of generated passwords (" + std::to_string(pwCounterMin) + " - " + std::to_string(pwCounterMax) + "): ";
	unsigned pwCounter{getNumber(message, pwCounterMin, pwCounterMax)};
	
	message = "Enter the length of the each password in characters (" + std::to_string(pwCharactersMin) + " - " + std::to_string(pwCharactersMax) + "): ";
	unsigned pwLength{getNumber(message, pwCharactersMin, pwCharactersMax)};
	
	printPwdMenu();	
	message = "Your choice (1 - 16): ";
	unsigned usersChoice{getNumber(message, 1, 16)};
	
	message = "Do you wants to save the new generated password into the text file? (yes/no): ";
	bool saveToFile{isAnswerYes(message)};
	
	std::string fileName;
	std::ofstream fo;
	
	// File operations handling.
	if (saveToFile)
	{
		bool fileExist{true};
		
		while (fileExist)
		{
			message = "Enter the file name without path, quotes and any extension (e.g. mypasswords): ";
			fileName = getFilename(message, fileExtension);
			
			std::ifstream fi(fileName, std::ios::in);
			
			if (fi)
			{
				std::cerr << "The file \"" << fileName << "\" is already exist! Choose another name.\n\n";
				fi.close();
			}
			else
			{
				fileExist = false;
			}
		}
		
		std::cout << okMessage;
		
		fo.open(fileName, std::ios::out);
		
		if (!fo)
		{
			std::cerr << "Can't open the \"" << fileName << "\" file!\n";
			std::cerr << "Perhaps you don't have enough privelegies to write into the current directory.\n\n";
		}
	}

	// Data output.
	std::string password;
	
	for (unsigned i{0}; i < pwCounter; i++)
	{
		password = getPassword(usersChoice, pwLength);
		
		if (fo)
		{
			fo << password << '\n';
		}
		
		if (!fo)
		{
			std::cout << password << '\n';
		}
	}
	
	// File closing.
	if (fo)
	{
		fo.close();
	}
	
	std::cout << "Job done!\n";
	return 0;
}

////////////////////////////////////////
// Functions definitions starts here. //
////////////////////////////////////////

// Prints program header.
void printProgramHeader()
{
	std::cout << "PassGen 0.0.1\n";
	std::cout << "Copyleft (@) Ate-my-door.\n\n";
}

// Prints main menu.
void printPwdMenu()
{
	std::cout << "Choose one of the following:\n\n";
	
	std::cout << " 1. Use DIGITS only. ('1', '2', '3' etc.)\n";
	std::cout << " 2. Use LETTERS only. ('A', 'B', 'C' etc.)\n";
	std::cout << " 3. Use SPECIAL CHARACTERS only. ('!', '@', '#' etc.)\n\n";
	
	std::cout << " 4. Use DIGITS and LETTERS only. The 1st character is a DIGIT.\n";
	std::cout << " 5. Use DIGITS and LETTERS only. The 1st character is a LETTER.\n";
	std::cout << " 6. Use DIGITS and LETTERS only. The 1st character is a DIGIT or a LETTER.\n\n";
	
	std::cout << " 7. Use DIGITS and SPECIAL CHARACTERS only. The 1st character is a DIGIT.\n";
	std::cout << " 8. Use DIGITS and SPECIAL CHARACTERS only. The 1st character is a SPECIAL CHARACTER.\n";
	std::cout << " 9. Use DIGITS and SPECIAL CHARACTERS only. The 1st character is a DIGIT or a SPECIAL CHARACTER.\n\n";
	
	std::cout << "10. Use LETTERS and SPECIAL CHARACTERS only. The 1st character is a LETTER.\n";
	std::cout << "11. Use LETTERS and SPECIAL CHARACTERS only. The 1st character is a SPECIAL CHARACTER.\n";
	std::cout << "12. Use LETTERS and SPECIAL CHARACTERS only. The 1st character is a LETTER or a SPECIAL CHARACTER.\n\n";
	
	std::cout << "13. Use ALL. The 1st character is a DIGIT.\n";
	std::cout << "14. Use ALL. The 1st character is a LETTER.\n";
	std::cout << "15. Use ALL. The 1st character is a SPECIAL CHARACTER.\n";
	std::cout << "16. Use ALL. The 1st character is a DIGIT or a LETTER or a SPECIAL CHARACTER.\n\n";
}

// Gets a number between min and max values from the users input.
unsigned getNumber(const std::string& s, const unsigned min, const unsigned max)
{
	unsigned long long n;
	std::string userInput;
	
	do
	{
		do
		{
			std::cout << s;
			getline(std::cin, userInput);
			
		} while (!isDigits(userInput));
	
		n = stoi(userInput);	
		
	} while ((n < min) || (n > max));
	
	std::cout << okMessage;	
	return static_cast<unsigned>(n);
}

// Check if entered value consist of a digits only.
bool isDigits(const std::string& s)
{
	if (s == "")
	{
		return false;
	}
	else
	{
		return s.find_first_not_of(digits) == std::string::npos;
	}
}

// Returns true if the user's answer starts with 'y', else returns false.
bool isAnswerYes(const std::string& s)
{
	std::string userInput;

	do
	{	
		std::cout << s;
		getline(std::cin, userInput);
		
	} while ((std::tolower(userInput[0]) != 'y') && (std::tolower(userInput[0]) != 'n'));
	
	std::cout << okMessage;
	return (std::tolower(userInput[0]) == 'y');
}

// Gets the name of file to save the passwords.
std::string getFilename(const std::string& s, const std::string& fext)
{
	std::string userInput;
	
	do
	{
		std::cout << s;
		getline(std::cin, userInput);
		
	} while ((userInput == "") || (userInput.find_first_of(deniedFilenameCharacters) != std::string::npos));
	
	return (userInput + fext);
}

// Generates the password based on the previous user input.
std::string getPassword(const unsigned choice, const unsigned length)
{
	std::string pwHead;
	std::string pwBody;
	
	switch (choice)
	{
		case 1:
			// Use digits only.
			pwHead = getPwHead(length, digits);
			pwBody = getPwBody(length, digits);
			break;
			
		case 2:
			//  Use letters only.
			pwHead = getPwHead(length, letters);
			pwBody = getPwBody(length, letters);
			break;
			
		case 3:
			// Use special characters only.
			pwHead = getPwHead(length, specialCharacters);
			pwBody = getPwBody(length, specialCharacters);
			break;
			
		case 4:
			// Use digits and letters only. The 1st character is a digit.
			pwHead = getPwHead(length, digits);
			pwBody = getPwBody(length, digits, letters);
			break;
			
		case 5:
			// Use digits and letters only. The 1st character is a letter.
			pwHead = getPwHead(length, letters);
			pwBody = getPwBody(length, digits, letters);
			break;
			
		case 6:
			// Use digits and letters only. The 1st character is a digit or a letter.
			pwHead = getPwHead(length, digits + letters);
			pwBody = getPwBody(length, digits, letters);
			break;
			
		case 7:
			// Use digits and special characters only. The 1st character is a digit.
			pwHead = getPwHead(length, digits);
			pwBody = getPwBody(length, digits, specialCharacters);
			break;
			
		case 8:
			// Use digits and special characters only. The 1st character is a special character.
			pwHead = getPwHead(length, specialCharacters);
			pwBody = getPwBody(length, digits, specialCharacters);
			break;
			
		case 9:
			// Use digits and special characters only. The 1st character is a digit or a special character.
			pwHead = getPwHead(length, digits + specialCharacters);
			pwBody = getPwBody(length, digits, specialCharacters);
			break;
			
		case 10:
			// Use letters and special characters only. The 1st character is a letter.
			pwHead = getPwHead(length, letters);
			pwBody = getPwBody(length, letters, specialCharacters);
			break;
			
		case 11:
			// Use letters and special characters only. The 1st character is a special character.
			pwHead = getPwHead(length, specialCharacters);
			pwBody = getPwBody(length, letters, specialCharacters);
			break;
		
		case 12:
			// Use letters and special character only. The 1st character is a letter or a special character.
			pwHead = getPwHead(length, letters + specialCharacters);
			pwBody = getPwBody(length, letters, specialCharacters);
			break;
			
		case 13:
			// Use all. The 1st character is a digit.
			pwHead = getPwHead(length, digits);
			pwBody = getPwBody(length, digits, letters, specialCharacters);
			break;
			
		case 14:
			// Use all. The 1st character is a letter.
			pwHead = getPwHead(length, letters);
			pwBody = getPwBody(length, digits, letters, specialCharacters);
			break;
			
		case 15:
			// Use all. The 1st character is a special character.
			pwHead = getPwHead(length, specialCharacters);
			pwBody = getPwBody(length, digits, letters, specialCharacters);
			break;
			
		case 16:
			// Use all. The 1st character is a digit or a letter or a special character.
			pwHead = getPwHead(length, digits + letters + specialCharacters);
			pwBody = getPwBody(length, digits, letters, specialCharacters);
			break;
		
	}
	
	return (pwHead + pwBody);
}

// Gets the first character of the password.
std::string getPwHead(const unsigned len, std::string chars)
{
	if (len == 0)
	{
		return "";
	}
	else
	{
		std::string hd{""};
	
		std::random_shuffle(chars.begin(), chars.end());
		hd.push_back(chars[0]);
	
		return hd;
	}
}

// Gets the rest of the password.
std::string getPwBody(const unsigned len, std::string s1, std::string s2, std::string s3)
{	
	std::string bd{""};
	
	if ((len == 0) || (len == 1))
	{
		return "";
	}
	else if (len <= 3)
	{	
		while (bd.size() < (len - 1))
		{
			if (s1 != "")
			{
				std::random_shuffle(s1.begin(), s1.end());
				bd.push_back(s1[0]);
			}
			
			if (s2 != "")
			{
				std::random_shuffle(s2.begin(), s2.end());
				bd.push_back(s2[0]);
			}
				
			if (s3 != "")
			{
				std::random_shuffle(s3.begin(), s3.end());
				bd.push_back(s3[0]);
			}
		}
		
		std::random_shuffle(bd.begin(), bd.end());
		return bd.substr(0, len - 1);
	}
	else
	{
		std::string sall{s1 + s2 + s3};
		std::random_device rd;
		std::uniform_int_distribution<unsigned> dist(1, (len - 1) / 3);	
		unsigned limit{dist(rd)};
		
		if (s1 != "")
		{
			for (unsigned i{0}; i < limit; i++)
			{
				std::random_shuffle(s1.begin(), s1.end());
				bd.push_back(s1[0]);
			}
		}
		
		limit = dist(rd);
		
		if (s2 != "")
		{
			for (unsigned i{0}; i < limit; i++)
			{
				std::random_shuffle(s2.begin(), s2.end());
				bd.push_back(s2[0]);
			}
		}
		
		limit = dist(rd);
		
		if (s3 != "")
		{
			for (unsigned i{0}; i < limit; i++)
			{
				std::random_shuffle(s3.begin(), s3.end());
				bd.push_back(s3[0]);
			}
		}
		
		while (bd.size() < (len - 1))
		{
			std::random_shuffle(sall.begin(), sall.end());
			bd.push_back(sall[0]);
		}
		
		std::random_shuffle(bd.begin(), bd.end());
		return bd.substr(0, len - 1);
	}
}
