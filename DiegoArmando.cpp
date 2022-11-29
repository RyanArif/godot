/*
Created by: Ryan Arif (Legendary Ryan)
LICENSED under GPLv3

What does this do?
Outputs a random quote from the Ace Attorney character, Godot.
Then, it displays an image of Godot.
*/

#include<iostream> //output to console
#include<string> //strings
#include<fstream> //read in from the long list of godot quotes
#include<random> //for randomizing the quote

using namespace std;

const string GODOT_IMAGE = "godot-utf.txt"; //set this to the file containing the utf image of godot (basically ascii art kind of)
const string GODOT_QUOTES = "godot.txt"; //set this to the file of godot quotes. Format should be one quote per line. No whitespace.

void sipCoffee(); //animation of Godot sipping coffee
string getQuote(); //gets a quote from the file that holds the Godot quotes, and returns it as a string

int main()
{
  string godotSays = "error: 19"; //error line 19 (this line lol)

  godotSays = getQuote(); //gets a quote from the database of quotes

  cout << godotSays << endl; //print the quote from Godot

  sipCoffee(); //Godot sips some coffee.

  return 0;
}

//get an image of Godot and display him on the screen!
void sipCoffee()
{
  ifstream infile(GODOT_IMAGE);
  while(infile){
    string t = "";
    getline(infile, t);
    cout << t << endl;
  };
  infile.close();
  cout << endl;
}

//returns a string that is a randomized quote from the file godot.txt
string getQuote()
{
  //RNG: https://en.cppreference.com/w/cpp/numeric/random/random_device
  string theQuote = "error: getQuote()"; //if this string isn't overridden, something went wrong in here. Investigate!
	random_device rd; // obtain a random number from hardware
	mt19937 gen(rd()); // seed the generator

  ifstream infile(GODOT_QUOTES); // open a new file object
  infile.seekg(0L, ios::end); //navigate to the end of the file
  int totalBytes = infile.tellg(); //this is the total bytes that make up the file
  uniform_int_distribution<> distr(0, totalBytes-1); //set up the rng parameters
  int randomNum = distr(gen);

  //if the random number starts off at the beginning of the file (the start of the first line), then we don't need to do anything. Just return whatever that line is and we're done.
  if(randomNum == 0)
  {
    getline(infile, theQuote);
    infile.close();
    return theQuote;
  }

  infile.seekg(randomNum, ios::beg); //navigate to the random position
  //go forward until we hit the new line character
  bool rewind = true;
  for(int i = randomNum; i < totalBytes-1; i++){
    char ch = '\0';
    //infile.seekg(1L, ios::cur); //move forward one byte
    infile.get(ch); //already moves us forward one byte anyway I think
    if(ch == '\n'){
      //we have reached our destination. time to go!
      rewind = false; //dont need to rewind! forward was good.
      break;
    }
  }
  if (rewind){ //Didn't find what we needed? Be kind, rewind.
    infile.clear(); //clear any failed bits (caused by the file.get() object)
    infile.seekg(randomNum, ios::beg); //reset the seek to our random number
    for(int i = randomNum; i > 0; i--){ //loop backwards
      char ch = '\0';
      infile.seekg(-2L, ios::cur); //rewind back two bytes, to counter the whole ifstream.get object
      infile.get(ch);
      if(ch == '\n'){
        //wow! It's a new line! Super dope.
        break;
      }
    }
  }

  getline(infile, theQuote);

  infile.close(); //close the file

  return theQuote;
}

