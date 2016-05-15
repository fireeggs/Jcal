/*
 * ============================================================================================
 * File Name : lists.c
 *
 * Name	: Seungkyu Kim
 * Created Date	: Sep.25.2015
 * Modified Date : Sep.28.2015
 *
 *
 * Copyright 2015 Seungkyu Kim all rights reserved
 * ============================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lists.h"

/*
 * Return a pointer to the struct calendar with name cal_name
 * or NULL if no calendar with this name exists in the cal_list
 */
Calendar *find_calendar(Calendar *cal_list, char *cal_name) {

	while(cal_list){
		if (strcmp(cal_list->name, cal_name) == 0){
			return cal_list;
		} else if (cal_list->next != NULL)
			cal_list = cal_list->next;
		else
			break;	
	}
	return NULL;
}

/* 
 * Return a pointer to the user with name username or NULL
 *   if no such user exists in user_list 
 */
User *find_user(User *user_list, char *username) {
	
	while(user_list){
		if (strcmp(user_list->name, username) == 0){
			return user_list;
		} else if (user_list->next != NULL) {
			user_list = user_list->next;
		} else
			break;	
	}
	return NULL;
}

/*
 * Return a pointer to the last user in user_list
 */
User *find_last_user(User *user_list) {
	
	while(user_list){
		if(user_list->next != NULL){
			user_list = user_list->next;
		} else 
			return user_list;
	}
	return NULL;
}

/*
 * Return a pointer to the last event in events_list
 * or NULL if no event in event_list
 */
Event *last_event(Event *event_list) {
	
	while(event_list){
		if(event_list->next != NULL) {
			event_list = event_list->next;
		}
		else
			return event_list;
	}
	return NULL;
}

/*
 * return a pointer to the last_sub in sub_list
 * or NULL if this user is already subscribed to this calendar
 */
Subscription *last_sub(Subscription *sub_list, char *cal_name){
	
	while(sub_list){
		if (sub_list->calendar != NULL){
			// this user is already subscribed to this calendar
			if(strcmp(sub_list->calendar->name, cal_name) == 0){
				return NULL;
			}
		}
		if (sub_list->next != NULL){
			sub_list = sub_list->next;
		} else 
			break;
	}
	return sub_list;

}

/* 
 * If a calendar with name cal_name does not exist, create a new
 * calendar by this name and insert it at the front of the calendar list.
 * Return values:
 *    0 if successful
 *   -1 if a calendar by cal_name already exists
 */
int add_calendar(Calendar **cal_list_ptr, char *cal_name) {
	
	Calendar *first_cal;
	// allocate memory for new calendar
	if ((first_cal = (Calendar *)malloc(sizeof(Calendar))) == NULL){
		perror("malloc\n");
		exit(1);
	}
	// allocate amemory for ne calendar's name
	if((first_cal->name = (char *)malloc(strlen(cal_name)+1)) == NULL){
		perror("malloc\n");
		exit(1);
	}
	strncpy(first_cal->name, cal_name,strlen(cal_name)+1);
	first_cal->events = NULL;
	// no calendar in cal_list
	if (*cal_list_ptr == NULL) {
		first_cal->next = NULL;
		*cal_list_ptr = first_cal;
		return 0;
	// 1 or more calendar in cal_list
	} else {
		// cal_name is in cal_list
		if (find_calendar(*cal_list_ptr, cal_name) != NULL){
			return -1;
		}
		// cal_name is not in cal_list so add cal_name into cal_list
		first_cal->next = *cal_list_ptr;
		*cal_list_ptr = first_cal;
	}
	return 0;
}

/* 
 * Print to stdout a list of all current calendar names (one per line)
 */
void list_calendars(Calendar *cal_list) {
	
	while(cal_list){
		printf("%s\n", cal_list->name);
		cal_list = cal_list->next;
	}
}

/* 
 * If a user with name username does not exist, create a new
 * user by this name and insert it at the end of the user list.
 * Return values:
 *    0 if successful
 *   -1 if username already exists
 */
int add_user(User **user_list_ptr, char *username) {
	
	User *first_user;
	// allocate momory for new user
	if ((first_user = (User *)malloc(sizeof(User))) == NULL) {
		perror("malloc\n");
		exit(1);
	}
	// allocate memory for new user's name
	if ((first_user->name = (char *)malloc(strlen(username)+1)) == NULL){
		perror("malloc\n");
		exit(1);
	}
	strcpy(first_user->name, username);
	first_user->subscriptions = NULL;
	// no user in user list
	if (*user_list_ptr == NULL){
		first_user->next = NULL;
		*user_list_ptr = first_user;
		return 0;
	// 1 or more user in list
	} else {
		// username is in user_list
		if (find_user(*user_list_ptr, username) != NULL){
			return -1;
		}
		// username is not in the user_list
		User *last_user = find_last_user(*user_list_ptr);
		last_user->next = first_user;
	}
	return 0;
}


/* 
 * Print to stdout a list of usernames one per line 
 */
void list_users(User *user_list) {
	
	while(user_list){
		printf("%s\n", user_list->name);
		user_list = user_list->next;
	}
}

/*
* free the momory in function subscribe
*/
void help_free_mem_sub(Calendar *found_cal, User *found_user) {
	free(found_cal);
	free(found_user);
}

/*
 * Subscribe the user username to calendar cal_name
 * Return:
 *    0 if successful
 *   -1 if no user exists by this name
 *   -2 if no calendar exists by this name
 *   -3 if this user is already subscribed to this calendar
 */
int subscribe(User *user_list, Calendar *cal_list, char *username, char *cal_name) {

	Calendar *found_cal;
	if ((found_cal = (Calendar *)malloc(sizeof(Calendar))) == NULL){
		perror("malloc\n");
		exit(1);
	}
	User *found_user;
	if((found_user = (User *)malloc(sizeof(User))) == NULL){
		perror("malloc\n");
		exit(1);
	}
	if((found_user = find_user(user_list, username)) == NULL) {
		return -1;
	} else if ((found_cal = find_calendar(cal_list, cal_name)) == NULL){
		return -2;
	}
	Subscription *new_sub;
	// allocate memory for new subscription
	if((new_sub = (Subscription *)malloc(sizeof(Subscription))) == NULL){
		perror("malloc\n");
		exit(1);
	}
	// user has no subscriptions at all
	if (found_user->subscriptions == NULL) {
		new_sub->calendar = found_cal;
		new_sub->next = NULL;
		found_user->subscriptions = new_sub;
		return 0;
	}
	// User is already subscribed to this calendar
	if ((new_sub = last_sub(found_user->subscriptions,cal_name)) == NULL){
		return -3;
	}
	// user has 1 or more subscriptions
	new_sub->next = NULL;
	found_user->subscriptions = new_sub;
	return 0;
}

/* 
 * Add an event with this name and date to the calender with name cal_name 
 *  Return:
 *   0 for success
 *  -1 for calendar does not exist by this name
 */
int add_event(Calendar *cal_list, char *cal_name, char *event_name, time_t time) {
	
	Calendar *cal_found;
	double diff_time1, diff_time2;
	if ((cal_found = (Calendar *)malloc(sizeof(Calendar))) == NULL){
		perror("malloc\n");
		exit(1);
	} 
	Event *new_event;
	// allocate memory for new event
	if ((new_event = (Event*)malloc(sizeof(Event))) == NULL) {
		perror("malloc\n");
		exit(1);
	}
	// allocate memory for the description of new event
	if ((new_event->description = (char*)malloc(strlen(event_name)+1)) == NULL) {
		perror("malloc\n");
		exit(1);
	}
	strcpy(new_event->description, event_name);
	new_event->time = time;
	new_event->next = NULL;
	cal_found = find_calendar(cal_list, cal_name);	// no calendar in cal_list with cal_name
	if (cal_found == NULL){
		return -1;
	} 
	// calendar is in cal_list with name cal_name
	else{
		// no event in calendar
		if(cal_found->events == NULL){
			cal_found->events = new_event;
		 }
		// at least 1 or more events in calendar
		else {
			// insert the event by order of time.
			while(cal_found->events != NULL){
				diff_time1 = difftime(cal_found->events->time, time);
				// new_cal should be inserted between two events
				if (cal_found->events->next != NULL){
					if(diff_time1 < 0){
						diff_time2 = difftime(cal_found->events->next->time, time);
						// found the place where new event inserted
						if (diff_time2 >= 0) {
							new_event->time = time;
							new_event->next = cal_found->events->next;
							cal_found->events->next = new_event;
							break;
						}
					// new_event shoul be inserted at the beginning of the list
					} if(diff_time1 >= 0){
						new_event->next = cal_found->events;
						cal_found->events = new_event;
						break;
					}
				}
				// insert new_event at the last in linked list
				else if (diff_time1 <= 0){
					cal_found->events->next = new_event;
					break;
				}
				cal_found->events = cal_found->events->next;
			}
		}
	}
	return 0;
}


/* 
 * Print to stdout a list of the events in this calendar ordered by time
 *  Return:
 *     0 if successful
 *    -1 if no calendar exists by this name
 */
int list_events(Calendar *cal_list, char *cal_name) {
	
	Event *tem_event;
	// allocate memory for temporary event
	if((tem_event = (Event *)malloc(sizeof(Event))) == NULL) {
		perror("malloc\n");
		exit(1);
	}
	Calendar *tem_cal;
	// allocate memory for temporary calendar
	if((tem_cal = (Calendar *)malloc(sizeof(Calendar)))== NULL){
		perror("malloc\n");
		exit(1);
	} 
	tem_cal = cal_list;
	while(tem_cal){
		// given calendar name is existed in calendar list
		if(strcmp(tem_cal->name, cal_name) == 0){
			tem_event = cal_list->events;
			// print all the events in this calendar
			while(tem_event){
				printf("%s, %s", tem_event->description,asctime(localtime(&tem_event->time)));
				tem_event = tem_event->next;
			}
			free(tem_event);
			free(tem_cal);
			return 0;
		}
		tem_cal = tem_cal->next;
	}
	free(tem_event);
	free(tem_cal);
	return -1;
}


/* 
 * Print to stdout, the description and time of the next event for 
 * user user_name
 *  Return:
 *   0 on success
 *   -1 if this user_name does not exist or has no events
 */
int print_next_event(Calendar *cal_list, User * user_list, char *user_name) {
   
	Event *earlist_event;
	Event *curr_event;
	User *temp_user1;
	Subscription *temp_sub1;
	Calendar *temp_cal1;
	double diff_time;
	temp_user1 = user_list;
	while(temp_user1){
		// user name existed in user list
		if (strcmp(temp_user1->name, user_name) == 0) {
			if(temp_user1->subscriptions == NULL){
				return -1;
			}
			temp_sub1 = temp_user1->subscriptions;
			// no event in the subscribed to this user
			while (temp_sub1) {
				temp_cal1 = temp_sub1->calendar;
				curr_event = temp_cal1->events;
				// no event in the events_list
				if (curr_event->description == NULL){
					return -1;
				}
				// events in the events_list
				if ((temp_cal1) && (curr_event)){
					if (earlist_event == NULL){
						earlist_event = curr_event;
					} else {
						diff_time = difftime(earlist_event->time, curr_event->time);
						if (diff_time > 0) {
							earlist_event = curr_event;
						}
					}
				} 
				temp_sub1 = temp_sub1->next;
			}
			printf("%s, %s", earlist_event->description, asctime(localtime(&earlist_event->time)));
			return 0;
		}
		temp_user1 = temp_user1->next;
	}
	return -1;
}

/*
* return a pointer indication list of all events.
*/
Event *make_all_event(Event *all_events_list, Event *temp_event) {
	
	double diff_time1, diff_time2;
	Event *new_event;
	if ((new_event = (Event *)malloc(sizeof(Event)))== NULL) {
		perror("malloc\n");
		exit(1);
	}
	if ((new_event->description = (char*)malloc(strlen(temp_event->description)+1)) == NULL) {
		perror("malloc\n");
		exit(1);
	}
	strcpy(new_event->description, temp_event->description);
	new_event->time = temp_event->time;
	new_event->next = NULL;
	// no event in all_events_list
	if(all_events_list == NULL){
		all_events_list = new_event;
	}
	// at least 1 or more events in all_events_lists
	else {
		while(all_events_list != NULL){
			diff_time1 = difftime(all_events_list->time, new_event->time);
			// new_cal should be inserted between two events
			if (all_events_list->next != NULL){
				if(diff_time1 < 0){
					diff_time2 = difftime(all_events_list->next->time, new_event->time);
					// found the place where new event inserted
					if (diff_time2 >= 0) {
						new_event->next = all_events_list->next;
						all_events_list->next = new_event;
						break;
					}
				// inser the event at the beginning of the list
				} else {
					new_event->next = all_events_list;
					all_events_list = new_event;
					break;
				}
			}
			// insert new_event at the last in linked list
			else if (diff_time1 <= 0){
				all_events_list->next = new_event;
				break;
			}
			all_events_list = all_events_list->next;
		}
	}
	return all_events_list;
}

/* 
 * Print to stdout a time-ordered list of all events in any calendars
 *  subscribed to by this user 
 *
 *  Do not change the calendar data-structure. 
 *  Return -1 if this user doesn't exist.
 */
int print_all_events(Calendar *cal_list, User * user_list, char *user_name) {
	
	Event *all_event;
	Event *temp_event;
	User *temp_user2;
	Subscription *temp_sub2;
	Calendar *temp_cal2;
	temp_user2 = user_list;
	while(temp_user2){
		// user name is in user_list
		if(strcmp(temp_user2->name, user_name) == 0){
			temp_sub2 = temp_user2->subscriptions;
			// checking user has subscription
			while(temp_sub2){
				temp_cal2 = temp_sub2->calendar;
				// checking subscription has calendar 
				while(temp_cal2){
					temp_event = temp_cal2->events;
					// checking calendar has events or not
					while(temp_event) {
						all_event = make_all_event(all_event, temp_event);
						temp_event = temp_event->next;
					}
					temp_cal2 = temp_cal2->next;
				}
				temp_sub2 = temp_sub2->next;
			}
		}
		temp_user2 = temp_user2->next;	
	}
	// User's subscription has no events
	if (all_event == NULL){
		return -1;
	}
	else {
		while(all_event){
			printf("%s, %s", all_event->description, asctime(localtime(&all_event->time)));
			all_event = all_event->next;
		}
	}
	return 0;
}
