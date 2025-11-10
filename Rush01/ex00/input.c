#include "main.h"


uint8_t check_format(char *buffer)
{
    uint8_t i = 0;
    uint8_t digitIndex[14] = {0, 1, 3, 4, 6, 7, 8, 9, 11, 12, 14, 15, 17, 18};
    uint8_t slashIndex[2] = {2, 5};
    uint8_t colonIndex[2] = {13, 16};
    uint8_t spaceIndex = 10;
    while (i < 14)
    {
        if (!ft_isdigit(buffer[digitIndex[i]])) 
            return (0);
        i++;
    }
    if (buffer[spaceIndex] != ' ')
        return (0);
    if (buffer[slashIndex[0]] != '/' || buffer[slashIndex[1]] != '/')
        return (0);
    if (buffer[colonIndex[0]] != ':' || buffer[colonIndex[1]] != ':')
        return (0);
    return (1);
}


uint8_t *convert_input(char *buffer, uint8_t *time)
{
    uint8_t a = 0;
    uint8_t skipArray[6] = {3, 3, 2, 3, 3, 3};
    uint8_t i = 0;
    while (a < TIME_ARRAY_SIZE && *buffer)
    {
        time[a] = ft_atouint8(buffer);
        a++;
        buffer += skipArray[i];
        i++;
    }
    return (time);
}
// Format:  "28/02/2023 18:03:17"
// Size 19
// 0123456789/: 
uint8_t check_input(char *buffer)
{
    /* char split[INPUT_FORMAT_SIZE][INPUT_FORMAT_SIZE]; */
    if (ft_strlen(buffer) != INPUT_FORMAT_SIZE)
        return (0);
    //
    if (!ft_is_allowed_chars(buffer, ALLOWED_CHARS))
        return (0);
    if (!check_format(buffer))
        return (0);
    
    return (1);
}

uint8_t get_input_uart(char *buffer)
{
    static uint8_t i = 0;
    char c = 0;
     
    c = read_non_block_uart();
    if (c == '\0') // no new input
        return (0);
    if (c == END_OF_INPUT)
    {
        buffer[i] = '\0';
        i = 0;
        if (!check_input(buffer))
            return (putstr_uart(WRONG_FORMAT), 0);
        else
            return (1); // end of input need to check for format .. then set i = 0 and maybe empty buffer
    }
    else if (c == BACKSPACE)
    {
        if (i > 0)
        {
            putstr_uart(REMOVE_CHAR);
            i--;
        }
    }
    else if (i < INPUT_BUFFER_SIZE)
    {
        buffer[i] = c;
        i++;
    }
    write_uart(c);
    return (0);
}
