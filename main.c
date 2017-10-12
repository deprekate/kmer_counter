#include <stdio.h>
#include "uthash.h"

struct my_struct {
    char key[10];                    /* key */
    int count;
    char readname[10][255];
    UT_hash_handle hh;         /* makes this structure hashable */
};
struct my_struct *kmers = NULL;    /* important! initialize to NULL */
void add_kmer(char *key, int count, char *readname) {
    struct my_struct *s;

    s = malloc(sizeof(struct my_struct));
    strcpy(s->key, key);
    s->count = count;
    strcpy(s->readname[count-1], readname);
    HASH_ADD_STR( kmers, key, s );  /* id: name of key field */
}
void delete_kmer(struct my_struct *key) {
    HASH_DEL(kmers, key);  /* user: pointer to deletee */
    free(key);             /* optional; it's up to you! */
}

main(void) {
	struct my_struct *s;
	char header[255] = "";
	char seq[255] = "";
	FILE *file = fopen("in.fna","r"); 
	if(file != NULL) {
		char line[255];
		while(fgets( line, sizeof line, file) != NULL) {
			line[strcspn(line, "\n")] = 0;
			if(line[0] == '>'){
				int i;
				int len;
				len = strlen(seq)-5;
				for (i = 0; i < len; i++){	
					char kmer[5];
					memcpy(kmer, seq+i, 5 );
					kmer[5] = '\0';
					HASH_FIND_STR( kmers, kmer, s);
					if(s == NULL){
						add_kmer(kmer, 1, line);
					}else if(s->count >= 10){
						delete_kmer(s);
					}else{
						//add_kmer(kmer, s->count+1, line);
    						strcpy(s->readname[s->count], line);
						s->count++;
					}
				}
				strcpy(header, line);
			}else{
				strcpy(seq, line);
			}
		}
		fclose ( file );
		for(s=kmers; s != NULL; s=s->hh.next){
    		    printf("%s\t%i\t%s\n", s->key, s->count, s->readname[s->count-1]);
    		}
	}
	printf("\n");
}
