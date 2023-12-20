// Complete me
#include "customparser.h"
#include <iostream>
#include <sstream>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";

string getNextLine(istream& stream) 
{
    string line;
    while (getline(stream >> ws, line)) 
    {
      if (!line.empty()) 
      {
            break;
        }
    }
    return line;
}

int safeReadIntContent(istream& is) 
{
  int n;
  if (!(is >> n)) 
  { 
    throw ParserError(error_msg_2);
  }
  return n;
}

int safeReadIntUser(istream& is) 
{
  int n;
  if (!(is >> n)) 
  { 
    throw ParserError(error_msg_3);
  }
  return n;
}

// To Do - Complete this function
void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    // Erase any old contents
    content.clear();
    users.clear();

    // TO DO - Add your code below.
    if (is.fail()) 
    {
      throw ParserError(error_msg_2); 
    }

    int n = 0;

    if (!(is >> n)) 
    { 
      throw ParserError(error_msg_1);
    }

    if (n < 0) 
    { 
      throw ParserError(error_msg_1);
    }

    for (int i = 0; i < n; i ++) 
    {
      int id;
      int type;
      is >> id;
      is >> type;
      string temp;

      string name = getNextLine(is);

      int num_reviews = safeReadIntContent(is);
      int total_starts = safeReadIntContent(is);
      int rating = safeReadIntContent(is);
      // is >> num_reviews;
      // is >> total_starts;
      // is >> rating;

      if (type == 0) 
      {
        Content* new_content = new Movie(id, name, num_reviews, total_starts, rating);
        string usernames;
        getline(is, usernames);
        getline(is, usernames);
        istringstream iss(usernames);
        string n;
        while (iss >> n) 
        {
            new_content->addViewer(n);
        }
      content.push_back(new_content);
      } 
      else 
      {
        int num_episodes;
        is >> num_episodes;
        Content* new_content = new Series(id, name, num_reviews, total_starts, rating, num_episodes);
        string usernames;
        getline(is, usernames);
        getline(is, usernames);
        istringstream iss(usernames);
        string n;
        while (iss >> n) 
        {
            new_content->addViewer(n);
        }
        content.push_back(new_content);
      }
    }

    while (!is.eof()) 
    {
      string username;
      is >> username;

      if (username.length() == 0) 
      {
        continue;
      }

      int rating_limit = safeReadIntUser(is);
      string ids = "";
      getline(is, ids);
      getline(is, ids);
      istringstream idstream(ids);

      User* new_user = new User(username, rating_limit);
      int n;
      while (idstream >> n) 
      {
        new_user->addToHistory(n);
      }
      users.push_back(new_user);
    }
}
