/* prefs.h -- see prefs.c. MIT licensed, see LICENSE. */
#ifndef TPK_PREFS_H
#define TPK_PREFS_H

/* Returns a malloc'd copy; "" when the key is unset (SharedPreferences
 * getString(key, "") semantics). */
char *prefs_get(const char *key);
void  prefs_set(const char *key, const char *value);

#endif
