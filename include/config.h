#ifndef __CONFIG_H__
#define __CONFIG_H__
void load(const char *conf_name);
const char *get_string(const char *key);
int get_int_default(const char* key,const int def);

#endif  //__CONFIG_H__
