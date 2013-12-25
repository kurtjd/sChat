#include <string.h>
#include <time.h>
#include "message.h"

void add_message(const int sender, const time_t timestamp, const char msg[], Message messages[])
{
    static int histlen = 0;
    messages[histlen].sender = sender;
    messages[histlen].timestamp = timestamp;
    strcpy(messages[histlen].txt, msg);

    if(histlen < (MAX_HISTORY - 1))
        ++histlen;
}
