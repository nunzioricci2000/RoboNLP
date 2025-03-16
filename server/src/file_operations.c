#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "types.h"
#include "cJSON.h"
#include "file_operations.h"


int separate_path(const char *path, char username[], char field_name[]) {
    char path_copy[strlen(path) + 1];
    strcpy(path_copy, path);

    char *folder = strtok(path_copy, "/");
    if (strcmp(folder, "user") != 0) {
        return -1; //invalid path
    }
    
    int found = 0;
    char* temp = NULL;
    temp = strtok(NULL, "/");

    if ( temp != NULL ) {
        found++;
        strcpy(username, temp);
        temp = strtok(NULL, "/");
            if ( temp != NULL ) {
                found++;
                strcpy(field_name, temp);
                if (strtok(NULL, "/") != NULL) {
                    return -1; //invalid path
                } 
            }
    }



    //Return 1 if there was only the username, 2 if there was both username and filename, 0 if neither 
    return found;
}

int is_valid_user(char *username) { 
    char path[PATH_LEN] = "/user/";
    strcat(path, username);
    return access(path, F_OK) == 0;
}

int is_valid_field_name(char *filename) {
    char *acceptable_names[] = FIELDS;
    int number_of_names = sizeof(acceptable_names)/sizeof(acceptable_names[0]);
    for (int i = 0; i < number_of_names; i++) {
        if (strcmp(filename, acceptable_names[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void user_profile_to_string(user_profile profile, char* buf) {
    cJSON *json = cJSON_CreateObject();
cJSON_AddStringToObject(json, "name", profile.name);
if (profile.extraversion >= 0) {
    cJSON_AddNumberToObject(json, "extraversion", profile.extraversion);
}  
if (profile.agreeableness >= 0) {
    cJSON_AddNumberToObject(json, "agreeableness", profile.agreeableness);
}
if (profile.conscientiousness >= 0) {
    cJSON_AddNumberToObject(json, "conscientiousness", profile.conscientiousness);
}
if (profile.emotional_stability >= 0) {
    cJSON_AddNumberToObject(json, "emotional_stability", profile.emotional_stability);
}
if (profile.openness_to_experience >= 0) {
    cJSON_AddNumberToObject(json, "openness_to_experience", profile.openness_to_experience);
}
if (strcmp(profile.facts, "") != 0) {
    cJSON_AddStringToObject(json, "facts", profile.facts);
}

char *content = cJSON_Print(json);
cJSON_Delete(json);
strcpy(buf, content);
free(content);
}

int overwrite_user_file(user_profile profile, char* username) {
    char path[PATH_LEN] = "/user/";
    strcat(path, username);
    
    char content[MAX_RESPONSE_BODY_SIZE];
    user_profile_to_string(profile, content);

    FILE *file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }

    fwrite(content, 1, sizeof(content), file);
    fclose(file);
    return 0;
}

int delete_user_file(char *username) {
    char path[PATH_LEN] = "/user/";
    strcat(path, username);

    printf("Deleting file: %s\n", path);
    return unlink(path);
}

int delete_field_from_user_file(char* username, char* file_name) {
    char file_content[MAX_RESPONSE_BODY_SIZE];
    if (get_user_file(username, file_content) < 0) {
        return -1;
    }
    cJSON *json = cJSON_Parse(file_content);
    if (json == NULL) {
        return -1;
    }

    cJSON_DeleteItemFromObject(json, file_name);
    char *content = cJSON_Print(json);
    cJSON_Delete(json);

    char path[PATH_LEN] = "/user/";
    strcat(path, username);
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }

    fwrite(content, 1, strlen(content), file);
    fclose(file);
    free(content);

    return 0;
}

int get_user_file(char *username, char *buffer) {
    char path[PATH_LEN] = "/user/";
    strcat(path, username);


    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return -1;
    }

    printf("Reading file: %s\n", path);

    size_t read = fread(buffer, 1, MAX_RESPONSE_BODY_SIZE, file);
    fclose(file);
    if (read == 0) {
        return -1;
    } 

    size_t length = read < MAX_RESPONSE_BODY_SIZE ? read : MAX_RESPONSE_BODY_SIZE-1;
    buffer[length] = '\0';

    printf("Read %zu bytes\nContent: \n%s\n", length, buffer);
    return length;
}

int get_field_from_user_file(char* field_name,char *username, char *buffer) {
    char file_content[MAX_RESPONSE_BODY_SIZE];
    if (get_user_file(username, file_content) < 0) {
        return -1;
    }

    cJSON *json = cJSON_Parse(file_content);
    if (json == NULL) {
        printf("Could not parse json file in memory.\n");
        return -1;
    }

    cJSON *item = cJSON_GetObjectItem(json, field_name);
    if (item == NULL) { //field not found
        snprintf(buffer, MAX_RESPONSE_BODY_SIZE, "{\n        \"%s\": null\n}", field_name);
    } else {
        char *content = cJSON_Print(item);
        snprintf(buffer, MAX_RESPONSE_BODY_SIZE, "{\n        \"%s\": %s\n}", field_name, content);
        free(content);
    }

    cJSON_Delete(json);


    return 0;
}

int put_user_file(char* username, user_profile updates_profile) {
    char buf[MAX_RESPONSE_BODY_SIZE];
    user_profile profile_to_update;
    if (get_user_file(username, buf) < 0) {
        return -1;
    }

    if (parse_user_profile(&profile_to_update, buf) < 0) {
        return -1;
    }  

    if (strcmp(updates_profile.name, "") != 0) {
        strcpy(profile_to_update.name, updates_profile.name);
    }
    if (updates_profile.extraversion >= 0) {
        profile_to_update.extraversion = updates_profile.extraversion;
    }
    if (updates_profile.agreeableness >= 0) {
        profile_to_update.agreeableness = updates_profile.agreeableness;
    }
    if (updates_profile.conscientiousness >= 0) {
        profile_to_update.conscientiousness = updates_profile.conscientiousness;
    }
    if (updates_profile.emotional_stability >= 0) {
        profile_to_update.emotional_stability = updates_profile.emotional_stability;
    }
    if (updates_profile.openness_to_experience >= 0) {
        profile_to_update.openness_to_experience = updates_profile.openness_to_experience;
    }
    if (strcmp(updates_profile.facts, "") != 0) {
        strcat(profile_to_update.facts, updates_profile.facts);
    }

    return overwrite_user_file(profile_to_update, username);
}

int post_user_file(user_profile profile) {
    char path[PATH_LEN] = "/user/";
    strcat(path, profile.username);
    
    char content[MAX_RESPONSE_BODY_SIZE];
    user_profile_to_string(profile, content);

    FILE *file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }


    fwrite(content, 1, sizeof(content), file);
    fclose(file);
    return 0;
}

void parse_string_field(char *dest, cJSON *json, const char* field) {
    char* temp = cJSON_GetStringValue(cJSON_GetObjectItem(json, field));
    if (temp == NULL) {
        strcpy(dest, "");
    } else {
        strcpy(dest, temp);
    }
}

void parse_double_field(double *dest, cJSON *json, const char* field) {
    cJSON *item = cJSON_GetObjectItem(json, field);
    if (item == NULL || !cJSON_IsNumber(item)) {
        *dest = -1;
    } else {
        *dest = cJSON_GetNumberValue(item);
    }
}

int parse_user_profile(user_profile* profile, char* buf) {  
    if (buf == NULL) {
        printf("Buffer is NULL in parse_user_profile\n");
        return -1;
    }
    cJSON *json = cJSON_Parse(buf);
    if (json == NULL) {
        printf("json is NULL in parse_user_profile\n");
        return -1;
    }

    parse_string_field(profile->username, json, "username");
    parse_string_field(profile->name, json, "name");
    
    parse_double_field(&profile->extraversion, json, "extraversion");
    parse_double_field(&profile->agreeableness, json, "agreeableness");
    parse_double_field(&profile->conscientiousness, json, "conscientiousness");
    parse_double_field(&profile->emotional_stability, json, "emotional_stability");
    parse_double_field(&profile->openness_to_experience, json, "openness_to_experience");

    parse_string_field(profile->facts, json, "facts");
    
    return 0;
}

int post_user_facts(char* username, char* buffer) {
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        return -1;
    }
    
    char *fact = cJSON_GetStringValue(cJSON_GetObjectItem(json, "fact"));

    if (fact == NULL) {
        return -1;
    }

    char buf[MAX_RESPONSE_BODY_SIZE];
    if (get_user_file(username, buf) < 0) {
        return -1;
    }

    user_profile profile;
    if (parse_user_profile(&profile, buf) < 0) {
        return -1;
    }
    strcat(profile.facts, "\n");
    strcat(profile.facts, fact);

    return overwrite_user_file(profile, username);
}

