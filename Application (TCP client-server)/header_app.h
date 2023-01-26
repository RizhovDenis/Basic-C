#ifndef HEADER_APP_H
#define HEADER_APP_H
#define STORAGE "Accounts"
#define MESSEGES_PATH "Messages"
#define READ_USER_STATUS "read_user"
#define ACTION_STATUS "action_with_message"
#define MAILBOX_STATUS "read_mailbox"
#define READ_MESSAGE_STATUS "read_mail"
#define LABEL ".json"
#define BUFFER_SIZE 20480
#define MESSAGES_LIMIT 100

struct json_object *json_format;
struct json_object *name;
struct json_object *lastname;
struct json_object *age;
struct json_object *hash_password_system;
struct json_object *message;

void create_user(char *login, int hash_password, char *name, char *lastname, char *age, int hash_filename, char *storage);
int read_user(char *login, int hash_filename, int hash_password, char *storage, char *status);
void send_message(char *from, int hash_filename, int hash_password, int hash_dest, char *destination, char *message, char *storage, char *status);
void read_mailbox(char *login, int hash_filename, int hash_password, char *storage, char *status, char *parse_status);
void read_mail(char *login, int hash_filename, int hash_password, char *message_id, char *storage, char *status, char *parse_status);

#endif