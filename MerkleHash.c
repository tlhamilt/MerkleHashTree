#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

int main(int argc, char** argv) {
        if(argc < 2) {
                printf("%s\n", "ERROR:  no valid files detected");
                return 0;
        }
        int numFiles = 0;
        int filesLeft;
        char hashes[argc - 1][17];
        for(int x = 0; x < argc - 1; x++) {
                for(int y = 0; y < 17; y++) {
                        hashes[x][y] = '\0';
                }
        }
        for(int i = 1; i < argc; i++) {
                FILE* fp = fopen(argv[i], "r");
                if(fp != NULL) {
                        const char* data = NULL;
                        size_t size = 0;
                        getdelim(&data, &size, '\0', fp);
                        MD5(data, strlen(data), hashes[i - 1]);
                        free(data);
                        numFiles++;
                }
        }
        if(numFiles == 0) {
                printf("%s\n", "ERROR:  no valid files detected");
                return 0;
        }
        filesLeft = numFiles;
        while(filesLeft > 1) {
                filesLeft = 0;
                int i = 0;
                while(i < numFiles) {
                        if(i + 1 < numFiles) {
                                char comb[33];
                                for(int x = 0; x < 33; x++) {
                                        comb[x] = '\0';
                                }
                                strncpy(comb, hashes[i], strlen(hashes[i]));
                                strncat(comb, hashes[i + 1], strlen(hashes[i + 1]));
                                for(int x = 0; x < 17; x++) {
                                        hashes[filesLeft][x] = '\0';
                                }
                                MD5(comb, strlen(hashes[i]) + strlen(hashes[i + 1]), hashes[filesLeft]);
                                i = i + 2;
                        }
                        else {
                                for(int x = 0; x < 17; x++) {
                                        hashes[filesLeft][x] = '\0';
                                }
                                strncpy(hashes[filesLeft], hashes[i], strlen(hashes[i]));
                                i++;
                        }
                        filesLeft++;
                }
                numFiles = filesLeft;
	}
        printf("Top Hash: \"%s\"\n", hashes[0]);
        return 0;