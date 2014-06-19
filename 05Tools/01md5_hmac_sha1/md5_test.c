#include "md5.h"

char sig_buf[10240];
snprintf(sig_buf, 10240, "%d%s%s", .....................); 

string sign = md5sum(sig_buf, strlen(sig_buf)); 