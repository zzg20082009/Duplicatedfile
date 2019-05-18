#include <openssl/md5.h>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <cstdio>
#include <ftw.h>

using namespace std;
const int BUFSIZE = 4096;

map<string, string> md5map;
unsigned char result[33];

void md5sum(const string& filename, const int& filesize)
{
  ifstream is(filename);
  MD5_CTX context;
  int bytetoread = 0;        // readed means the byte to be read;
  int length = filesize;
  
  if (is) {
    char* buf = new char[BUFSIZE];

    MD5_Init(&context);

    bytetoread = length < BUFSIZE ? length : BUFSIZE;
    while (is.read(buf, bytetoread)) {
      MD5_Update(&context, buf, bytetoread);
      length -= bytetoread;
      if (length <= 0)
	break;
      bytetoread = (length < BUFSIZE)? length : BUFSIZE;
    }
    MD5_Final(result, &context);
    delete []buf;
  }
}

void print_md5_sum(unsigned char* md) {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
            printf("%02x",md[i]);
    }
}

void build_map_of_md5(const string& filename, const int& filesize)
{
  md5sum(filename, filesize);
  cout << "The file is: " << filename << "       ";
  print_md5_sum(result);
  cout << endl;

  string md5str = (char *) result;
  
  if (md5map.find(md5str) == md5map.end()) {
    md5map[md5str] = filename;
  } else {
    cout << "Remove the file: " << filename << endl;
    remove(filename.c_str());
  }
}

int walk_dir(const char* fpath, const struct stat* sb, int tflag, struct FTW* ftwbuf) {
  if (tflag == FTW_F) {
    //    cout << "the file is: " << fpath << endl;
    build_map_of_md5(fpath, sb->st_size);
  }
  return 0;
}

int main(int argc, char* argv[])
{
    // The first char* is the md5 of file, the second char* is the path of the file
  int flags = 0;
  
  for (int i = 1; i < argc; i++) {
    if (nftw((argc < 2) ? "." : argv[i], walk_dir, 20, flags) == -1) {
      perror("nftw");
      exit(2);
    }
  }

  // for (map<string, string>::const_iterator m5i = md5ofile.begin(); m5i != md5ofile.end(); m5i++) {
  //   cout << "The file is: " << m5i->second << "    The md5 is: " << m5i->first << endl;
  // }
    
}
