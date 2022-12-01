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
#include<filesystem> //to get the path to the executable and the resources

using namespace std;

const string GODOT_IMAGE = "./godot-utf.txt"; //set this to the file containing the utf image of godot (basically ascii art kind of)
const string GODOT_QUOTES = "./godot.txt"; //set this to the file of godot quotes. Format should be one quote per line. No whitespace.

void sipCoffee(); //animation of Godot sipping coffee
void printQuote(string);
string getQuote(); //gets a quote from the file that holds the Godot quotes, and returns it as a string
string wrapText(string, int);
void clear(); //clear the terminal

int main(int argc, char** argv)
{
  //handle command line args
  bool quiet = false;
  for(int i = 0; i < argc; i++){
    string arg = argv[i];
    //if(argv[i] == "-q") //doesn't work, I think it might have something to do with the whole char to string thing maybe? I don't really know.
    if (arg == "-q" || arg == "--quiet"){ //don't output anything
      quiet = true;
    }
  }

  //only works on Linux as far as I can see!
  //Gets the current working directory of the executable
  //this way, the program always has the Godot sippy sprite and the quotes list!
  filesystem::path exepath = filesystem::canonical("/proc/self/exe");
  exepath.remove_filename();
  filesystem::current_path(exepath);

  //get the quote
  string godotQuote = "ERROR: failed ot get quote";
  godotQuote = getQuote();

  //only output the quote, nothing else.
  if(quiet){
    cout << godotQuote << endl;
    return 0;
  }

  //output to the terminal
  sipCoffee(); //Godot sips some coffee.
  printQuote(godotQuote); //prints the new quote to the screen
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
}

//prints the godot quote, wrapped in a cute text box, to the console.
//string quote is the randomized quote from the other random quote function
void printQuote(string quote)
{
  const string ANSI_BLUE = "\u001B[34m";
  const string ANSI_RESET = "\u001B[0m";

  quote = wrapText(quote, 35);
  //42 -'s
  cout << ANSI_BLUE << R"( _____)" << endl <<
"|" << ANSI_RESET << "Godot" << ANSI_BLUE << R"(|
|-------------------------------------------|)" << endl << "|";

  int count = 0;
  for(int i = 0; i < quote.length(); i++){
    if (quote.at(i) == '\n'){
      for(int j = 0; j <= 42-count; j++)
        cout << " ";
      cout << "|" << quote.at(i) << "|";
      count = 0;
    }else{
      cout << ANSI_RESET << quote.at(i) << ANSI_BLUE;
      count++;
    }
  }
  for(int i = 0; i <= 42-count; i++){
    cout << " ";
  }
  cout << "|" << endl <<
  R"(|-------------------------------------------|)" << endl << ANSI_RESET;
}

//wraps the text by replacing a space with a newline char
string wrapText(string text, int loc)
{
  if(loc > text.length()){
    return text; //dont do anything
  }
  for(int i = loc; i < text.length(); i++){
    if ((i % loc == 0)){
      int spaceLoc = text.rfind(' ', i);
      text.at(spaceLoc) = '\n';
    }
  }

  return text;
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

void clear()
{
  // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
  std::cout << "\x1B[2J\x1B[H";
}

