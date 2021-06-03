#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <thread>
#include <curl/curl.h>
#include <stdio.h>
#include <string>
using namespace std;

size_t callbackfunction(void *ptr, size_t size, size_t nmemb, void* userdata)
{
    FILE* stream = (FILE*)userdata;
    if (!stream)
    {
        printf("!!! No stream\n");
        return 0;
    }

    size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
    return written;
}

bool download_jpeg(char* url,int i)
{
    char c = static_cast<char>(i);
    FILE* fp = fopen(c+"out.jpg", "wb");
    if (!fp)
    {
        printf("!!! Failed to create file on the disk\n");
        return false;
    }

    CURL* curlCtx = curl_easy_init();
    curl_easy_setopt(curlCtx, CURLOPT_URL, url);
    curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, callbackfunction);
    curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION, 1);

    CURLcode rc = curl_easy_perform(curlCtx);
    if (rc)
    {
        printf("!!! Failed to download: %s\n", url);
        return false;
    }

    long res_code = 0;
    curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &res_code);
    if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
    {
        printf("!!! Response code: %d\n", res_code);
        return false;
    }

    curl_easy_cleanup(curlCtx);

    fclose(fp);

    return true;
}

void print(int n, const std::string &str)  {
  std::string msg = std::to_string(n) + " : " + str;
  std::cout << msg  << std::endl;
}

 
int main() {
char image1[100]  = "https://upload.wikimedia.org/wikipedia/commons/4/41/Sunflower_from_Silesia2.jpg";
char image2[100]  = "https://brooklynintergroup.org/brooklyn/wp-content/uploads/2021/01/flower-729512__340-1.jpg";
char image3[100]  = "https://upload.wikimedia.org/wikipedia/commons/4/41/Sunflower_from_Silesia2.jpg";

  std::vector<std::thread> threads;

 threads.push_back(std::thread(download_jpeg, image1,1));
threads.push_back(std::thread(download_jpeg, image2,2));
threads.push_back(std::thread(download_jpeg, image3,3));
 
  for (auto &th : threads) {
    th.join();
  }
  return 0;
}

