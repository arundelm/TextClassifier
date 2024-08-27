#include <iostream>
#include "csvstream.hpp"
#include <set>
#include <cmath>

using namespace std;
// 124,331,703,730
class Classifier
{
public:
  Classifier(csvstream &train, bool debug, csvstream &test) : debug(debug)
  {
    read_data(train);
    analyze_data();
    read_testing_data(test);
    print_training_data();
  }

  double log_prior(const string &label)
  {
    double total = total_posts;
    double intermed = labels.at(label) / total;
    return log(intermed);
  }

  double log_likelihood(const string &label, const string &word)
  {
    double total = total_posts;
    double intermed;
    if (word_per_label.find({label, word}) != word_per_label.end())
    {
      intermed = word_per_label.at({label, word}) / labels.at(label);
    }
    else if (words.find(word) != words.end())
    {
      intermed = words.at(word) / total;
    }
    else
    {
      intermed = 1 / total;
    }
    return log(intermed);
  }

  void print_training_data()
  {
    print_debug();
    print_test();
  }

  void print_test()
  {
    cout << endl;
    cout << "test data:" << endl;

    for (int i = 0; i < test_data.size(); ++i)
    {
      pair<double, string> analysis = analyze_test(test_data[i]);
      if (analysis.second == test_data[i].first)
      {
        correct++;
      }
      cout << "  correct = " << test_data[i].first << ", predicted = " << analysis.second
           << ", log-probability score = " << analysis.first << endl;
      cout << "  content = " << test_content[i] << endl
           << endl;
    }

    cout << "performance: " << correct << " / " 
    << total_tests << " posts predicted correctly" << endl;
  }

  void print_debug()
  {
    if (debug)
    {
      cout << "training data:" << endl;
      for (int i = 0; i < data_print.size(); ++i)
      {
        cout << "  label = " << data_print[i].first << ", content = "
             << data_print[i].second << endl;
      }
      cout << "trained on " << total_posts << " examples" << endl;

      cout << "vocabulary size = " << unique_words_int << endl
           << endl;

      cout << "classes:" << endl;
      for (auto itr : labels)
      {
        cout << "  " << itr.first << ", "
             << labels[itr.first] << " examples, log-prior = "
             << log_prior(itr.first) << endl;
      }

      cout << "classifier parameters:" << endl;
      for (auto iter : word_per_label)
      {
        cout << "  " << iter.first.first << ":"
             << iter.first.second << ", count = "
             << iter.second << ", log-likelihood = "
             << log_likelihood(iter.first.first, iter.first.second) << endl;
      }
    }
    else
    {
      cout << "trained on " << total_posts << " examples" << endl;
    }
  }

  pair<double, string> analyze_test(pair<string, set<string>> &content)
  {
    string label;
    double intermediate = -INFINITY;
    double log;
    for (auto iter : labels)
    {
      log = log_prior(iter.first);
      for (auto itr : content.second)
      {
        log += log_likelihood(iter.first, itr);
      }
      if (log > intermediate)
      {
        intermediate = log;
        label = iter.first;
      }
    }
    return {intermediate, label};
  }

private:
  vector<pair<string, set<string>>> data;
  int total_posts = 0;
  int unique_words_int = 0;
  map<string, double> words;
  map<string, double> labels;
  map<pair<string, string>, double> word_per_label;
  vector<pair<string, string>> data_print;
  vector<pair<string, set<string>>> test_data;
  vector<string> test_content;
  int total_tests = 0;
  int correct = 0;
  bool debug;

  void read_testing_data(csvstream &test)
  {
    map<string, string> row;
    string second;
    while (test >> row)
    {
      for (auto &col : row)
      {
        pair<string, string> p;
        const string &col_name = col.first;
        string &content = col.second;
        if (col_name == "tag")
        {
          p.first = content;
          set<string> sett = remove_duplicates(unique_words(second));
          test_data.push_back({content, sett});
          test_content.push_back(second);
          total_tests++;
        }
        if (col_name == "content")
        {
          second = content;
        }
      }
    }
  }

  void read_data(csvstream &input)
  {
    map<string, string> row;
    string second;
    while (input >> row)
    {
      for (auto &col : row)
      {
        pair<string, string> p;
        const string &col_name = col.first;
        string &content = col.second;
        if (col_name == "tag")
        {
          p.first = content;
          data.push_back({content, remove_duplicates(unique_words(second))});
          data_print.push_back({content, second});
          ++total_posts;
        }
        if (col_name == "content")
        {
          second = content;
        }
      }
    }
  }

  void analyze_data()
  {
    for (auto iter : data)
    {
      check_label(iter.first);
      for (auto itr : iter.second)
      {
        check_word(itr);
        check_word_label({iter.first, itr});
      }
    }
  }

  set<string> remove_duplicates(set<string> content)
  {
    set<string> without;
    for (set<string>::iterator it = content.begin(); it != content.end(); ++it)
    {
      if (without.find(*it) == without.end())
      {
        without.insert(*it);
      }
    }
    return without;
  }

  set<string> unique_words(string &str)
  {
    istringstream source(str);
    set<string> words;
    string word;
    while (source >> word)
    {
      words.insert(word);
    }
    return words;
  }

  void check_word(string &s)
  {
    if (words.find(s) == words.end())
    {
      unique_words_int++;
      words.insert({s, 1});
    }
    else
    {
      words[s]++;
    }
  }

  void check_label(string &s)
  {
    if (labels.find(s) == labels.end())
    {
      labels.insert({s, 1});
    }
    else
    {
      labels[s]++;
    }
  }

  void check_word_label(pair<string, string> p)
  {
    if (word_per_label.find(p) == word_per_label.end())
    {
      word_per_label.insert({p, 1});
    }
    else
    {
      word_per_label[p]++;
    }
  }
  void print_set_data()
  {
    cout << "training data:" << endl;
    for (auto iter : data)
    {
      cout << " label = " << iter.first << ", content: ";
      for (auto itr : iter.second)
      {
        cout << itr << " ";
      }
      cout << "\n";
    }
    cout << total_posts << endl;
  }
};

int main(int argc, char *argv[])
{
  cout.precision(3);
  bool debugg = false;
  if (argc < 3 || argc > 4)
  {
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    return -1;
  }
  else if (argc == 4)
  {
    string debug = argv[3];
    if (debug != "--debug")
    {
      cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
      return -1;
    }
    else
    {
      debugg = true;
    }
  }
  csvstream train(argv[1]);
  csvstream test(argv[2]);

  Classifier trainer(train, debugg, test);
}
