#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <dirent.h>

#include "asm_function.hh"
#include "secded_for_text.hh"
#include "filter.hh"
#include "fault_injector.hh"
#include "trainer.hh"

#include "program.hh"

void print_help_and_exit()
{
  std::cerr << "Incorrect arguments: " << std::endl
            << "\t-t <dirname>\tfor training where every file in directory will be used for training" << std::endl
            << "\t            \t(the directory has to be non-empty)" << std::endl
            << "\t-c <filename> -t <type>\tfor testing the corrector" << std::endl
            << "\t             \t(the file has to exist)" << std::endl
            << "\t             \t(valid types are: random, freq)" << std::endl;
  exit(-1);
}

inline bool file_exists(const char * path)
{
  std::ifstream infile(path);
  return infile.good();
}

inline bool dir_exists(const char * path)
{
  struct stat sb;
  return stat(path, &sb) == 0 && S_ISDIR(sb.st_mode);
}

char* get_cmd_option(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

std::vector<std::string> listdir(const char *path) {
    struct dirent *entry;
    DIR *dp;

    std::vector<std::string> filenames;

    dp = opendir(path);
    if (dp == NULL) {
        print_help_and_exit();
    }

    while ((entry = readdir(dp)))
    {
      if(entry->d_name[0] == '.') continue; //skip hidden files
      std::string filename = std::string(path) + std::string("/") + entry->d_name;
      filenames.push_back(filename);
    }
    closedir(dp);
    return filenames;
}

bool check_cmd_option_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}


int main(int argc, char *argv[])
{
  if(check_cmd_option_exists(argv, argv+argc, "-t"))
  {
      char* directory = get_cmd_option(argv, argv+argc, "-t");
      if(!directory || !dir_exists(directory)) print_help_and_exit();
      std::vector<std::string> filenames = listdir(directory);
      trainer::train(&filenames);
  }
  else if(check_cmd_option_exists(argv, argv+argc, "-c") && check_cmd_option_exists(argv, argv+argc, "-f"))
  {
    char* filename = get_cmd_option(argv, argv+argc, "-c");
    if(!filename || !file_exists(filename)) print_help_and_exit();

    char* filter_name = get_cmd_option(argv, argv+argc, "-f");
    if(!filter_name || !filter::valid_filter_type(filter_name)) print_help_and_exit();
    filter::filter_type ft = filter::get_filter_type(filter_name);

    Program program(SECDED_for_text::generate_secded_for_text(filename));
    // SECDED_for_text::print_text_and_secded(&functions);

    fault_injector::inject_faults(1, 2, &program);
    // uint64_t faulty_instructions =
    SECDED_for_text::check_secded(&program, ft);
    // std::cerr << "There are " << faulty_instructions << " faulty instructions." << std::endl;
    std::cout << "Finished" << std::endl;
  }
  else
  {
    print_help_and_exit();
  }

  return 0;
}