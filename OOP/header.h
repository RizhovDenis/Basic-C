#ifndef HEADER_H
#define HEADER_H
#define NUMBER_ACCOUNTS 10
#define BUFFER_SIZE 20480

typedef struct account{
    char *first_name;
    char *last_name;
    int age;
    char *address;
}account;

typedef struct list_of_account{
    account **items;
    int number_accounts;
}list_of_account;

typedef struct format{
    void (*dump)();
    void (*decode)();
    const char *(*encode)();
}format;

typedef struct json{
    format action;
    char *label;
}json;

typedef struct xml{
    format action;
    char *label;
}xml;

typedef struct yaml{
    format action;
    char *label;
}yaml;


typedef struct cycle{
    format action;
    char *label;
}cycle;

struct json_object *json_format;
struct json_object *first_name;
struct json_object *last_name;
struct json_object *age;
struct json_object *address;

yaml_parser_t parser;
yaml_document_t document;
yaml_node_t *node;
yaml_emitter_t emitter;
yaml_event_t event;

json *init_json();
xml *init_xml();
yaml *init_yaml();
void write_to_json(json *json, const char *encoded_json, char *name);
void write_to_xml(xml *json, const char *encoded_xml, char *name);
void write_to_yaml(yaml *json, const char *encoded_yaml, char *name);
void json_parser(json *json, char *name);
void xml_parser(xml *xml, char *name);
void yaml_parser(yaml *yaml, char *name);
const char *encode_json(account *item);
const char *encode_xml(account *item);
const char *encode_yaml(account *item);

cycle **init_cycle(json *json_n, yaml *yaml_n, xml *xml_n);

list_of_account *init_list(int number_accounts);
list_of_account *generator_accounts(list_of_account *list);

#endif
