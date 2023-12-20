#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;

// To do - Complete this function
StreamService::StreamService()

{
  cUser_ = nullptr;
}

// To do - Complete this function
StreamService::~StreamService()
{
  for (size_t i = 0; i < content_.size(); i++) 
  {
    delete content_[i];
  }

  for (size_t i = 0; i < users_.size(); i++) 
  {
    delete users_[i];
  }

}

// Complete - Do not alter
void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

// To do - Complete this function

void StreamService::userLogin(const std::string& uname)
{ 
  bool valid = true; 
  if (getUserIndexByName(uname) == -1) 
  {
    valid = false; 
    throw std::invalid_argument("This is an invalid username");
  }

  if (cUser_ != nullptr) 
  {
    valid = false; 
    throw std::runtime_error("A user is already logged in");
  }
  
  if(valid)
  {
    cUser_ = users_[getUserIndexByName(uname)];
  }
 
}


// To do - Complete this function
void StreamService::userLogout()
{
  cUser_ = nullptr;

}

std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    std::vector<CID_T> results;
    for(size_t i = 0; i < content_.size(); i++){
        // TO DO - modify this to also push back when the string 'partial'
        //  is contained in the name of the current content. Lookup the string
        //  class documentation to find an appropriate function that can be used
        //  to do this simply.
        if(partial == "*" || content_[i]->name().find(partial) != std::string::npos){
            results.push_back(i);
        }        
    }
    return results;
}


// Complete - Do not alter
std::vector<CID_T> StreamService::getUserHistory() const
{
    throwIfNoCurrentUser();
    return cUser_->history;
}

// To do - Complete this function
void StreamService::watch(CID_T contentID)
{
  throwIfNoCurrentUser();
  if (contentID>content_.size()) 
  {
    throw ReviewRangeError("Invalid contentID");
  }
  int crating = content_[contentID] -> rating();
  int urating = cUser_->ratingLimit;
  if (urating < crating) 
  {
    throw RatingLimitError("you can not watch due to rating limits");
  }
  int index = getUserIndexByName(cUser_ -> uname);
  users_[index]-> addToHistory(contentID);
  content_[index]-> addViewer(cUser_ -> uname);
}

// To do - Complete this function

void StreamService::reviewShow(CID_T contentID, int numStars)
{
  bool logged = false;
  if(cUser_== nullptr)
  {
    logged = true;
  }
  if (true) 
  {
    throwIfNoCurrentUser();
  }
  if (contentID > content_.size()) 
  {
    throw ReviewRangeError("invalid ID");
  }
  if (numStars > 5)
  {
    throw ReviewRangeError("stars error");
  }
  if (numStars < 0) 
  {
    throw ReviewRangeError("stars error");
  }
  content_[contentID]-> review(numStars);  
}

// To do - Complete this function
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{

    if(cUser_ == NULL)
    {
      throw std::runtime_error("Nobody is logged in");
    }

    if(!isValidContentID(contentID))
    {
      throw std::range_error("not valid content id");
    }

    std::vector<User*> watched; 
    std::vector<int> counter;

    for(int i = 0; i<content_.size(); i++)
    {
      counter.push_back(-1);
    }

    int max = -1;
    CID_T maxi = -1;
    for(int i = 0; i < users_.size(); i++)
    {
      if(users_[i]->haveWatched(contentID))
      {
        watched.push_back(users_[i]);
      }
    }

    for(int x = 0; x< watched.size(); x++)
    {
      if (watched[x] != cUser_)
      {
        for(int j = 0; j<watched[x]->history.size(); j++)
        {
          for(int k = 0; k< content_.size(); k++)
          {
            if(content_[k] -> id() == watched[x]->history[j] && !cUser_->haveWatched(content_[k] -> id()) && contentID != content_[k]->id() /*content_[k]->rating()<=cUser_->ratingLimit*/)
            {
              counter[k]++;
            }
          }
        }
      }
    }

    for (int i = 0; i < counter.size(); i++)
    {
      if (counter[i] > max)
      {
        maxi = content_[i]->id();
        max = counter[i];
      }
    }
    return maxi;

}

// To do - Complete this function
void StreamService::displayContentInfo(CID_T contentID) const
{
    // Do not alter this
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }
}
    

// Complete - Do not alter
bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int)content_.size());
}

// Complete - Do not alter
void StreamService::throwIfNoCurrentUser() const
{
  if(cUser_ == nullptr){
      throw UserNotLoggedInError("No user logged is currently logged in");
  }
}

// Complete - Do not alter
int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
