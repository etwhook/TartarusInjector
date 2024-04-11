#pragma once
#include<iostream>
#define okay(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__);
#define info(msg, ...) printf("[*] " msg "\n", ##__VA_ARGS__);
#define warn(msg, ...) printf("[!] " msg "\n", ##__VA_ARGS__);
#define error(msg, ...) printf("[-] " msg "\n", ##__VA_ARGS__);

#define wokay(msg, ...) wprintf("[+] " msg "\n", ##__VA_ARGS__);
#define winfo(msg, ...) wprintf("[*] " msg "\n", ##__VA_ARGS__);
#define wwarn(msg, ...) wprintf("[!] " msg "\n", ##__VA_ARGS__);
#define werror(msg, ...) wprintf("[-] " msg "\n", ##__VA_ARGS__);