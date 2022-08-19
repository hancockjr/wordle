#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <string.h>

#define DEBUG 0
using namespace std;

struct word_t
{
  string s;
  int score;
  bool fully_unique;

  void is_fully_unique();
};

static const int MAX_LENGTH = 5;
static const int LETTERS = 26;
static vector<word_t> dictionary;
static int counter[MAX_LENGTH][LETTERS];
static int max_score = 0;

static int load_dictionary()
{
  ifstream f("wordles.txt");
  if (!f.is_open())
  {
    cout << "unable to read file" << endl;
    return -1;
  }

  word_t word;
  while (getline(f, word.s))
  {
    dictionary.push_back(word);
  }
  //cout << "size:" << dictionary.size() << endl;

  return 0;
}

static int initialize()
{
  dictionary.clear();
  if (load_dictionary() != 0)
  {
    return -1;
  }
  return 0;
}

static int filter(char c)
{
#if DEBUG
  cout << __func__ << "(" << c << ")" << endl;
#endif // DEBUG
  auto it = dictionary.begin();
  while (it != dictionary.end())
  {
    if (it->s.find(c) != string::npos)
    {
#if DEBUG
      cout << __func__ << " remove:" << it->s << endl;
#endif // DEBUG
      it = dictionary.erase(it);
    }
    else
    {
      ++it;
    }
  }
  return 0;
}

static int filter_yellow(char c, int ii)
{
  c = tolower(c);
#if DEBUG
  cout << __func__ << "(" << c << "," << ii << ")" << endl;
#endif // DEBUG
  auto it = dictionary.begin();
  while (it != dictionary.end())
  {
    if (it->s.find(c) == string::npos)
    {
#if DEBUG
      cout << __func__ << " remove:" << it->s << endl;
#endif // DEBUG
      it = dictionary.erase(it);
    }
    else if (it->s.at(ii) == c)
    {
#if DEBUG
      cout << __func__ << " remove:" << it->s << endl;
#endif // DEBUG
      it = dictionary.erase(it);
    }
    else
    {
      ++it;
    }
  }
  return 0;
}

static int filter_green(char c, int ii)
{
  c = tolower(c);
#if DEBUG
  cout << __func__ << "(" << c << "," << ii << ")" << endl;
#endif // DEBUG
  auto it = dictionary.begin();
  while (it != dictionary.end())
  {
    if (it->s.at(ii) != c)
    {
#if DEBUG
      cout << __func__ << " remove:" << it->s << endl;
#endif // DEBUG
      it = dictionary.erase(it);
    }
    else
    {
      ++it;
    }
  }
  return 0;
}

static int parse(char *arg)
{
#if DEBUG
  cout << "arg:" << arg << endl;
#endif // DEBUG

  int len = strlen(arg);
  if (len < MAX_LENGTH)
  {
    cout << arg << " is < " << MAX_LENGTH << endl;
    return -1;
  }
  int asterisks = 0;
  for (int ii = 0; ii < len; ii++)
  {
    if ('a' <= arg[ii] && arg[ii] <= 'z')
    {
      filter(arg[ii]);
    }
    else if ('A' <= arg[ii] && arg[ii] <= 'Z')
    {
      filter_yellow(arg[ii], ii - asterisks);
    }
    else if (arg[ii] == '*')
    {
      if (ii+1 >= len)
      {
	cout << "invalid arg:" << arg << endl;
	return -1;
      }
      ii++;
      filter_green(arg[ii], ii - ++asterisks);
    }
  }
  return 0;
}

static void score()
{
  max_score = 0;
  for (word_t& word : dictionary)
  {
    word.score = 0;
    if (!word.fully_unique)
    {
      continue;
    }
    for (int ii = 0; ii < MAX_LENGTH; ii++)
    {
      int jj = word.s[ii] - 'a';
      if (jj < 0 || jj >= LETTERS)
      {
	cout << __func__ << " invalid index:" << jj << endl;
	continue;
      }
      word.score += counter[ii][jj];
    }
    if (word.score > max_score)
    {
      max_score = word.score;
    }
  }
}

void word_t::is_fully_unique()
{
  bool encountered[LETTERS] = {};
  bool res = true;
  for (int ii = 0; ii < MAX_LENGTH; ii++)
  {
    int jj = s[ii] - 'a';
    if (jj < 0 || jj >= LETTERS)
    {
	cout << __func__ << " invalid index:" << jj << endl;
	continue;
    }
    if (encountered[jj])
    {
      res = false;
      break;
    }
    encountered[jj] = true;
  }
  fully_unique = res;
}

static void count()
{
  for (word_t& word : dictionary)
  {
    for (int ii = 0; ii < MAX_LENGTH; ii++)
    {
      int jj = word.s[ii] - 'a';
      if (jj < 0 || jj >= LETTERS)
      {
	cout << __func__ << " invalid index:" << jj << endl;
	continue;
      }
      counter[ii][jj]++;
    }
    word.is_fully_unique();
  }
  score();
}

static void dump()
{
  cout << "-----" << endl;
  for (word_t& word : dictionary)
  {
    cout << word.s;
    for (int ii = 0; ii < MAX_LENGTH; ii++)
    {
      int jj = word.s[ii] - 'a';
      if (jj < 0 || jj >= LETTERS)
      {
	cout << __func__ << " invalid index:" << jj << endl;
	continue;
      }
      cout << "\t" << counter[ii][jj];
    }
    cout << "\t" << word.score;
    if (word.score == max_score)
    {
      cout << "*";
    }
    
    cout << endl;
  }
  cout << "----- " << dictionary.size() << endl;
}

static void usage(char *arg0)
{
  cout << "USAGE: " << arg0 << " [GUESS]...\n"
       << "  lowercase letter         GREY\n"
       << "  captialized letter       YELLOW\n"
       << "  preceding asterisk (*)   GREEN\n"
       << endl;
}

int main(int argc, char *argv[])
{
  if (initialize() != 0)
  {
    return -1;
  }

  for (int ii = 1; ii < argc; ii++)
  {
    if (!strcmp(argv[ii], "--help") || !strcmp(argv[ii], "-h"))
    {
      usage(argv[0]);
      return 0;
    }

    if (parse(argv[ii]) != 0)
    {
      return -1;
    }
  }

#if !DEBUG
  count();
  dump();
#endif // DEBUG

  dictionary.clear();

  return 0;
}
