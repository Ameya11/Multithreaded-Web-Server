#include "http_server.hh"

#include <vector>

#include <sys/stat.h>
#include<ctime>
#include <fstream>
#include <iostream>
#include <sstream>


string root = "/Users/ameya/Downloads/22M2118/";

vector<string> split(const string &s, char delim) {
  vector<string> elems;

  stringstream ss(s);
  string item;

  while (getline(ss, item, delim)) {
    if (!item.empty())
      elems.push_back(item);
  }

  return elems;
}

HTTP_Request::HTTP_Request(string request) {
  vector<string> lines = split(request, '\n');
  vector<string> first_line = split(lines[0], ' ');

  this->HTTP_version = "1.0"; // We'll be using 1.0 irrespective of the request
  this->method = first_line[0];
  this->url = first_line[1];
  /*
   TODO : extract the request method and URL from first_line here
  */

  if (this->method != "GET") {
    cerr << "Method '" << this->method << "' not supported" << endl;
    exit(1);
  }
}

HTTP_Response *handle_request(string req) {

  cout << "\n--------------------------------\n";
  cout << "*--Request Accepted--*";
  cout << "\n--------------------------------\n";
  HTTP_Request *request = new HTTP_Request(req);

  HTTP_Response *response = new HTTP_Response();

  string url = string("html_files") + request->url;

  response->HTTP_version = "1.0";
  // cout << "i m here\n";
  struct stat sb;
  if (stat(url.c_str(), &sb) == 0) // requested path exists
  {
    response->status_code = "200";
    response->status_text = "OK";
    response->content_type = "text/html";

    string body;
    string path = root + url;
    if (S_ISDIR(sb.st_mode)) {

      path = path + "/index.html";
      /*
      In this case, requested path is a directory.
      TODO : find the index.html file in that directory (modify the url
      accordingly)
      */
    }

    string text;
    ifstream MyReadFile(path);
    if (MyReadFile){
      ostringstream myfile;
      myfile << MyReadFile.rdbuf();
      response->body = response->body + myfile.str() ;

    }
    //
    // while (getline (MyReadFile, text)) {
    //
    //
    //
    //   response->body = response->body + text ;
    // }
    // cout << response->body;
    /*
    TODO : open the file and read its contents
    */

      response->content_length = to_string(response->body.length());
      //cout << response->content_length<<endl;
      /*
    TODO : set the remaining fields of response appropriately
    */
  }

  else {
    response->status_code = "404" ;
    response->status_text = "Not Found";
    //response->content_type = "NA";
    response->content_length = "0";
    /*
    TODO : set the remaining fields of response appropriately
    */
  }

  delete request;

  return response;
}

string HTTP_Response::get_string() {
  // time_t timetoday;
  // time (&timetoday);
  /*
  TODO : implement this function
  */
  string aline;
  aline = "HTTP/" + this->HTTP_version + " " + this->status_code + " " + this->status_text + "\n";
  char data_str[100];
  time_t now = time(0);
  struct tm tm = *gmtime(&now);
  strftime(data_str, sizeof data_str, "%a, %d %b %Y %H:%M:%S ", &tm);
  //printf("Time is: [%s]\n", data_str);
  //cout << asctime(localtime(&timetoday));
  if (this->status_code != "404") {
    aline = aline + "Date: " + data_str +"GMT" + "\n";
    aline = aline + "Content-Type: " + this->content_type + "\n";
  }
  aline = aline + "Content-Length: " + this->content_length + "\n\n";
  aline = aline + this->body;

  // cout << "\n++++++++++++++++\n";
  // cout<<aline << endl;
  // cout << "\n+++++++++++++++++\n";


 return aline;
}
