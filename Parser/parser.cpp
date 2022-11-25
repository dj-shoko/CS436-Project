#include<iostream>
#include<fstream>
#include<string>
using namespace std;

#define DEBUG "DEBUG: RESERVED WORD TYPE IS "

/* INSTRUCTION:  Complete all ** parts.
   You may use any method to connect this file to scanner.cpp
   that you had written.  
  e.g. You can copy scanner.cpp here by:
          cp ../ScannerFiles/scanner.cpp .  
       and then append the two files into one: 
          cat scanner.cpp parser.cpp > myparser.cpp
*/

//=================================================
// File parser.cpp written by Group Number: 12
//=================================================

//=================================================
// File scanner.cpp appended
//=================================================

// --------- Two DFAs ---------------------------------

// WORD DFA
// Done by: Raymond Quach, Arnold Bermejo, Luis Zamora
/* RE: (vowel | vowel n | consonant vowel | consonant vowel n |
consonant-pair vowel | consonant-pair vowel n)^+ */

bool word (string s)
{
  char state = 0; // Initialize the state as 0 as the starting state
  int charpos = 0; // Initialize the character position as the first letter in string

  // Loop to go through each letter in the string
  while (s[charpos] != '\0')
    {
      char c = s[charpos]; // Current character in the string s

      // State 0 block
      // Check if it is a vowel state
      if (state == 0 && (c == 'a' || tolower(c) == 'e' ||
      tolower(c) == 'i' || c == 'o' || c == 'u'))
        state = 1;

      // Consanant check, non-pair state
      else if (state == 0 && (c == 'd' || c == 'j' ||
      c == 'w' || c == 'y' || c == 'z'))
        state = 'a';

      // Consanant check, pair allowed state
      else if (state == 0 && (c == 'b' || c == 'g' || c == 'h' || c == 'k'
      || c == 'm' || c == 'n' || c == 'p' || c == 'r'))
        state = 'y';

      // Special consanant in case of "ch" state
      else if (state == 0 && c == 'c')
        state = 'c';

      // Consanant 's' case this is a consonant pair state
      else if (state == 0 && c == 's')
        state = 's';

      // Consanant 't' case this is a consonant pair state
      else if (state == 0 && c == 't')
        state = 't';
      // End of state 0 block



      // Check if it is a vowel state
      else if (state == 1 && (c == 'a' || tolower(c) == 'e' ||
      tolower(c) == 'i' || c == 'o' || c == 'u'))
        state = 1;

      //Is any consonant followed up by a vowel? (qsa)
      else if ((state == 'a' || state == 'y' || state == 's' || state == 't') &&
      (c == 'a' || tolower(c) == 'e' || tolower(c) == 'i' || c == 'o' || c == 'u'))
        state = 1;

      // Is it a vowel followed up by an n
      else if (state == 1 && s[charpos] == 'n') {
        if (s[charpos-1] != 'n') // If it is a vowel+'n' (state 1)
          state = 1;
        else // If it was previously a vowel+'n' followed up by another 'n' (qy)
          state = 'y';
      }



      // Consanant check, pair allowed state
      else if (state == 1 && (c == 'b' || c == 'g' || c == 'h' ||
      c == 'k' || c == 'm' || c == 'p' || c == 'r'))
        state = 'y';

      // Special consanant in case of "ch" state
      else if (state == 1 && c == 'c')
        state = 'c';

      // Consonant Pair State Block (qy, qc, qs, and qt)
      else if (state == 'y' && c == 'y' || // bghkmnpr + 'y' pair
      ((state == 'c' || state == 's') && c == 'h') || // 'c' + 'h' and 's' + 'h' pair
      (state == 't' && c == 's')) // 't' + 's' pair
        state = 'a';

      // Consanant check, non-pair state (qsa)
      else if (state == 1 && (c == 'd' || c == 'j' ||
      c == 'w' || c == 'y' || c == 'z'))
        state = 'a';

      // Consanant 's' case this is a consonant pair state
      else if (state == 1 && c == 's')
        state = 's';

      // Consanant 't' case this is a consonant pair state
      else if (state == 1 && c == 't')
        state = 't';



      else {
        // Debugging
        // if (s != ".")
        //  cout << "DEBUG:\n\tString: " << s << ", Char: " << c << charpos << endl;
	      return(false);
      }
      charpos++;
    }

  // where did I end up????
  if (state == 1) // end in a final state
    return(true);
  else
    return(false);
}

// PERIOD DFA
// Done by: Raymond Quach
// RE:   .

bool period (string s)
{
  // complete this **
  //If the string is a period, return true
  if (s == ".")
    return true;
  else //If the string is not a period, return false
    return false;
}

// ------ Three  Tables -------------------------------------

// TABLES Done by: Raymond Quach

// Table for the token type.
enum tokentype {
  VERB, VERBNEG, VERBPAST, VERBPASTNEG, IS, WAS,
  OBJECT, SUBJECT, DESTINATION, PRONOUN, CONNECTOR,
  WORD1, WORD2, PERIOD, ERROR,
  EOFM
};

// String table for the display names of tokens, same order as "tokentype" table
string tokenName[30] = {
  "VERB", "VERBNEG", "VERBPAST", "VERBPASTNEG", "IS", "WAS",
  "OBJECT", "SUBJECT", "DESTINATION", "PRONOUN", "CONNECTOR",
  "WORD1", "WORD2", "PERIOD", "ERROR",
  "EOFM"
};

// ** Need the reservedwords table to be set up here.
// ** Do not require any file input for this. Hard code the table.
// ** a.out should work without any additional files.

// String table for reserved words
string reservedwords[30][2] = {
  {"masu", "VERB"},
  {"masen", "VERBNEG"},
  {"mashita", "VERBPAST"},
  {"masendeshita", "VERBPASTNEG"},
  {"desu", "IS"},
  {"deshita", "WAS"},
  {"o", "OBJECT"},
  {"wa", "SUBJECT"},
  {"ni", "DESTINATION"},
  {"watashi", "PRONOUN"},
  {"anata", "PRONOUN"},
  {"kare", "PRONOUN"},
  {"kanojo", "PRONOUN"},
  {"sore", "PRONOUN"},
  {"mata", "CONNECTOR"},
  {"soshite", "CONNECTOR"},
  {"shikashi", "CONNECTOR"},
  {"dakara", "CONNECTOR"},
  {"eofm", "EOFM"}
};

// ------------ Scanner and Driver -----------------------

ifstream fin;  // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
// ** Done by: Raymond Quach
int scanner(tokentype& tt, string& w)
{
  // Grab the next word from the file via fin
  fin >> w;

  cout << "Scanner called using word: " << w << endl;

  // If the word is "eofm" then return right now.
  if (w == "eofm")
    return 0;

  // Check if it is a period, return the type as period if it is.
  else if (period(w)) {
    tt = PERIOD;
    return 0;
  }

  // Call word function and if it is a word
  // And if it is then check if it is a reserved word.
  // If it is a reserved word then return the type.
  // If it is not then return the type as either WORD1 or WORD2.
  else if (word(w)) {
    //Reserved List Check
    for (int i = 0; i < 19; i++) {
      if (w == reservedwords[i][0]) {
        //If it is a reserved word, match the type
        switch (i) {
          case 0: //If the word is "masu", return type as "VERB"
            tt = VERB;
            return 0;
          case 1: //If the word is "masen", return type as "VERBNEG"
            tt = VERBNEG;
            return 0;
          case 2: //If the word is "mashita", return type as "VERBPAST"
            tt = VERBPAST;
            return 0;
          case 3: //If the word is "masendeshita", return type as "VERBPASTNEG"
            tt = VERBPASTNEG;
            return 0;
          case 4: //If the word is "desu", return type as "IS"
            tt = IS;
            return 0;
          case 5: //If the word is "deshita", return type as "WAS"
            tt = WAS;
            return 0;
          case 6: //If the word is "o", return type as "OBJECT"
            tt = OBJECT;
            return 0;
          case 7: //If the word is "wa", return type as "SUBJECT"
            tt = SUBJECT;
            return 0;
          case 8: //If the word is "ni", return type as "DESTINATION"
            tt = DESTINATION;
            return 0;
          //If the word is "watashi", "anata", "kare", "kanojo", or "sore",
          //return type as "PRONOUN"
          case 9: case 10: case 11: case 12: case 13:
            tt = PRONOUN;
            return 0;
          //If the word is "mata", "soshite", "shikashi", or "dakara",
          //return type as "CONNECTOR
          case 14: case 15: case 16: case 17:
            tt = CONNECTOR;
            return 0;
          case 18: //This should never happen...
            cout << DEBUG << "EOFM" << endl;
            tt = EOFM;
            return 0;
          default: //I sure as hell hope this never happens...
            cout << DEBUG << "ERROR" << endl;
            tt = ERROR;
            return 0;
        }
      }
    }

    char finalChar = w[w.length() - 1]; // Final character of the string

    if (finalChar == 'E' || finalChar == 'I') // Word 2 scenario
      tt = WORD2;
    else // Word 1 scenario
      tt = WORD1;
    return 0;
  }

  else { // If it is neither a word nor a period, then it is an error
    cout << "\nLexical error: " << w << " is not a valid token" << endl;
    tt = ERROR;
  }

  return 0; // Stop giving me compile warnings >:(
} //the end of scanner

//Start of the parser functions;

// ----- Four Utility Functions and Globals -----------------------------------

// ** Need syntaxerror1 and syntaxerror2 functions (each takes 2 args)
//    to display syntax error messages as specified by me.
tokentype saved_token;
string saved_lexeme;

// Type of error: When the lexical does not match expected token name
// Done By: Luis Zamora
void syntax_error1(tokentype expected, string saved_lexeme)
{
  cout << "\nSYNTAX ERROR: expected " << tokenName[expected] << " but found "
  << saved_lexeme << endl;

  exit (1); //halting
}

// Type of error: When unexpected lexical dound in RDP parser functions
// Done By: Luis Zamora
void syntax_error2(string saved_lexeme, string parserFunct)
{
  cout << "\nSYNTAX ERROR: unexpected " << saved_lexeme << " found in "
  << parserFunct << endl;

  exit (1); //halting
}

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Purpose: Grabs the next token and update it in two variables
// Done by: Raymond Quach
tokentype next_token() {
  //Calls the scanner to retrieve token and lexeme
  scanner(saved_token, saved_lexeme);

  //Returns the token value to global variable
  return saved_token;
}

// Purpose: Compares the saved token to the expected type and see if it matches
// Done by: Raymond Quach
bool match(tokentype expected) {
  //Sends syntax error if token does not match expected
  if (saved_token != expected)
    syntax_error1(expected, saved_lexeme);

  //Sends message of matched token and return true
  else {
    cout << "Matched " << tokenName[expected] << endl;
    return true;
  }
}

// ----- RDP functions - one per non-term -------------------

// ** Make each non-terminal into a function here
// ** Be sure to put the corresponding grammar rule above each function
// ** Be sure to put the name of the programmer above each function

// Done by: Raymond Quach (for all of below) 

// Grammar: <noun> ::= WORD1 | PRONOUN
void noun() {
  cout << "Processing <noun>" << endl;
  switch(next_token()) {
    case WORD1:
      match(WORD1); //This already called next_token() right before so it's fine
      break;
    case PRONOUN:
      match(PRONOUN);
      break;
    default: //Sends syntax error if unexpexted lexical found in parsing
      syntax_error2(saved_lexeme, "noun");
  }
}

// Grammar: <verb> ::= WORD2
void verb() {
  cout << "Processing <verb>" << endl;
  switch(next_token()) {
    case WORD2:
      match(WORD2);
      break;
    default: //Sends syntax error if unexpexted lexical found in parsing
      syntax_error2(saved_lexeme, "verb");
  }
}

// Grammar: <tense> ::= VERB | VERB NEGATIVE | VERB PAST | VERB PAST NEGATIVE
void verb_tense() {
  cout << "Processing <tense>" << endl;
  switch(next_token()) {
    case VERB:
      match(VERB);
      break;
    case VERBNEG:
      match(VERBNEG);
      break;
    case VERBPAST:
      match(VERBPAST);
      break;
    case VERBPASTNEG:
      match(VERBPASTNEG);
      break;
    default: //Sends syntax error if unexpexted lexical found in parsing
      syntax_error2(saved_lexeme, "tense");
  }
}

// Grammar: <be> ::= IS | WAS
void be() {
  cout << "Processing <be>" << endl;
  switch(next_token()) {
    case IS:
      match(IS);
      break;
    case WAS:
      match(WAS);
      break;
    default: //Sends syntax error if unexpexted lexical found in parsing
      syntax_error2(saved_lexeme, "be");
  }
}

// Grammar: <afterObject> ::= verb tense PERIOD | noun dest verb tense PERIOD
void after_object() {
  cout << "Processing <afterObject>" << endl;
  switch(next_token()) {
    case WORD2:
      verb();
      verb_tense();
      match(PERIOD);
      break;
    case WORD1: case PRONOUN:
      noun();
      match(DESTINATION);
      verb();
      verb_tense();
      match(PERIOD);
      break;
    case OBJECT:
      match(OBJECT);
      after_object();
    default: //Sends syntax error if unexpexted lexical found in parsing
      syntax_error2(saved_lexeme, "afterObject");
  }
}

// Grammar: <afterNoun> ::= be PERIOD | DESTINATION verb tense PERIOD | object afterObject
void after_noun() {
  cout << "Processing <afterNoun>" << endl;
  switch(next_token()) {
    case IS: case WAS:
      be();
      match(PERIOD);
      break;
    case DESTINATION:
      match(DESTINATION);
      verb();
      verb_tense();
      match(PERIOD);
      break;
    case OBJECT:
      match(OBJECT);
      after_object();
    default: //Sends syntax error if unexpexted lexical found in parsing
      syntax_error2(saved_lexeme, "afterNoun");
  }
}

// Grammar: <afterSubject> ::= verb tense PERIOD | noun afterNoun
void after_subject() {
  cout << "Processing <afterSubject>" << endl;
  switch(next_token()) {
    case WORD2:
      verb();
      verb_tense();
      match(PERIOD);
      break;
    case WORD1: PRONOUN:
      noun();
      after_noun();
      break;
    default: //Sends syntax error if unexpexted lexical found in parsing
      syntax_error2(saved_lexeme, "afterSubject");
  }
}

// Geammar: <s> ::= [CONNECTOR] noun SUBJECT after_subject
void s() {
  cout << "Processing <s>" << endl;

  //If the next token is a connector, match it
  /*Need to find a way to prevent it from getting called immediately on
  processing story() global bool variable maybe(?), delete this comment block after*/
  if (next_token() == CONNECTOR) 
    match(CONNECTOR);
  
  noun();
  next_token(); //Pretty sure this is needed everywhere before a match() call
  match(SUBJECT);
  after_subject();
}

// Grammar: <s> { <s> } (Loops for as long as possible)
void story() {
  cout << "Processing <story>\n\n";

  s(); //Proccesses <s>
  bool first = false;

  //Loops s() next token is either a connector, word1, or pronoun
  while (1) {
    if (next_token() == CONNECTOR ||
      next_token() == WORD1 || next_token() == PRONOUN)
         s();
      else
         break;
  }

  cout << "\nSuccessfully parsed <story>." << endl;
}

string filename;

//----------- Driver ---------------------------

// The new test driver to start the parser
// Done by: Raymond Quach
int main()
{
  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.c_str());

  //calls the <story> to start parsing
  story();

  //closes the input file
  fin.close();

}// end
//** require no other input files!
//** syntax error EC requires producing errors.txt of error messages
//** tracing On/Off EC requires sending a flag to trace message output functions
