#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Song {
	char name[50];
	struct Song* next;
	struct Song* prev;
}Song;
struct Song* head;
FILE *fp,*fl;
void insertSong(struct Song** head,char *new_name,char *direction);
void printPlaylist(struct Song* head,char *direction,FILE *fl);
struct Song* reversePlaylist(struct Song* head);
void removeSong(struct Song** head,char *name,FILE *fl,char playingSong[50]);
void moveSong(struct Song** head,char *direction, char *song1, char *song2);
void playSong(char *direction,struct Song* head,char playingSong[50],FILE *fl);
int hasNext(char *name,struct Song** head);
int hasPrev(char *name,struct Song** head);

int main() {
	head = NULL;
	char playingSong[50] = "";
	char process[100];
	char direction[100];
	char song1[100];
	char song2[100];
	if((fp = fopen("input.txt","r")) == NULL) printf("Something go wrong!");
	fscanf(fp,"%s",process);
	fl = fopen("output.txt","w+");
	while(!feof(fp)) {
		if(strcmp(process,"ReversePlaylist") == 0) {
			struct Song* temp = reversePlaylist(head);
			head = temp;
		}
		else if(strcmp(process,"PrintPlaylist") == 0) {
			fscanf(fp,"%s",direction);	
			printPlaylist(head,direction,fl);
		}
		else if(strcmp(process,"PlaySong") == 0) {
			fscanf(fp,"%s",direction);
			playSong(direction,head,playingSong,fl);
		}
		else if(strcmp(process,"InsertSong") == 0) {
			fscanf(fp,"%s\t%s",direction,song1);
			insertSong(&head,song1,direction);	
		}
		else if(strcmp(process,"MoveSong") == 0) {
			fscanf(fp,"%s\t%s\t%s",direction,song1,song2);
			moveSong(&head,direction,song1,song2);
		}
		else if(strcmp(process,"RemoveSong") == 0) {
			fscanf(fp,"%s",song1);
			removeSong(&head,song1,fl,playingSong);
			
		}
		fscanf(fp,"%s",process);
	}
	fclose(fp);
	fclose(fl);
}
void insertSong(struct Song** head,char *new_name,char *direction) 
{
	if(strcmp(direction,"H") == 0) {
		struct Song* temp = (Song*)malloc(sizeof(struct Song));
		strcpy(temp->name,new_name);
		temp->next = (*head);
		temp->prev = NULL;
		if((*head) != NULL) {
			(*head)->prev = temp;
		}
		(*head) = temp;
	}
	else if(strcmp(direction,"T") == 0) {
		struct Song* temp = (Song*)malloc(sizeof(struct Song));
		struct Song* last = (*head);
		strcpy(temp->name,new_name);
		temp->next = NULL;
		if((*head) == NULL) {
			temp->prev = NULL;
			(*head) = temp;
			return;
		}
		while(last->next != NULL) {
			last = last->next;
		}
		last->next = temp;
		temp->prev = last;
		return;
	}
}
void printPlaylist(struct Song* head,char *direction,FILE *fl) {
	if(head == NULL) {
		char message[50] = "No Songs To Print\n*****\n";
		fprintf(fl,"%s",message);
	}
	else if(strcmp(direction,"F") == 0) {
		while(head != NULL) {
			fprintf(fl,"%s\n",head->name);
			head = head->next;
		}
		char message[50] = "*****\n";
		fprintf(fl,"%s",message);
	}
	else if(strcmp(direction,"R") == 0) {
		struct Song* temp = head;
		struct Song* new_temp = reversePlaylist(temp);
		while(new_temp != NULL) {
			fprintf(fl,"%s\n",new_temp->name);
			new_temp = new_temp->next;
		}
		char message[50] = "*****\n";
		fprintf(fl,"%s",message);
	}
}

struct Song* reversePlaylist(struct Song* head)
{
    struct Song* temp = head;
    struct Song* new_temp = NULL;
    if(temp == NULL) return;
	while(temp != NULL) {
		insertSong(&new_temp,temp->name,"H");
		temp = temp->next;
	}
	return new_temp;
}
void removeSong(struct Song** head,char *name,FILE *fl,char playingSong[50]) {
	if(strcmp(name,playingSong) == 0)  {
		char message[50] = "Cannot Remove The Playing Song\n*****\n";
		fprintf(fl,"%s",message);
		return;
	}
	struct Song* temp = (*head);
	struct Song* new_temp = NULL;
	char new_name[50];
	while(temp != NULL) {
		if(strcmp(temp->name,name) != 0) {
			strcpy(new_name,temp->name);
			insertSong(&new_temp,temp->name,"T");
		}
		temp = temp->next;
	}
	(*head) = new_temp;
	
}
void moveSong(struct Song** head,char *direction, char *song1, char *song2) {
	struct Song* temp = (*head);
	struct Song* new_temp = NULL;
	char moveThisSong[50];
	char afterOrBeforeThis[50];
	char anotherSong[50];
	while(temp != NULL) {
		if(strcmp(temp->name,song1) == 0) {
			strcpy(moveThisSong,song1);
		}
		else if(strcmp(temp->name,song2) == 0) {
			strcpy(afterOrBeforeThis,song2);
		}
		temp = temp->next;
	}
	
	if(strcmp(direction,"A") == 0) {
		struct Song* temp2 = (*head);
		struct Song* new_temp = NULL;
		while(temp2 != NULL) {
			if(strcmp(temp2->name,moveThisSong) == 0) {
			} 
			else if(strcmp(temp2->name,afterOrBeforeThis) == 0) {
				insertSong(&new_temp,afterOrBeforeThis,"T");
				insertSong(&new_temp,moveThisSong,"T");
			}
			else {
				strcpy(anotherSong,temp2->name);
				insertSong(&new_temp,anotherSong,"T");
			}
			temp2 = temp2->next;
		}
		(*head) = new_temp;
	}
	else if(strcmp(direction,"B") == 0) {
		struct Song* temp2 = (*head);
		struct Song* new_temp = NULL;
		while(temp2 != NULL) {
			if(strcmp(temp2->name,moveThisSong) == 0) {
			} 
			else if(strcmp(temp2->name,afterOrBeforeThis) == 0) {
				insertSong(&new_temp,moveThisSong,"T");
				insertSong(&new_temp,afterOrBeforeThis,"T");	
			}
			else {
				strcpy(anotherSong,temp2->name);
				insertSong(&new_temp,temp2->name,"T");
			}
			temp2 = temp2->next;
		}
		(*head) = new_temp;
	}
}
void playSong(char *direction,struct Song* head,char playingSong[50],FILE *fl) {
	
	if(head == NULL) {
		char message[50] = "No Songs To Play\n*****\n";
		fprintf(fl,"%s",message);
		return;
	}
	char firstSongOnList[50];
	strcpy(firstSongOnList,head->name);
	char lastSongOnList[50];
	struct Song* temp = head;
	while(temp != NULL) {
		if(hasNext(temp->name,&temp) == 0) {
			strcpy(lastSongOnList,temp->name);
		}
		temp = temp->next;
	}
	if(strcmp(direction,"N") == 0) {
		if(strcmp(playingSong,"") == 0) {
			strcpy(playingSong,head->name);
			fprintf(fl,"Playing\t%s\n",playingSong);
			char message[50] = "*****\n";
			fprintf(fl,"%s",message);
			return;
		}
		else {
			int hasPrevious = hasPrev(playingSong,&head);
			int ishasNext = hasNext(playingSong,&head);
			while(head != NULL) {
				if(strcmp(head->name,playingSong) == 0) {
					if(hasNext(head->name,&head) == 1) {
						strcpy(playingSong,head->next->name);
						fprintf(fl,"Playing\t%s\n",playingSong);
						char message[50] = "*****\n";
						fprintf(fl,"%s",message);
						return;
					}
					else if((ishasNext == 0) && (hasPrevious == 0)) {
						fprintf(fl,"Playing\t%s\n",playingSong);
						char message[50] = "*****\n";
						fprintf(fl,"%s",message);
						return;
					}
					else {
						strcpy(playingSong,firstSongOnList);
						fprintf(fl,"Playing\t%s\n",playingSong);
						char message[50] = "*****\n";
						fprintf(fl,"%s",message);
						return;
					}
				}
				head = head->next;
			}
		}
		
	}
	else if(strcmp(direction,"P") == 0) {
		struct Song* temp2 = reversePlaylist(head);
		if(strcmp(playingSong,"") == 0) {
			strcpy(playingSong,lastSongOnList);
			fprintf(fl,"Playing\t%s\n",playingSong);
			char message[50] = "*****\n";
			fprintf(fl,"%s",message);
			return;
		}
		else {
			int hasPrevious = hasPrev(playingSong,&head);
			int ishasNext = hasNext(playingSong,&head);
			while(head != NULL) {
				if((ishasNext == 0) && (hasPrevious == 0)) {
					fprintf(fl,"Playing\t%s\n",playingSong);
					char message[50] = "*****\n";
					fprintf(fl,"%s",message);
					return;
				}
				else if(strcmp(playingSong,lastSongOnList) == 0) {
					struct Song* temp = reversePlaylist(head);
					strcpy(playingSong,temp->next->name);
					fprintf(fl,"Playing\t%s\n",playingSong);
					char message[50] = "*****\n";
					fprintf(fl,"%s",message);
					return;
				}
				else if(strcmp(head->name,playingSong) == 0) {
					if(hasPrevious == 1) {
						while(temp2 != NULL) {
							if(strcmp(temp2->name,playingSong) == 0) {
								strcpy(playingSong,temp2->next->name);
								fprintf(fl,"Playing\t%s\n",playingSong);
								char message[50] = "*****\n";
								fprintf(fl,"%s",message);
								return;
							}
							temp2 = temp2->next;
						}
					}
				
					else {
						strcpy(playingSong,lastSongOnList);
						fprintf(fl,"Playing\t%s\n",playingSong);
						char message[50] = "*****\n";
						fprintf(fl,"%s",message);
						return;
					}
				}
				head = head->next;
			}
		}
	}

}
int hasNext(char *name,struct Song** head) {
	struct Song* temp = (*head);
	while(temp != NULL) {
		if(strcmp(name,temp->name) == 0) {
			if(temp->next == NULL) {
				return 0;
			}
			else {
				return 1;
			}
		}
		temp = temp->next;
	}
	return 0;
}
int hasPrev(char *name,struct Song** head) {
	struct Song* temp = (*head);
	struct Song* new_temp = reversePlaylist(temp);
	while(new_temp != NULL) {
		if(strcmp(name,new_temp->name) == 0) {
			if(new_temp->next == NULL) {
				return 0;
			}
			else {
				return 1;
			}
		}
		new_temp = new_temp->next;
	}
	return 0;
}


